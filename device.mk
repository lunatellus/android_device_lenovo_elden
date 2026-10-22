#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

# Add common definitions for Qualcomm
$(call inherit-product, hardware/qcom-caf/common/common.mk)

# A/B
$(call inherit-product, $(SRC_TARGET_DIR)/product/virtual_ab_ota/vabc_features.mk)

AB_OTA_POSTINSTALL_CONFIG += \
    RUN_POSTINSTALL_system=true \
    POSTINSTALL_PATH_system=system/bin/otapreopt_script \
    FILESYSTEM_TYPE_system=ext4 \
    POSTINSTALL_OPTIONAL_system=true

AB_OTA_POSTINSTALL_CONFIG += \
    RUN_POSTINSTALL_vendor=true \
    POSTINSTALL_PATH_vendor=bin/checkpoint_gc \
    FILESYSTEM_TYPE_vendor=erofs \
    POSTINSTALL_OPTIONAL_vendor=true

PRODUCT_PACKAGES += \
    checkpoint_gc \
    otapreopt_script

PRODUCT_VIRTUAL_AB_COMPRESSION_METHOD := lz4

# AAPT
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := xxhdpi

# ANT+
PRODUCT_PACKAGES += \
    com.dsi.ant@1.0.vendor

# Audio
PRODUCT_PACKAGES += \
    android.hardware.audio.common-V3-ndk.vendor \
    android.hardware.audio.common-V4-ndk.vendor \
    android.hardware.audio.common@5.0.vendor \
    android.hardware.audio.core.sounddose-V1-ndk.vendor \
    android.hardware.audio.core.sounddose-V2-ndk.vendor \
    android.hardware.audio.effect-V2-ndk.vendor \
    android.media.audio.common.types-V2-ndk.vendor \
    android.media.audio.common.types-V3-ndk.vendor \
    android.media.audio.common.types-V4-ndk.vendor \
    audio.primary.default \
    audio.r_submix.default \
    audio.usb.default \
    audioadsprpcd \
    audiohalservice.qti \
    audioeffectservice_qti.xml \
    manifest_audiocorehal_default.xml \
    libagm \
    libagm_compress_plugin \
    libagm_mixer_plugin \
    libagm_pcm_plugin \
    libagmclient \
    libagmipcservice \
    libalsautils.vendor \
    libalsautilsv2.vendor \
    libar-acdb \
    libar-gpr \
    libar-gsl \
    libats \
    libaudio_aidl_conversion_common_ndk.vendor \
    libaudioaidlcommon.vendor \
    libaudiochargerlistener \
    libaudioroute.vendor \
    libaudioutils.vendor \
    libbatterylistener \
    libcustomva_intf \
    libfmpal \
    libhfp_pal \
    liblx-ar_util \
    liblx-osal \
    libmediautils_vendor.vendor \
    libmemunreachable.vendor \
    libnbaio_mono \
    libpalclient \
    libpaleventnotifier \
    libpalipcservice \
    libsndcardparser \
    libsoundtriggerhal.qti \
    libtinyalsa.vendor \
    libtinyalsav2.vendor \
    libtinycompress \
    libvui_intf \
    qti-audio-types-aidl-V1-ndk.vendor \
    vendor.qti.hardware.agm-V1-ndk \
    vendor.qti.hardware.pal-V1-ndk \
    vendor.qti.hardware.paleventnotifier-V2-ndk.vendor

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/audio/audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_policy_configuration.xml \
    frameworks/av/services/audiopolicy/config/bluetooth_audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/bluetooth_audio_policy_configuration.xml

# Bluetooth
PRODUCT_PACKAGES += \
    aidl_default_audio_set_configurations_json \
    aidl_default_audio_set_scenarios_json \
    android.hardware.bluetooth-V1-ndk.vendor \
    android.hardware.bluetooth.audio-V5-ndk.vendor \
    android.hardware.bluetooth.audio-impl \
    android.hardware.bluetooth.audio@2.0.vendor \
    android.hardware.bluetooth.audio@2.1.vendor \
    android.hardware.bluetooth.finder-V1-ndk.vendor \
    android.hardware.bluetooth.lmp_event-V1-ndk.vendor \
    android.hardware.bluetooth@1.0.vendor \
    lib_bt_aptx \
    lib_bt_ble \
    lib_bt_bundle \
    libbluetooth_audio_session_aidl_shim \
    vendor.qti.hardware.bluetooth.audio-V1-ndk.vendor \
    vendor.qti.hardware.bluetooth_audio@2.0.vendor \
    vendor.qti.hardware.bluetooth_audio@2.1.vendor

