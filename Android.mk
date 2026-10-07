#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),elden)
$(PRODUCT_OUT)/vbmeta_system.img: vendor/lenovo/elden/radio/pvmfw.img

# Recovery loads these modules before vendor_dlkm is mounted.
define elden-recovery-module
include $$(CLEAR_VARS)
LOCAL_MODULE := $(1).ko_recovery
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := ../elden-kernel/vendor_dlkm/$(1).ko
LOCAL_MODULE_STEM := $(1).ko
LOCAL_MODULE_PATH := $$(TARGET_RECOVERY_ROOT_OUT)/vendor/lib/modules
include $$(BUILD_PREBUILT)
endef

$(foreach module,adsp_loader_dlkm gpr_dlkm q6_notifier_dlkm q6_pdr_dlkm qti_battery_charger snd_event_dlkm spf_core_dlkm,$(eval $(call elden-recovery-module,$(module))))
endif
