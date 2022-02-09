#include QMK_KEYBOARD_H

#include "oneshot.h"

#define L_LEFT MO(1)
#define L_RIGHT MO(2)
/* OVERRIDES */

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &delete_key_override,
    NULL // Null terminate the array of overrides!
};


bool is_alt_tab_active = false;
bool is_ctl_tab_active = false;
enum custom_keycodes {          // Make sure have the awesome keycode ready
  ALT_TAB = SAFE_RANGE,
  CTL_TAB, 
// Custom oneshot mod implementation with no timers.
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_CMD
};

layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_alt_tab_active) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }
    if (is_ctl_tab_active) {
        unregister_code(KC_LCTL);
        is_ctl_tab_active = false;
    }
    return state;
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );
  switch (keycode){
        case ALT_TAB: // super alt tab macro
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                if (get_mods() & MOD_MASK_SHIFT)
                  register_code16(LSFT(KC_TAB));
                else
                  register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
        case CTL_TAB: // super alt tab macro
            if (record->event.pressed) {
                if (!is_ctl_tab_active) {
                    is_ctl_tab_active = true;
                    register_code(KC_LCTL);
                }
                if (get_mods() & MOD_MASK_SHIFT)
                  register_code16(LSFT(KC_TAB));
                else
                  register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
        return false;
     }
  return true;
}


bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case L_LEFT:
    case L_RIGHT:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case L_LEFT:
    case L_RIGHT:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}