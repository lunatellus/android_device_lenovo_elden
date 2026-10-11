#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

import json
from pathlib import Path

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
    return f'{lib}-{partition}' if partition == 'vendor' else None


lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
    (
        'vendor.qti.ImsRtpService-V1-ndk',
        'vendor.qti.diaghal-V1-ndk',
        'vendor.qti.diaghal@1.0',
    ): lib_fixup_vendor_suffix,
}


def prebuilt(*libs: str) -> blob_fixup:
    fixup = blob_fixup()
    for lib in libs:
        fixup = fixup.replace_needed(f'{lib}.so', f'{lib}_prebuilt.so')
    return fixup


def fixup_le_audio(ctx, file, file_path, *args, **kwargs):
    path = Path(file_path)
    document = json.loads(path.read_text())
    strategies = {
        'MONO_ONE_CIS_PER_DEVICE': (1, 1),
        'STEREO_TWO_CISES_PER_DEVICE': (2, 1),
        'STEREO_ONE_CIS_PER_DEVICE': (1, 2),
    }
    for config in document['codec_configurations']:
        rows = []
        for row in config['subconfigurations']:
            # Already converted dumps can be extracted again.
            if 'device_cnt' not in row and 'configuration_strategy' not in row:
                if row.get('ase_channel_cnt') not in (1, 2) or row['ase_cnt'] not in (1, 2):
                    raise ValueError('Invalid LE Audio subconfiguration')
                rows.append(row)
                continue
            strategy = row.pop('configuration_strategy', 'MONO_ONE_CIS_PER_DEVICE')
            ases, channels = strategies[strategy]
            devices = row.pop('device_cnt')
            total = row['ase_cnt']
            if type(devices) is not int or devices not in (1, 2) or total != devices * ases:
                raise ValueError('Inconsistent LE Audio device/ASE count')
            row['ase_channel_cnt'] = channels
            # The AOSP consumer expands only one or two ASEs per row.
            for offset in range(0, total, 2):
                rows.append({**row, 'ase_cnt': min(2, total - offset)})
        config['subconfigurations'] = rows
    path.write_text(json.dumps(document, indent=2) + '\n')