# Boot animation
TARGET_SCREEN_HEIGHT := 3040
TARGET_SCREEN_WIDTH := 1904

# Boot control
PRODUCT_PACKAGES += \
    android.hardware.boot-service.qti \
    android.hardware.boot-service.qti.recovery

# Camera
PRODUCT_PACKAGES += \
    android.hardware.camera.common-V1-ndk.vendor \
    android.hardware.camera.device-V2-ndk.vendor \
    android.hardware.camera.metadata-V2-ndk.vendor \
    android.hardware.camera.provider-V2-ndk.vendor \
    libcamera_metadata.vendor \
    vendor.qti.hardware.camera.aon-V2-ndk.vendor \
    vendor.qti.hardware.camera.offlinecamera-V2-ndk.vendor

# Dalvik
$(call inherit-product, frameworks/native/build/phone-xhdpi-6144-dalvik-heap.mk)

# DRM
PRODUCT_PACKAGES += \
    android.hardware.drm-V1-ndk.vendor \
    android.hardware.drm-V2-ndk.vendor \
    android.hardware.drm-service.clearkey \
    com.android.hardware.cas

# Enforce generic ramdisk allow list
$(call inherit-product, $(SRC_TARGET_DIR)/product/generic_ramdisk.mk)

# Fastboot
PRODUCT_PACKAGES += \
    android.hardware.fastboot-service.example_recovery

# Graphics
PRODUCT_PACKAGES += \
    android.hardware.graphics.allocator-V2-ndk.vendor \
    android.hardware.graphics.common-V7-ndk.vendor \
    android.hardware.graphics.composer3-V4-ndk.vendor \
    android.hardware.graphics.mapper@4.0-impl-qti-display \
    gralloc.default \
    init.qti.display_boot.rc \
    init.qti.display_boot.sh \
    libdrmutils \
    libgpu_tonemapper \
    libqservice \
    mapper.qti \
    vendor.display.config@1.0.vendor \
    vendor.display.config@2.0.vendor \
    vendor.qti.hardware.display.aiqe-V2-ndk.vendor \
    vendor.qti.hardware.display.allocator-service \
    vendor.qti.hardware.display.color-V1-ndk.vendor \
    vendor.qti.hardware.display.composer-service.rc \
    vendor.qti.hardware.display.composer-service.xml \
    vendor.qti.hardware.display.config-V12-ndk.vendor \
    vendor.qti.hardware.display.config-V2-ndk.vendor \
    vendor.qti.hardware.display.config-V5-ndk.vendor \
    vendor.qti.hardware.display.config-V7-ndk.vendor \
    vendor.qti.hardware.display.demura-V1-ndk.vendor \
    vendor.qti.hardware.display.demura-service \
    vendor.qti.hardware.display.postproc-V1-ndk.vendor

PRODUCT_COPY_FILES += \
    hardware/qcom-caf/sm8750/display/core/config/snapdragon_color_libs_config.xml:$(TARGET_COPY_OUT_VENDOR)/etc/snapdragon_color_libs_config.xml

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.software.opengles.deqp.level-2024-03-01.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.software.opengles.deqp.level.xml \
    frameworks/native/data/etc/android.software.vulkan.deqp.level-2024-03-01.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.software.vulkan.deqp.level.xml

# HWUI
TARGET_USES_VULKAN := true

# Hardware
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/handheld_core_hardware.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/handheld_core_hardware.xml

# Health
PRODUCT_PACKAGES += \
    android.hardware.health-service.qti \
    android.hardware.health-service.qti_recovery \
    lineage_charger_animation_vendor \
    lineage_charger_battery_fail_vendor \
    lineage_charger_battery_scale_vendor \
    lineage_charger_font_vendor

# Init
PRODUCT_PACKAGES += \
    fstab.qcom \
    init.recovery.qcom.rc \
    init.target.rc \
    ueventd.qcom.rc \
    vendor_bt_firmware_mountpoint \
    vendor_dsp_mountpoint \
    vendor_firmware_mnt_mountpoint

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/init/fstab.qcom:$(TARGET_COPY_OUT_VENDOR_RAMDISK)/first_stage_ramdisk/fstab.qcom

