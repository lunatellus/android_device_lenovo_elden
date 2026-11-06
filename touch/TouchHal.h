/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/vendor/lenovo/elden/touch/BnTouch.h>
#include <aidl/vendor/lineage/touch/BnHighTouchPollingRate.h>

namespace aidl::vendor::lenovo::elden::touch {

class Touch : public BnTouch {
  public:
    ndk::ScopedAStatus setTapSensitivity(int32_t level) override;
    ndk::ScopedAStatus setSwipeResponsiveness(int32_t level) override;
    ndk::ScopedAStatus setPenMode(bool enabled) override;
    ndk::ScopedAStatus setPanelDirection(int32_t rotation) override;
    ndk::ScopedAStatus setCornerInhibition(int32_t mode) override;
};

}  // namespace aidl::vendor::lenovo::elden::touch

namespace aidl::vendor::lineage::touch {

class HighTouchPollingRate : public BnHighTouchPollingRate {
  public:
    ndk::ScopedAStatus getEnabled(bool* _aidl_return) override;
    ndk::ScopedAStatus setEnabled(bool enabled) override;
};

}  // namespace aidl::vendor::lineage::touch
