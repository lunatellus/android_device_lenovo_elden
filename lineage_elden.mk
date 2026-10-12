#
# Copyright (C) 2024 The Android Open Source Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Inherit from products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit_only.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

# Inherit some common Lineage stuff.
$(call inherit-product, vendor/lineage/config/common_full_tablet_wifionly.mk)

# Inherit from elden device.
$(call inherit-product, device/lenovo/elden/device.mk)

# Device identifier
PRODUCT_DEVICE := elden
PRODUCT_NAME := lineage_elden
PRODUCT_BRAND := Lenovo
PRODUCT_MODEL := TB322FC
PRODUCT_MANUFACTURER := LENOVO
PRODUCT_CHARACTERISTICS := nosdcard,tablet

# GMS
PRODUCT_GMS_CLIENTID_BASE := android-lenovo