# Input
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/idc/Vendor_17ef_Product_617f.idc:$(TARGET_COPY_OUT_VENDOR)/usr/idc/Vendor_17ef_Product_617f.idc \
    $(LOCAL_PATH)/configs/keylayout/Vendor_17ef_Product_617f.kl:$(TARGET_COPY_OUT_VENDOR)/usr/keylayout/Vendor_17ef_Product_617f.kl

# Kernel
PRODUCT_COPY_FILES += \
    kernel/platform/kernel-6.12/vendor/lenovo/sm8750/modules-lists/modules.systemdlkm_blocklist.msm.sun:$(TARGET_COPY_OUT_VENDOR_DLKM)/lib/modules/system_dlkm.modules.blocklist

# Keymint
PRODUCT_PACKAGES += \
    android.hardware.gatekeeper-V1-ndk.vendor \
    android.hardware.hardware_keystore.xml \
    android.hardware.identity-V5-ndk.vendor \
    android.hardware.keymaster-V3-ndk.vendor \
    android.hardware.keymaster@3.0.vendor \
    android.hardware.keymaster@4.0.vendor \
    android.hardware.keymaster@4.1.vendor \
    android.hardware.security.keymint-V1-ndk.vendor \
    android.hardware.security.keymint-V3-ndk.vendor \
    android.hardware.security.keymint-V4-ndk.vendor \
    android.hardware.security.rkp-V3-ndk.vendor \
    android.hardware.security.secureclock-V1-ndk.vendor \
    android.hardware.security.sharedsecret-V1-ndk.vendor \
    android.hardware.weaver-V2-ndk.vendor \
    android.system.keystore2-V1-ndk.vendor \
    libgatekeeper.vendor \
    libkeymaster_messages.vendor

# Lineage Health
PRODUCT_PACKAGES += \
    vendor.lineage.health-service.default

$(call soong_config_set,lineage_health,charging_control_charging_path,/sys/class/power_supply/battery/battery_maintenance20)
$(call soong_config_set,lineage_health,charging_control_charging_enabled,0)
$(call soong_config_set,lineage_health,charging_control_charging_disabled,1)

# Media
PRODUCT_PACKAGES += \
    android.hardware.media.bufferpool2-V2-ndk.vendor \
    android.hardware.media.bufferpool@2.0.vendor \
    android.hardware.media.c2-V1-ndk.vendor \
    android.hardware.media.c2@1.0.vendor \
    android.hardware.media.c2@1.1.vendor \
    android.hardware.media.c2@1.2.vendor \
    android.hardware.media.omx@1.0.vendor \
    android.hardware.media@1.0.vendor \
    libavservices_minijail.vendor \
    libcodec2.vendor \
    libcodec2_aidl.vendor \
    libcodec2_hal_common.vendor \
    libcodec2_hidl@1.0.vendor \
    libcodec2_hidl@1.1.vendor \
    libcodec2_hidl@1.2.vendor \
    libcodec2_hidl_plugin \
    libcodec2_shim \
    libcodec2_soft_common.vendor \
    libcodec2_vndk.vendor \
    libsfplugin_ccodec_utils.vendor \
    libstagefright_aidl_bufferpool2.vendor \
    libstagefright_bufferpool@2.0.1.vendor \
    libstagefright_foundation.vendor

# Memtrack
PRODUCT_PACKAGES += \
    vendor.qti.hardware.memtrack-service

# Overlays
DEVICE_PACKAGE_OVERLAYS += \
    $(LOCAL_PATH)/overlay-lineage

PRODUCT_ENFORCE_RRO_TARGETS := *

PRODUCT_PACKAGES += \
    FrameworksResTarget \
    SettingsResTarget \
    WifiResTarget

# Partitions
PRODUCT_USE_DYNAMIC_PARTITIONS := true

# Parts
PRODUCT_PACKAGES += \
    LenovoParts

# Power
PRODUCT_PACKAGES += \
    android.hardware.power-V6-ndk.vendor \
    android.hardware.power-service-qti \
    android.hardware.power@1.0.vendor \
    android.hardware.power@1.1.vendor \
    android.hardware.power@1.2.vendor \
    power.default

PRODUCT_COPY_FILES += \
    vendor/qcom/opensource/power/config/sun/powerhint.xml:$(TARGET_COPY_OUT_VENDOR)/etc/powerhint.xml

$(call soong_config_set,qtipower,tap_to_wake_node,/proc/gesture_control)

