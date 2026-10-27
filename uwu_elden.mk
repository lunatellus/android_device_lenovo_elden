#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit_only.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

# Inherit from elden device
$(call inherit-product, device/lenovo/elden/device.mk)

# Inherit some common uwuAOSP stuff.
UWU_DEVICE_TYPE := tablet
UWU_SUPPORTS_TELEPHONY := false
$(call inherit-product, vendor/uwu/config/common.mk)

PRODUCT_NAME := uwu_elden
PRODUCT_DEVICE := elden
PRODUCT_MANUFACTURER := LENOVO
PRODUCT_BRAND := Lenovo
PRODUCT_MODEL := TB322FC
PRODUCT_CHARACTERISTICS := nosdcard,tablet

PRODUCT_GMS_CLIENTID_BASE := android-lenovo
