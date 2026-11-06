/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.elden.settings;

import android.os.Bundle;

import androidx.preference.PreferenceFragmentCompat;
import androidx.preference.PreferenceScreen;
import androidx.recyclerview.widget.RecyclerView;

import com.android.settingslib.collapsingtoolbar.CollapsingToolbarBaseActivity;
import com.android.settingslib.datastore.SettingsSystemStore;
import com.android.settingslib.preference.PreferenceDataStoreAdapter;
import com.android.settingslib.widget.SettingsPreferenceGroupAdapter;

public class TouchSettingsActivity extends CollapsingToolbarBaseActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction()
                    .replace(com.android.settingslib.collapsingtoolbar.R.id.content_frame,
                            new TouchSettingsFragment())
                    .commit();
        }
    }

    public static class TouchSettingsFragment extends PreferenceFragmentCompat {
        @Override
        public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
            getPreferenceManager().setPreferenceDataStore(
                    new PreferenceDataStoreAdapter(SettingsSystemStore.get(requireContext())));
            setPreferencesFromResource(R.xml.touch_settings, null);
        }

        @Override
        protected RecyclerView.Adapter onCreateAdapter(PreferenceScreen preferenceScreen) {
            return new SettingsPreferenceGroupAdapter(preferenceScreen);
        }
    }
}
