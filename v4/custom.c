#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"
#include "tapdance.h"
#include "achordion.h"

enum layers {
    _COLEMK,
    _QWERTY,
    _WINNAV,
    _VINAV,
    _NUM,
    _SYM,
    _MOUSE,
    _CONF,
    _SELECT
};

#define L_MODL MO(_WINNAV)
#define R_MODL MO(_VINAV)

#define R_SYML MO(_SYM) //Toggle symbols on left hand
#define NAV MO(_MOUSE) //Toggle mouse layer for left
#define L_SYML MO(_SYM) //Toggle symbols on right hand
#define L_NUMP MO(_NUM) //Toggle num pad on right hand
enum combo_events {
  L_CTRL,
  R_CTRL,
  L_ALT,
  R_ALT,
  L_GUI,
  R_GUI,
  L_SYM,
  R_SYM,
  COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH;

const uint16_t PROGMEM cmb_F_T[] = {KC_F, KC_T, COMBO_END};
const uint16_t PROGMEM cmb_U_N[] = {KC_U, KC_N, COMBO_END};

const uint16_t PROGMEM cmb_S_D[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM cmb_E_H[] = {KC_E, KC_H, COMBO_END};

const uint16_t PROGMEM cmb_R_D[] = {KC_R, KC_D, COMBO_END};
const uint16_t PROGMEM cmb_I_H[] = {KC_I, KC_H, COMBO_END};

const uint16_t PROGMEM cmb_W_T[] = {KC_W, KC_T, COMBO_END};
const uint16_t PROGMEM cmb_Y_N[] = {KC_Y, KC_N, COMBO_END};

const uint16_t PROGMEM cmb_P_G[] = {KC_P, KC_G, COMBO_END};
const uint16_t PROGMEM cmb_L_M[] = {KC_L, KC_M, COMBO_END};

combo_t key_combos[COMBO_LENGTH] = {
  COMBO(cmb_F_T, L_SYML),
  COMBO(cmb_U_N, R_SYML),
  COMBO(cmb_S_D, L_NUMP),
  COMBO(cmb_E_H, L_NUMP),
  
  COMBO(cmb_R_D, KC_LALT),
  COMBO(cmb_I_H, KC_LALT),

  COMBO(cmb_W_T, KC_LGUI),
  COMBO(cmb_Y_N, KC_LGUI),
};

/* OVERRIDES */

///const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// This globally defines all key overrides to be used
//const key_override_t **key_overrides = (const key_override_t *[]){
//    &delete_key_override,
//    NULL // Null terminate the array of overrides!
//};


bool is_alt_tab_active = false;
bool is_ctl_tab_active = false;
enum custom_keycodes {          // Make sure have the awesome keycode ready
  ALT_TAB = SAFE_RANGE,
  CTL_TAB, 
// Custom oneshot mod implementation with no timers.
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
    VIUP,
    VIDOWN,
    VILEFT,
    VIRIGHT,
    VTOGGLE
};

// Tap dance enums
enum {
    L_LAYER_DANCE,
    SOME_OTHER_DANCE
};

void handle_vikey(
    uint16_t keycode,
    bool vi_active,
    uint16_t trigger,
    uint16_t nKey,
    uint16_t vKey,
    keyrecord_t *record
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            if (vi_active) {
                register_code(nKey);
            } else {
                register_code(vKey);
            }
        } else {
            if (vi_active) {
                unregister_code(nKey);
            } else {
                unregister_code(vKey);
            }
        }
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_alt_tab_active) {
        unregister_code(KC_LALT);
        is_alt_tab_active = false;
    }
    if (is_ctl_tab_active) {
        unregister_code(KC_LCTL);
        is_ctl_tab_active = false;
    }
    return update_tri_layer_state(state, _WINNAV, _VINAV, _CONF);
    return state;
}

bool vi_mode_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_achordion(keycode, record)) { return false; }
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
        &os_gui_state, KC_LEFT_GUI, OS_GUI,
        keycode, record
    );
    handle_vikey(keycode, vi_mode_active, VIUP, KC_UP, KC_K, record);
    handle_vikey(keycode, vi_mode_active, VIDOWN, KC_DOWN, KC_J, record);
    handle_vikey(keycode, vi_mode_active, VILEFT, KC_LEFT, KC_H, record);
    handle_vikey(keycode, vi_mode_active, VIRIGHT, KC_RIGHT, KC_L, record);
  
    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
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
        case VTOGGLE:
            if (record->event.pressed) {
                vi_mode_active = !vi_mode_active;
            }
            break;
     }
}



bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case KC_ESC:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case L_MODL:
    case R_MODL:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
    case L_SYML:
    case L_NUMP:
    case TD(L_LAYER_DANCE):
        return true;
    default:
        return false;
    }
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t lltap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void ll_finished(qk_tap_dance_state_t *state, void *user_data) {
    lltap_state.state = hold_me(state);
    switch (lltap_state.state) {
        case TD_SINGLE_HOLD: layer_on(_WINNAV); break;
        case TD_DOUBLE_HOLD: layer_on(_SELECT); break;
        default:
            break;
    }
}

void ll_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (lltap_state.state) {
        case TD_SINGLE_HOLD: layer_off(_WINNAV); break;
        case TD_DOUBLE_HOLD: layer_off(_SELECT); break;
        default:
            break;
    }
    lltap_state.state = TD_NONE;
}

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [L_LAYER_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED_TIME(NULL, ll_finished, ll_reset, 275)
};

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(L_LAYER_DANCE):
            // Immediately select the hold action when another key is tapped.
            return true;
        default:
            // Do not select the hold action when another key is tapped.
            return false;
    }
}

void matrix_scan_user(void) {
  achordion_task();
}