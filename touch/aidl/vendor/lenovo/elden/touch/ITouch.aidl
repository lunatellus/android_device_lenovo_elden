/*
 * SPDX-FileCopyrightText: The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

package vendor.lenovo.elden.touch;

@VintfStability
interface ITouch {
    void setTapSensitivity(int level);
    void setSwipeResponsiveness(int level);
    void setPenMode(boolean enabled);
    void setPanelDirection(int rotation);
    void setCornerInhibition(int mode);
}
