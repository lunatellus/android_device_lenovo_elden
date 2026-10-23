/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Vibrator.h"

#include <android-base/file.h>
#include <android-base/logging.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

using ::android::base::ReadFileToString;
using ::android::base::unique_fd;
using ::android::base::WriteFully;
using ::android::base::WriteStringToFile;

namespace aidl::android::hardware::vibrator {
namespace {

constexpr uint16_t kMaxMagnitude = 0x7fff;

void restoreCalibration(const std::string& source, const std::string& destination) {
    std::string calibration;
    if (!ReadFileToString(source, &calibration) ||
        !WriteStringToFile(calibration, destination)) {
        PLOG(WARNING) << "Failed to restore vibrator calibration from " << source;
    }
}

}  // namespace

Vibrator::Vibrator() {
    std::unique_ptr<DIR, decltype(&closedir)> input(opendir("/dev/input"), closedir);
    CHECK(input) << "Failed to open input devices";

    while (auto* entry = readdir(input.get())) {
        if (strncmp(entry->d_name, "event", 5) != 0) {
            continue;
        }
        std::string path = std::string("/dev/input/") + entry->d_name;
        unique_fd fd(open(path.c_str(), O_RDWR | O_CLOEXEC));
        char name[80] = {};
        if (fd.get() < 0 || ioctl(fd.get(), EVIOCGNAME(sizeof(name)), name) < 0 ||
            strcmp(name, "aw-haptic-hv") != 0) {
            continue;
        }
        mMotors.push_back({std::move(fd), -1});
        if (mMotors.size() == 2) {
            break;
        }
    }
    CHECK_EQ(mMotors.size(), 2u) << "Failed to find both vibrator devices";

    restoreCalibration("/mnt/vendor/persist/factory/vibcali_l",
                       "/sys/class/leds/vibrator_l/f0_save");
    restoreCalibration("/mnt/vendor/persist/factory/vibcali_r",
                       "/sys/class/leds/vibrator_r/f0_save");
}

ndk::ScopedAStatus Vibrator::getCapabilities(int32_t* result) {
    *result = IVibrator::CAP_AMPLITUDE_CONTROL;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::stop() {
    int error = 0;
    for (auto& motor : mMotors) {
        if (motor.effectId == -1) {
            continue;
        }
        if (TEMP_FAILURE_RETRY(ioctl(motor.fd.get(), EVIOCRMFF, motor.effectId)) < 0) {
            error = errno;
            PLOG(ERROR) << "Failed to remove vibrator effect";
        } else {
            motor.effectId = -1;
        }
    }
    return error ? ndk::ScopedAStatus::fromServiceSpecificError(error)
                 : ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::setGain(uint16_t magnitude) {
    input_event event = {};
    event.type = EV_FF;
    event.code = FF_GAIN;
    event.value = magnitude;
    for (auto& motor : mMotors) {
        if (!WriteFully(motor.fd.get(), &event, sizeof(event))) {
            return ndk::ScopedAStatus::fromServiceSpecificError(errno);
        }
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::play(ff_effect effect, uint16_t magnitude) {
    auto status = stop();
    if (!status.isOk()) {
        return status;
    }
    for (auto& motor : mMotors) {
        effect.id = -1;
        if (TEMP_FAILURE_RETRY(ioctl(motor.fd.get(), EVIOCSFF, &effect)) < 0) {
            status = ndk::ScopedAStatus::fromServiceSpecificError(errno);
            stop();
            return status;
        }
        motor.effectId = effect.id;
    }

    status = setGain(magnitude);
    if (!status.isOk()) {
        stop();
        return status;
    }
    for (auto& motor : mMotors) {
        input_event event = {};
        event.type = EV_FF;
        event.code = motor.effectId;
        event.value = 1;
        if (!WriteFully(motor.fd.get(), &event, sizeof(event))) {
            status = ndk::ScopedAStatus::fromServiceSpecificError(errno);
            stop();
            return status;
        }
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::off() {
    mAmplitude = kMaxMagnitude;
    mPlayingEffect = false;
    return stop();
}

ndk::ScopedAStatus Vibrator::on(int32_t timeoutMs,
                               const std::shared_ptr<IVibratorCallback>& callback) {
    if (callback) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    if (timeoutMs <= 0) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }
    mPlayingEffect = false;
    ff_effect effect = {};
    effect.type = FF_CONSTANT;
    effect.replay.length = std::min<int32_t>(timeoutMs, UINT16_MAX);
    effect.u.constant.level = mAmplitude;
    return play(effect, mAmplitude);
}

ndk::ScopedAStatus Vibrator::perform(Effect effect, EffectStrength strength,
                                    const std::shared_ptr<IVibratorCallback>& callback,
                                    int32_t* durationMs) {
    if (callback) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    int16_t waveform;
    switch (effect) {
        case Effect::CLICK:
        case Effect::DOUBLE_CLICK:
            waveform = 1;
            break;
        case Effect::TICK:
            waveform = 2;
            break;
        case Effect::THUD:
        case Effect::POP:
            waveform = 3;
            break;
        case Effect::HEAVY_CLICK:
            waveform = 5;
            break;
        default:
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    int16_t magnitude;
    switch (strength) {
        case EffectStrength::LIGHT:
            magnitude = 0x3fff;
            break;
        case EffectStrength::MEDIUM:
            magnitude = 0x5fff;
            break;
        case EffectStrength::STRONG:
            magnitude = 0x7fff;
            break;
        default:
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    mPlayingEffect = true;
    int16_t data[] = {waveform, magnitude, 0};
    ff_effect request = {};
    request.type = FF_PERIODIC;
    request.u.periodic.waveform = FF_CUSTOM;
    request.u.periodic.magnitude = magnitude;
    request.u.periodic.custom_len = sizeof(data);
    request.u.periodic.custom_data = data;
    auto status = play(request, magnitude);
    if (status.isOk()) {
        *durationMs = 100;
    }
    return status;
}

ndk::ScopedAStatus Vibrator::getSupportedEffects(std::vector<Effect>* effects) {
    *effects = {Effect::CLICK, Effect::DOUBLE_CLICK, Effect::TICK,
                Effect::THUD, Effect::POP, Effect::HEAVY_CLICK};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::setAmplitude(float amplitude) {
    if (!(amplitude > 0.0f && amplitude <= 1.0f)) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }
    mAmplitude = static_cast<uint16_t>(amplitude * kMaxMagnitude);
    return mPlayingEffect ? ndk::ScopedAStatus::ok() : setGain(mAmplitude);
}

ndk::ScopedAStatus Vibrator::setExternalControl(bool /* enabled */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getCompositionDelayMax(int32_t* /* delayMs */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getCompositionSizeMax(int32_t* /* size */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getSupportedPrimitives(std::vector<CompositePrimitive>* primitives) {
    primitives->clear();
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::getPrimitiveDuration(CompositePrimitive /* primitive */,
                                               int32_t* /* durationMs */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::compose(const std::vector<CompositeEffect>& /* composite */,
                                    const std::shared_ptr<IVibratorCallback>& /* callback */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getSupportedAlwaysOnEffects(std::vector<Effect>* /* effects */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::alwaysOnEnable(int32_t /* id */, Effect /* effect */,
                                          EffectStrength /* strength */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::alwaysOnDisable(int32_t /* id */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getResonantFrequency(float* /* frequencyHz */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getQFactor(float* /* qFactor */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getFrequencyResolution(float* /* resolutionHz */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getFrequencyMinimum(float* /* minimumHz */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getBandwidthAmplitudeMap(std::vector<float>* /* amplitudes */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getPwlePrimitiveDurationMax(int32_t* /* durationMs */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getPwleCompositionSizeMax(int32_t* /* size */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getSupportedBraking(std::vector<Braking>* /* braking */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::composePwle(const std::vector<PrimitivePwle>& /* composite */,
                                       const std::shared_ptr<IVibratorCallback>& /* callback */) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

}  // namespace aidl::android::hardware::vibrator