blob_fixups: blob_fixups_user_type = {
    (
        'vendor/bin/hw/android.hardware.security.keymint-service-spu-qti',
        'vendor/lib64/libspukeymint.so',
    ): prebuilt('android.hardware.security.sharedsecret-V2-ndk'),
    'vendor/etc/aidl/le_audio/aidl_audio_set_configurations.json': blob_fixup()
        .call(fixup_le_audio, need_tmp_dir=False),
    (
        'vendor/bin/poweropt-service',
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
    ): prebuilt('libtinyxml2'),
    (
        'vendor/bin/hw/android.hardware.identity-service-qti',
        'vendor/lib64/libqtiidentitycredential.so',
        'vendor/lib64/libspukeymintprovision.so',
    ): prebuilt('android.hardware.security.keymint-V2-ndk'),
    'vendor/bin/hw/vendor.qti.hardware.display.composer-service': prebuilt(
        'android.hardware.graphics.composer3-V3-ndk',
        'vendor.qti.hardware.display.composer3-V1-ndk',
        'libtinyxml2',
    ),
    'vendor/etc/init/hw/init.qcom.rc': blob_fixup()
        .regex_replace(r'\nservice qti-testscripts .*\n(    .*\n)+', '\n')
        .regex_replace(r'\n    start qti-testscripts\n', '\n'),
    'vendor/etc/seccomp_policy/syshealthmon.policy': blob_fixup()
        .add_line_if_missing('lseek: 1'),
    (
        'vendor/etc/wifi/p2p_supplicant_overlay.conf',
        'vendor/etc/wifi/wpa_supplicant_overlay.conf',
    ): blob_fixup()
        .add_line_if_missing('p2p_optimize_listen_chan=1')
        .add_line_if_missing('bss_no_flush_when_down=1'),
    (
        'vendor/lib64/btaudio_offload_if.so',
        'vendor/lib64/hw/android.hardware.bluetooth.audio-impl-qti.so',
        'vendor/lib64/libbluetooth_audio_session_aidl_qti.so',
    ): prebuilt('android.hardware.bluetooth.audio-V4-ndk'),
    (
        'vendor/lib64/c2.dolby.hevc.dec.so',
        'vendor/lib64/c2.dolby.hevc.sec.dec.so',
    ): blob_fixup()
        .add_needed('libcodec2_shim.so'),
    (
        'vendor/lib64/camera/components/com.qti.node.dewarp.so',
        'vendor/lib64/hw/com.qti.chi.override.so',
        'vendor/lib64/libcamximageformatutils.so',
        'vendor/lib64/libchifeature2.so',
        'vendor/lib64/vendor.qti.hardware.camera.offlinecamera-service-impl.so',
    ): prebuilt('android.hardware.graphics.allocator-V1-ndk'),
    'vendor/lib64/hw/android.hardware.bluetooth.audio_sw.so': prebuilt(
        'android.hardware.audio.core-V2-ndk',
        'android.hardware.bluetooth.audio-V4-ndk',
        'android.media.audio.common.types-V3-ndk',
        'libaudioutils',
    ),
    'vendor/lib64/hw/audio.bluetooth_qti.default.so': prebuilt(
        'android.hardware.bluetooth.audio-V4-ndk',
        'libaudioutils',
    ),
    (
        'vendor/lib64/hw/libaudiocorehal.default.so',
        'vendor/lib64/hw/libaudiocorehal.qti.so',
    ): prebuilt(
        'android.hardware.audio.core-V2-ndk',
        'android.media.audio.common.types-V3-ndk',
        'libaudioutils',
    ),
    (
        'vendor/lib64/hw/libaudioeffecthal.qti.so',
        'vendor/lib64/soundfx/libquasar.so',
    ): prebuilt(
        'android.media.audio.common.types-V3-ndk',
        'libtinyxml2',
    ),
    (
        'vendor/lib64/libVoiceSdk.so',
        'vendor/lib64/libcapiv2uvvendor.so',
        'vendor/lib64/liblistensoundmodel2vendor.so',
    ): prebuilt('libtensorflowlite_c'),
    'vendor/lib64/libarcsoft_video_superportrait.so': blob_fixup()
        .clear_symbol_version('AHardwareBuffer_allocate')
        .clear_symbol_version('AHardwareBuffer_describe')
        .clear_symbol_version('AHardwareBuffer_lockPlanes')
        .clear_symbol_version('AHardwareBuffer_release')
        .clear_symbol_version('AHardwareBuffer_unlock'),
    'vendor/lib64/soundfx/libhapticgenerator.so': prebuilt('libvibratorutils'),
    'vendor/lib64/libaudioplatformconverter.qti.so': prebuilt(
        'android.hardware.audio.core-V2-ndk',
    ),
    'vendor/lib64/libaudioserviceexampleimpl.so': prebuilt(
        'android.hardware.audio.core-V2-ndk',
        'android.hardware.bluetooth.audio-V4-ndk',
        'libaudioutils',
    ).add_needed('libbluetooth_audio_session_aidl_shim.so'),
    (
        'vendor/lib64/libbluetooth_audio_session_qti.so',
        'vendor/lib64/libbluetooth_audio_session_qti_2_1.so',
    ): prebuilt('libaudioutils'),
    'vendor/lib64/libqcodec2_core.so': prebuilt(
        'android.hardware.graphics.common-V5-ndk',
    ),
    'vendor/lib64/libqtigefar.so': blob_fixup()
        .remove_needed('android.hardware.audio.core-V2-ndk.so')
        .remove_needed('android.media.audio.common.types-V3-ndk.so'),
    'vendor/lib64/libqvrservice.so': prebuilt(
        'android.hardware.audio.core-V2-ndk',
        'android.hardware.graphics.allocator-V1-ndk',
        'android.media.audio.common.types-V3-ndk',
        'vendor.qti.hardware.display.allocator@4.0',
    ),
    'vendor/lib64/libsxrservice.so': prebuilt(
        'android.hardware.audio.core-V2-ndk',
        'android.hardware.common-V2-ndk_platform',
        'android.media.audio.common.types-V3-ndk',
    ),
    'vendor/lib64/libwfdmmsrc_proprietary.so': prebuilt(
        'android.hardware.audio.core-V2-ndk',
    ).remove_needed('android.media.audio.common.types-V2-ndk.so'),
    (
        'vendor/lib64/soundfx/libswdapaidl.so',
        'vendor/lib64/soundfx/libswgamedapaidl.so',
    ): prebuilt('android.media.audio.common.types-V3-ndk'),
    'vendor/lib64/android.hardware.graphics.composer3-V3-ndk_prebuilt.so': prebuilt(
        'android.hardware.graphics.common-V5-ndk',
    ),
    'vendor/lib64/vendor.qti.hardware.display.composer3-V1-ndk_prebuilt.so': prebuilt(
        'android.hardware.graphics.composer3-V3-ndk',
    ),
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
