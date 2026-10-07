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

constexpr const char* kGloveModePath = "/proc/report_threshold";
constexpr const char* kHighTouchPollingRatePath = "/proc/HighReportRate";

ndk::ScopedAStatus readNode(const char* path, bool* enabled) {
    std::string value;
    if (!ReadFileToString(path, &value)) {
        LOG(ERROR) << "Failed to read " << path;
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

ndk::ScopedAStatus writeNode(const char* path, bool enabled) {
    if (!WriteStringToFile(enabled ? "1" : "0", path, true)) {
        LOG(ERROR) << "Failed to write " << path;
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_STATE);
    }

    return ndk::ScopedAStatus::ok();
}

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

ndk::ScopedAStatus GloveMode::getEnabled(bool* _aidl_return) {
    return readNode(kGloveModePath, _aidl_return);
}

ndk::ScopedAStatus GloveMode::setEnabled(bool enabled) {
    return writeNode(kGloveModePath, enabled);
}

ndk::ScopedAStatus HighTouchPollingRate::getEnabled(bool* _aidl_return) {
    return readNode(kHighTouchPollingRatePath, _aidl_return);
}

ndk::ScopedAStatus HighTouchPollingRate::setEnabled(bool enabled) {
    return writeNode(kHighTouchPollingRatePath, enabled);
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
