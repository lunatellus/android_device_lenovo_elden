# SPDX-FileCopyrightText: The LineageOS Project
# SPDX-License-Identifier: Apache-2.0

import json
from pathlib import Path
import runpy
import sys
import tempfile
import unittest

DEVICE = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(DEVICE.parents[2] / 'tools/extract-utils'))
fixup_le_audio = runpy.run_path(str(DEVICE / 'extract-files.py'))['fixup_le_audio']


class LeAudioTest(unittest.TestCase):
    def test_conversion_and_reextraction(self):
        for strategy, devices, ases, channels in (
            ('MONO_ONE_CIS_PER_DEVICE', 1, 1, 1),
            ('MONO_ONE_CIS_PER_DEVICE', 2, 2, 1),
            ('STEREO_TWO_CISES_PER_DEVICE', 1, 2, 1),
            ('STEREO_TWO_CISES_PER_DEVICE', 2, 4, 1),
            ('STEREO_ONE_CIS_PER_DEVICE', 1, 1, 2),
            ('STEREO_ONE_CIS_PER_DEVICE', 2, 2, 2),
        ):
            with self.subTest(strategy=strategy, devices=devices), tempfile.TemporaryDirectory() as tmp:
                path = Path(tmp) / 'audio.json'
                row = dict(configuration_strategy=strategy, device_cnt=devices,
                           ase_cnt=ases, codec_id={'coding_format': 6},
                           codec_configuration=[{'type': 3}], direction='SINK')
                original = dict(codec_configurations=[dict(name='codec', subconfigurations=[row])],
                                qos_configurations=[{'name': 'qos'}])
                path.write_text(json.dumps(original))
                fixup_le_audio(None, None, path)
                converted = json.loads(path.read_text())
                rows = converted['codec_configurations'][0]['subconfigurations']
                self.assertEqual(sum(r['ase_cnt'] for r in rows), ases)
                self.assertEqual(converted['qos_configurations'], original['qos_configurations'])
                for result in rows:
                    self.assertEqual(result['ase_channel_cnt'], channels)
                    self.assertEqual(result['codec_configuration'], row['codec_configuration'])
                    self.assertEqual(result['codec_id'], row['codec_id'])
                    self.assertNotIn('device_cnt', result)
                    self.assertNotIn('configuration_strategy', result)
                first = path.read_bytes()
                fixup_le_audio(None, None, path)
                self.assertEqual(path.read_bytes(), first)

    def test_reject_inconsistent_count(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'audio.json'
            data = {'codec_configurations': [{'subconfigurations': [
                {'configuration_strategy': 'STEREO_TWO_CISES_PER_DEVICE',
                 'device_cnt': 2, 'ase_cnt': 1}]}]}
            path.write_text(json.dumps(data))
            original = path.read_bytes()
            with self.assertRaises(ValueError):
                fixup_le_audio(None, None, path)
            self.assertEqual(path.read_bytes(), original)


if __name__ == '__main__':
    unittest.main()
