/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/vibrator/BnVibrator.h>
#include <android-base/unique_fd.h>
#include <linux/input.h>

namespace aidl::android::hardware::vibrator {

class Vibrator : public BnVibrator {
public:
    Vibrator();

    ndk::ScopedAStatus getCapabilities(int32_t* result) override;
    ndk::ScopedAStatus off() override;
    ndk::ScopedAStatus on(int32_t timeoutMs,
                         const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus perform(Effect effect, EffectStrength strength,
                              const std::shared_ptr<IVibratorCallback>& callback,
                              int32_t* durationMs) override;
    ndk::ScopedAStatus getSupportedEffects(std::vector<Effect>* effects) override;
    ndk::ScopedAStatus setAmplitude(float amplitude) override;
    ndk::ScopedAStatus setExternalControl(bool enabled) override;
    ndk::ScopedAStatus getCompositionDelayMax(int32_t* delayMs) override;
    ndk::ScopedAStatus getCompositionSizeMax(int32_t* size) override;
    ndk::ScopedAStatus getSupportedPrimitives(std::vector<CompositePrimitive>* primitives) override;
    ndk::ScopedAStatus getPrimitiveDuration(CompositePrimitive primitive, int32_t* durationMs) override;
    ndk::ScopedAStatus compose(const std::vector<CompositeEffect>& composite,
                              const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus getSupportedAlwaysOnEffects(std::vector<Effect>* effects) override;
    ndk::ScopedAStatus alwaysOnEnable(int32_t id, Effect effect, EffectStrength strength) override;
    ndk::ScopedAStatus alwaysOnDisable(int32_t id) override;
    ndk::ScopedAStatus getResonantFrequency(float* frequencyHz) override;
    ndk::ScopedAStatus getQFactor(float* qFactor) override;
    ndk::ScopedAStatus getFrequencyResolution(float* resolutionHz) override;
    ndk::ScopedAStatus getFrequencyMinimum(float* minimumHz) override;
    ndk::ScopedAStatus getBandwidthAmplitudeMap(std::vector<float>* amplitudes) override;
    ndk::ScopedAStatus getPwlePrimitiveDurationMax(int32_t* durationMs) override;
    ndk::ScopedAStatus getPwleCompositionSizeMax(int32_t* size) override;
    ndk::ScopedAStatus getSupportedBraking(std::vector<Braking>* braking) override;
    ndk::ScopedAStatus composePwle(const std::vector<PrimitivePwle>& composite,
                                  const std::shared_ptr<IVibratorCallback>& callback) override;

private:
    struct Motor {
        ::android::base::unique_fd fd;
        int16_t effectId = -1;
    };

    ndk::ScopedAStatus play(ff_effect effect, uint16_t magnitude);
    ndk::ScopedAStatus stop();
    ndk::ScopedAStatus setGain(uint16_t magnitude);

    std::vector<Motor> mMotors;
    uint16_t mAmplitude = 0x7fff;
    bool mPlayingEffect = false;
};

}  // namespace aidl::android::hardware::vibrator
