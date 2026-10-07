/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.elden"

#include "TouchHal.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::vendor::lineage::touch::GloveMode;
using aidl::vendor::lineage::touch::HighTouchPollingRate;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    std::shared_ptr<GloveMode> gm = ndk::SharedRefBase::make<GloveMode>();
    const std::string gmInstance = std::string(GloveMode::descriptor) + "/default";
    binder_status_t status = AServiceManager_addService(gm->asBinder().get(), gmInstance.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to add service " << gmInstance;

    std::shared_ptr<HighTouchPollingRate> htpr =
            ndk::SharedRefBase::make<HighTouchPollingRate>();
    const std::string htprInstance = std::string(HighTouchPollingRate::descriptor) + "/default";
    status = AServiceManager_addService(htpr->asBinder().get(), htprInstance.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to add service " << htprInstance;

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
