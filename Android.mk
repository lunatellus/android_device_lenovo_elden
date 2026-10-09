#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),elden)
$(PRODUCT_OUT)/vbmeta_system.img: vendor/lenovo/elden/radio/pvmfw.img
endif
