/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include "TouchHal.h"

#include <android-base/file.h>
#include <android-base/logging.h>

using ::android::base::ReadFileToString;
using ::android::base::WriteStringToFile;

namespace {

constexpr const char* kTapSensitivityPath = "/proc/report_threshold";
constexpr const char* kSwipeResponsivenessPath = "/proc/game_mode";
constexpr const char* kPenModePath = "/proc/support_pen";
constexpr const char* kPanelDirectionPath = "/proc/panel_direction";
constexpr const char* kCornerInhibitionPath = "/proc/game_edge";
constexpr const char* kHighTouchPollingRatePath = "/proc/HighReportRate";

ndk::ScopedAStatus readNode(const char* path, bool* enabled) {
    std::string value;
    if (!ReadFileToString(path, &value)) {
        PLOG(ERROR) << "Failed to read " << path;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    auto pos = value.find_last_of("01");
    if (pos == std::string::npos) {
        LOG(ERROR) << "Unexpected value in " << path << ": " << value;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    *enabled = value[pos] == '1';
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus writeNode(const char* path, int32_t value, int32_t maximum) {
    if (value < 0 || value > maximum) {
        return ndk::ScopedAStatus::fromExceptionCodeWithMessage(
                EX_ILLEGAL_ARGUMENT, "Unsupported touch value");
    }
    if (!WriteStringToFile(std::to_string(value), path, true)) {
        PLOG(ERROR) << "Failed to write " << path;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }
    return ndk::ScopedAStatus::ok();
}

}  // anonymous namespace

namespace aidl::vendor::lenovo::elden::touch {

ndk::ScopedAStatus Touch::setTapSensitivity(int32_t level) {
    return writeNode(kTapSensitivityPath, level, 3);
}

ndk::ScopedAStatus Touch::setSwipeResponsiveness(int32_t level) {
    return writeNode(kSwipeResponsivenessPath, level, 3);
}

ndk::ScopedAStatus Touch::setPenMode(bool enabled) {
    return writeNode(kPenModePath, enabled, 1);
}

ndk::ScopedAStatus Touch::setPanelDirection(int32_t rotation) {
    return writeNode(kPanelDirectionPath, rotation, 3);
}

ndk::ScopedAStatus Touch::setCornerInhibition(int32_t mode) {
    return writeNode(kCornerInhibitionPath, mode, 3);
}

}  // namespace aidl::vendor::lenovo::elden::touch

namespace aidl::vendor::lineage::touch {

ndk::ScopedAStatus HighTouchPollingRate::getEnabled(bool* _aidl_return) {
    return readNode(kHighTouchPollingRatePath, _aidl_return);
}

ndk::ScopedAStatus HighTouchPollingRate::setEnabled(bool enabled) {
    return writeNode(kHighTouchPollingRatePath, enabled, 1);
}

}  // namespace aidl::vendor::lineage::touch
