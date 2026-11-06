/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package org.lineageos.elden.touch;

import android.app.ActivityManager;
import android.content.ContentResolver;
import android.content.Context;
import android.database.ContentObserver;
import android.net.Uri;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.IServiceCallback;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.UserHandle;
import android.provider.Settings;
import android.util.Slog;
import android.view.Display;
import android.view.IRotationWatcher;
import android.view.WindowManagerGlobal;

import com.android.server.LocalServices;
import com.android.server.SystemService;
import com.android.server.pm.UserManagerInternal;

import java.util.Collection;

import vendor.lenovo.elden.touch.ITouch;

public final class EldenTouchService extends SystemService {
    private static final String TAG = "EldenTouchService";
    private static final String SENSITIVITY = "elden_touch_sensitivity";
    private static final String SWIPE_RESPONSIVENESS = "elden_touch_swipe_responsiveness";
    private static final String PEN_ENABLED = "elden_touch_pen_enabled";
    private static final String CORNER_INHIBITION = "elden_touch_corner_inhibition";
    private static final String[] SETTINGS = {
            SENSITIVITY, SWIPE_RESPONSIVENESS, PEN_ENABLED, CORNER_INHIBITION};

    private final Handler mHandler;
    private final ContentResolver mResolver;
    private ITouch mTouch;
    private int mUserId;
    private int mRotation;

    public EldenTouchService(Context context) {
        super(context);
        mResolver = context.getContentResolver();
        HandlerThread thread = new HandlerThread(TAG);
        thread.start();
        mHandler = new Handler(thread.getLooper());
    }

    @Override
    public void onStart() {}

    @Override
    public void onBootPhase(int phase) {
        if (phase == PHASE_ACTIVITY_MANAGER_READY) {
            mHandler.post(this::initialize);
        }
    }

    @Override
    public void onUserSwitching(TargetUser from, TargetUser to) {
        mHandler.post(() -> {
            setUser(to.getUserIdentifier());
            applySettings();
        });
    }

    private void setUser(int userId) {
        mUserId = LocalServices.getService(UserManagerInternal.class).getProfileParentId(userId);
    }

    private void initialize() {
        setUser(ActivityManager.getCurrentUser());
        ContentObserver observer = new ContentObserver(mHandler) {
            @Override
            public void onChange(boolean selfChange, Collection<Uri> uris, int flags, int userId) {
                if (userId == mUserId) {
                    for (Uri uri : uris) {
                        if (Settings.System.CONTENT_URI.equals(uri)) {
                            applySettings();
                        } else {
                            applySetting(uri.getLastPathSegment());
                        }
                    }
                }
            }
        };
        for (String key : SETTINGS) {
            mResolver.registerContentObserverAsUser(
                    Settings.System.getUriFor(key), false, observer, UserHandle.ALL);
        }
        try {
            mRotation = WindowManagerGlobal.getWindowManagerService().watchRotation(
                    new IRotationWatcher.Stub() {
                        @Override
                        public void onRotationChanged(int rotation) {
                            mHandler.post(() -> {
                                mRotation = rotation;
                                applyRotation();
                            });
                        }
                    }, Display.DEFAULT_DISPLAY);
            ServiceManager.registerForNotifications(ITouch.DESCRIPTOR + "/default",
                    new IServiceCallback.Stub() {
                        @Override
                        public void onRegistration(String name, IBinder binder) {
                            mHandler.post(() -> {
                                mTouch = ITouch.Stub.asInterface(binder);
                                applySettings();
                                applyRotation();
                            });
                        }
                    });
        } catch (RemoteException e) {
            Slog.e(TAG, "Unable to initialize touch control", e);
        }
    }

    private void applySettings() {
        for (String key : SETTINGS) {
            applySetting(key);
        }
    }

    private void applySetting(String key) {
        if (mTouch == null) {
            return;
        }
        try {
            switch (key) {
                case SENSITIVITY:
                    mTouch.setTapSensitivity(Settings.System.getIntForUser(
                            mResolver, SENSITIVITY, 1, mUserId));
                    break;
                case SWIPE_RESPONSIVENESS:
                    mTouch.setSwipeResponsiveness(Settings.System.getIntForUser(
                            mResolver, SWIPE_RESPONSIVENESS, 0, mUserId));
                    break;
                case CORNER_INHIBITION:
                    mTouch.setCornerInhibition(Settings.System.getIntForUser(
                            mResolver, CORNER_INHIBITION, 0, mUserId));
                    break;
                case PEN_ENABLED:
                    mTouch.setPenMode(Settings.System.getIntForUser(
                            mResolver, PEN_ENABLED, 0, mUserId) == 1);
                    break;
            }
        } catch (RemoteException | IllegalArgumentException | IllegalStateException e) {
            Slog.e(TAG, "Unable to apply " + key + " for user " + mUserId, e);
        }
    }

    private void applyRotation() {
        if (mTouch == null) {
            return;
        }
        try {
            mTouch.setPanelDirection(mRotation);
        } catch (RemoteException | IllegalArgumentException | IllegalStateException e) {
            Slog.e(TAG, "Unable to apply touch rotation " + mRotation, e);
        }
    }
}
