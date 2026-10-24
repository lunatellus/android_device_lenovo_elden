#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixups,
    lib_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
    'hardware/qcom-caf/sm8750',
    'hardware/qcom-caf/wlan',
    'hardware/qcom-caf/wlan/qcwcn',
    'vendor/qcom/opensource/commonsys/display',
    'vendor/qcom/opensource/commonsys-intf/display',
    'vendor/qcom/opensource/dataservices',
]


def lib_fixup_vendor_suffix(lib: str, partition: str, *args, **kwargs):
    return f'{lib}_{partition}' if partition == 'vendor' else None


lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
    (
        'vendor.qti.ImsRtpService-V1-ndk',
        'vendor.qti.diaghal-V1-ndk',
        'vendor.qti.diaghal@1.0',
    ): lib_fixup_vendor_suffix,
}


blob_fixups: blob_fixups_user_type = {
    'vendor/bin/hw/vendor.qti.hardware.display.composer-service': blob_fixup()
        .replace_needed('vendor.qti.hardware.display.composer3-V1-ndk.so', 'vendor.qti.hardware.display.composer3-V1-ndk_vendor.so')
        .replace_needed('libtinyxml2.so', 'libtinyxml2-v34.so'),
    (
        'vendor/bin/hw/android.hardware.security.keymint-service-spu-qti',
        'vendor/lib64/libspukeymint.so',
    ): blob_fixup()
        .replace_needed('android.hardware.security.sharedsecret-V2-ndk.so', 'android.hardware.security.sharedsecret-V2-ndk_vendor.so'),
    (
        'vendor/lib64/camera/components/com.qti.node.dewarp.so',
        'vendor/lib64/hw/com.qti.chi.override.so',
        'vendor/lib64/libcamximageformatutils.so',
        'vendor/lib64/libchifeature2.so',
        'vendor/lib64/libqvrservice.so',
        'vendor/lib64/vendor.qti.hardware.camera.offlinecamera-service-impl.so',
    ): blob_fixup()
        .replace_needed('android.hardware.graphics.allocator-V1-ndk.so', 'android.hardware.graphics.allocator-V1-ndk_vendor.so'),
    (
        'vendor/lib64/libVoiceSdk.so',
        'vendor/lib64/libcapiv2uvvendor.so',
        'vendor/lib64/liblistensoundmodel2vendor.so',
    ): blob_fixup()
        .replace_needed('libtensorflowlite_c.so', 'libtensorflowlite_c_vendor.so'),
    (
        'vendor/bin/poweropt-service',
        'vendor/lib64/hw/libaudioeffecthal.qti.so',
        'vendor/lib64/libaodoptfeature.so',
        'vendor/lib64/libapengine.so',
        'vendor/lib64/libcamerapoweroptfeature.so',
        'vendor/lib64/libcamxcoreutils.so',
        'vendor/lib64/libcamxods.so',
        'vendor/lib64/libgamepoweroptfeature.so',
        'vendor/lib64/liblearningmodule.so',
        'vendor/lib64/liboffscreenpoweroptfeature.so',
        'vendor/lib64/libpowercallback.so',
        'vendor/lib64/libpowercore.so',
        'vendor/lib64/libpsmoptfeature.so',
        'vendor/lib64/libsdmclient.so',
        'vendor/lib64/libstandbyfeature.so',
        'vendor/lib64/libvideooptfeature.so',
        'vendor/lib64/soundfx/libquasar.so',
    ): blob_fixup()
        .replace_needed('libtinyxml2.so', 'libtinyxml2-v34.so'),
    'vendor/etc/seccomp_policy/qsap_qapeservice.policy': blob_fixup()
        .add_line_if_missing('lseek: 1'),
    (
        'vendor/etc/wifi/p2p_supplicant_overlay.conf',
        'vendor/etc/wifi/wpa_supplicant_overlay.conf',
    ): blob_fixup()
        .add_line_if_missing('p2p_optimize_listen_chan=1')
        .add_line_if_missing('bss_no_flush_when_down=1'),
    (
        'vendor/lib64/c2.dolby.hevc.dec.so',
        'vendor/lib64/c2.dolby.hevc.sec.dec.so',
    ): blob_fixup()
        .add_needed('libcodec2_shim.so'),
    (
        'vendor/lib64/hw/android.hardware.bluetooth.audio_sw.so',
        'vendor/lib64/hw/audio.bluetooth_qti.default.so',
        'vendor/lib64/hw/libaudiocorehal.default.so',
        'vendor/lib64/hw/libaudiocorehal.qti.so',
        'vendor/lib64/libbluetooth_audio_session_qti.so',
        'vendor/lib64/libbluetooth_audio_session_qti_2_1.so',
    ): blob_fixup()
        .add_needed('libaudioutils_shim.so'),
    'vendor/lib64/libarcsoft_video_superportrait.so': blob_fixup()
        .clear_symbol_version('AHardwareBuffer_allocate')
        .clear_symbol_version('AHardwareBuffer_describe')
        .clear_symbol_version('AHardwareBuffer_lockPlanes')
        .clear_symbol_version('AHardwareBuffer_release')
        .clear_symbol_version('AHardwareBuffer_unlock'),
    'vendor/lib64/libaudioserviceexampleimpl.so': blob_fixup()
        .add_needed('libaudioutils_shim.so')
        .add_needed('libbluetooth_audio_session_aidl_shim.so'),
    'vendor/lib64/libqcodec2_core.so': blob_fixup()
        .remove_needed('android.hardware.graphics.common-V5-ndk.so'),
    'vendor/lib64/libqtigefar.so': blob_fixup()
        .remove_needed('android.hardware.audio.core-V2-ndk.so')
        .remove_needed('android.media.audio.common.types-V3-ndk.so'),
    'vendor/lib64/libsxrservice.so': blob_fixup()
        .replace_needed('android.hardware.common-V2-ndk_platform.so', 'android.hardware.common-V2-ndk.so'),
    'vendor/lib64/libwfdmmsrc_proprietary.so': blob_fixup()
        .remove_needed('android.media.audio.common.types-V2-ndk.so'),
}  # fmt: skip

module = ExtractUtilsModule(
    'elden',
    'lenovo',
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
    add_firmware_proprietary_file=True,
    check_elf=True,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