# QSPA
PRODUCT_PACKAGES += \
    qspa_vendor.rc \
    vendor.qti.hardware.qspa-V1-ndk.vendor \
    vendor.qti.qspa-service

# RIL
PRODUCT_PACKAGES += \
    android.hardware.radio-V3-ndk.vendor \
    android.hardware.radio.data-V3-ndk.vendor \
    android.hardware.radio.network-V3-ndk.vendor \
    android.hardware.radio@1.0.vendor \
    android.hardware.radio@1.1.vendor \
    librmnetctl

# Sensors
PRODUCT_PACKAGES += \
    android.frameworks.sensorservice-V1-ndk.vendor \
    android.hardware.sensors-V3-ndk.vendor \
    android.hardware.sensors-service.multihal \
    android.hardware.sensors@1.0.vendor \
    android.hardware.sensors@2.0-ScopedWakelock.vendor \
    android.hardware.sensors@2.0.vendor \
    android.hardware.sensors@2.1.vendor \
    libsensorndkbridge

PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.sensor.dynamic.head_tracker.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.sensor.dynamic.head_tracker.xml

# Shipping API
BOARD_SHIPPING_API_LEVEL := 202404
PRODUCT_SHIPPING_API_LEVEL := 35

# Soong namespaces
PRODUCT_SOONG_NAMESPACES += \
    $(LOCAL_PATH)

# Storage
$(call inherit-product, $(SRC_TARGET_DIR)/product/emulated_storage.mk)

# Thermal
PRODUCT_PACKAGES += \
    android.hardware.thermal-V1-ndk.vendor \
    android.hardware.thermal-V2-ndk.vendor \
    android.hardware.thermal-service.qti \
    android.hardware.thermal@1.0.vendor

# Touch
PRODUCT_PACKAGES += \
    vendor.lineage.touch-service.elden

# Update engine
PRODUCT_PACKAGES += \
    update_engine \
    update_engine_sideload \
    update_verifier

PRODUCT_PACKAGES_DEBUG += \
    update_engine_client

# USB
PRODUCT_PACKAGES += \
    android.hardware.usb-V1-ndk.vendor \
    android.hardware.usb-service.qti \
    android.hardware.usb.gadget-V1-ndk.vendor \
    android.hardware.usb.gadget-service.qti \
    android.hardware.usb.gadget@1.0.vendor \
    android.hardware.usb.gadget@1.1.vendor

PRODUCT_PACKAGES += \
    init.qcom.usb.rc \
    init.qcom.usb.sh \
    usb_compositions.conf

PRODUCT_SOONG_NAMESPACES += vendor/qcom/opensource/usb/etc

# Vibrator
PRODUCT_PACKAGES += \
    android.hardware.vibrator-V2-ndk.vendor \
    vendor.qti.hardware.vibrator.service

PRODUCT_COPY_FILES += \
    vendor/qcom/opensource/vibrator/excluded-input-devices.xml:$(TARGET_COPY_OUT_VENDOR)/etc/excluded-input-devices.xml

# Virtualization
$(call inherit-product, packages/modules/Virtualization/apex/product_packages.mk)

# Vndservice manager
PRODUCT_PACKAGES += \
    vndservicemanager

# WiFi
PRODUCT_PACKAGES += \
    android.hardware.wifi-V4-ndk.vendor \
    android.hardware.wifi-service \
    android.hardware.wifi.common-V1-ndk.vendor \
    android.hardware.wifi.common-V2-ndk.vendor \
    android.hardware.wifi.hostapd-V2-ndk.vendor \
    android.hardware.wifi.hostapd-V3-ndk.vendor \
    android.hardware.wifi.supplicant-V3-ndk.vendor \
    android.hardware.wifi.supplicant-V5-ndk.vendor \
    hostapd \
    hostapd_cli \
    libcld80211 \
    libkeystore-engine-wifi-hidl \
    libpasn \
    libwifi-hal \
    libwifi-hal-ctrl \
    libwifi-hal-qcom \
    libwpa_client \
    wpa_cli \
    wpa_supplicant \
    wpa_supplicant.conf

# WiFi firmware symlinks
PRODUCT_PACKAGES += \
    firmware_WCNSS_qcom_cfg.ini_symlink \
    firmware_wlan_mac.bin_symlink \
    firmware_wlanmdsp.otaupdate_symlink

# Inherit from the proprietary files makefile.
$(call inherit-product, vendor/lenovo/elden/elden-vendor.mk)
