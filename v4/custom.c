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

#define L_LT_SYML_F LT(_SYM, KC_F)
#define R_LT_SYML_U LT(_SYM, KC_U)
#define L_LT_NUM_G LT(_NUM, KC_G)

#define L_LT_SYML_C LT(_SYM, KC_C)
#define R_LT_SYML_COMM LT(_SYM, KC_COMM)
#define L_LT_NUM_X LT(_NUM, KC_X)

#define L_LT_NAV_ENT LT(_VINAV, KC_ENT)
#define L_LAYER_KEY TD(L_LAYER_DANCE)

#define R_SYML MO(_SYM) //Toggle symbols on left hand
#define NAV MO(_MOUSE) //Toggle mouse layer for left
#define L_SYML MO(_SYM) //Toggle symbols on right hand
#define L_NUMP MO(_NUM) //Toggle num pad on right hand


/* OVERRIDES */

///const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// This globally defines all key overrides to be used
//const key_override_t **key_overrides = (const key_override_t *[]){
//    &delete_key_override,
//    NULL // Null terminate the array of overrides!
//};


bool is_alt_tab_active = false;
bool is_ctl_tab_active = false;

enum {
    Windows,
    MacOs,
    Mcw_state_count
} current_os = Windows;

enum custom_keycodes {          // Make sure have the awesome keycode ready
  ALT_TAB = SAFE_RANGE,
  CTL_TAB, 
  SFT_ALT_TAB,
  SFT_CTL_TAB,
// Custom oneshot mod implementation with no timers.
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
    VIUP,
    VIDOWN,
    VILEFT,
    VIRIGHT,
    VTOGGLE,
    // mcwin
    MC_TOGGLE,
    MC_BSLSH,
    MC_LCURL,
    MC_RCURL,
    MC_PIPE,
    MC_WLEFT,
    MC_WRIGHT,
    MC_HOME,
    MC_END,
    MC_CTLC,
    MC_CTLX,
    MC_CTLV,
    MC_CTLA,
    MC_CTLZ,
    MC_CTLY,
    MC_APPTABMOD,
    MC_SNIP,
    MC_STOPDEF
};
//KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                   KC_NO, KC_NO, LSFT(KC_RBRC), KC_NO, KC_NO, KC_NO, 
//KC_TRNS, KC_NO, LSFT(KC_3), LSFT(KC_5), RALT(KC_7), RALT(KC_0),               KC_NUBS, LSFT(KC_NUBS), LSFT(KC_0), KC_PPLS, KC_PAST, KC_NO, 
//KC_TRNS, RALT(KC_2), RALT(KC_4), LSFT(KC_2), LSFT(KC_8), LSFT(KC_9),          RALT(KC_MINS), LSFT(KC_7), LSFT(KC_MINS), LSFT(KC_1), RALT(KC_RBRC), KC_NO, 
//KC_TRNS, KC_NO, RALT(KC_NUBS), LSFT(KC_6), RALT(KC_8), RALT(KC_9), KC_TRNS,   KC_TRNS, LSFT(KC_EQL), KC_EQL, LSFT(KC_5), LSFT(KC_NUHS), KC_NO, KC_TRNS, 
//KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
// Cmd = GUI, 
uint16_t mcwin_keys[MC_STOPDEF-MC_TOGGLE][Mcw_state_count] = {
    [MC_BSLSH-MC_TOGGLE-1] = {RALT(KC_MINS), LSA(KC_7)},
    [MC_LCURL-MC_TOGGLE-1] = {RALT(KC_7), LSA(KC_8)},
    [MC_RCURL-MC_TOGGLE-1] = {RALT(KC_0), LSA(KC_9)},
    [MC_PIPE-MC_TOGGLE-1] = {RALT(KC_NUBS), RALT(KC_7)},
    [MC_WLEFT-MC_TOGGLE-1] = {LCTL(KC_LEFT), RALT(KC_LEFT)},
    [MC_WRIGHT-MC_TOGGLE-1] = {LCTL(KC_RIGHT), RALT(KC_RIGHT)},
    [MC_HOME-MC_TOGGLE-1] = {KC_HOME, LGUI(KC_LEFT)},
    [MC_END-MC_TOGGLE-1] = {KC_END, LGUI(KC_RIGHT)},
    [MC_CTLC-MC_TOGGLE-1] = {LCTL(KC_C), LGUI(KC_C)},
    [MC_CTLX-MC_TOGGLE-1] = {LCTL(KC_X), LGUI(KC_X)},
    [MC_CTLV-MC_TOGGLE-1] = {LCTL(KC_V), LGUI(KC_V)},
    [MC_CTLA-MC_TOGGLE-1] = {LCTL(KC_A), LGUI(KC_A)},
    [MC_CTLZ-MC_TOGGLE-1] = {LCTL(KC_Z), LGUI(KC_Z)},
    [MC_CTLY-MC_TOGGLE-1] = {LCTL(KC_Y), SGUI(KC_Z)},
    [MC_SNIP-MC_TOGGLE-1] = {SGUI(KC_S), LCTL(LSG(KC_4))}, //Take screen snippet into clipboard. Cmd+Ctrl+Shift+4 on mac 
    [MC_APPTABMOD-MC_TOGGLE-1] = {KC_LALT, KC_LGUI}
};

// Tap dance enums
enum {
    L_LAYER_DANCE,
    SOME_OTHER_DANCE
};

uint16_t getMcWinKey(uint16_t mcKeycode) 
{
	return mcwin_keys[mcKeycode-MC_TOGGLE-1][current_os];
}

void handle_mcwin_key(uint16_t keycode,
    keyrecord_t *record
) {
    if (keycode == MC_TOGGLE && record->event.pressed) {
        if (current_os == Windows)
            current_os = MacOs;
        else
            current_os = Windows;
        return;
    }
    if (keycode == MC_STOPDEF && record->event.pressed) {
        if(current_os == Windows)
            SEND_STRING("Win");
        if(current_os == MacOs)
            SEND_STRING("Mac");
    }
    if (keycode > MC_TOGGLE && keycode < MC_STOPDEF) {
        if (record->event.pressed) {
            register_code16(getMcWinKey(keycode));
        } else {
            unregister_code16(getMcWinKey(keycode));
        }
    }
}

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
        unregister_code16(getMcWinKey(MC_APPTABMOD));
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
    handle_mcwin_key(keycode, record);

    switch (keycode){
        case KC_TAB:
        case LSFT(KC_TAB):
            if (record->event.pressed) {
                if (is_alt_tab_active) {
                    unregister_code16(getMcWinKey(MC_APPTABMOD));
                    is_alt_tab_active = false;
                }
                if (is_ctl_tab_active) {
                    unregister_code16(KC_LCTL);
                    is_ctl_tab_active = false;
                }
            }
            break;
    }
    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode){
        case ALT_TAB: // super alt tab macro
        case SFT_ALT_TAB:
            if (record->event.pressed) {
                if (!is_alt_tab_active) {//if this is the first press of the alt tab(app switch) activate the relevant modifier key for the os
                    is_alt_tab_active = true;
                    register_code16(getMcWinKey(MC_APPTABMOD));
                }
                if ((get_mods() & MOD_MASK_SHIFT) || keycode == SFT_ALT_TAB)
                  register_code16(LSFT(KC_TAB));
                else
                  register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            break;
        case CTL_TAB: // super alt tab macro
        case SFT_CTL_TAB:
            if (record->event.pressed) {
                if (!is_ctl_tab_active) {
                    is_ctl_tab_active = true;
                    register_code(KC_LCTL);
                }
                if ((get_mods() & MOD_MASK_SHIFT) || keycode == SFT_CTL_TAB)
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
    [L_LAYER_DANCE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ll_finished, ll_reset)
};

/// tapping-hold decision stuff

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case L_LT_NAV_ENT:
            return TAPPING_TERM;
        default:
            return TAPPING_TERM;
    }
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(L_LAYER_DANCE):
        case L_LT_NAV_ENT:
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

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
  
  if (other_keycode == LSFT_T(KC_ENT) || other_keycode == RSFT_T(KC_ESC))
    return true;
  if (tap_hold_keycode == L_LT_NAV_ENT)
    return true;

  // Otherwise, follow the opposite hands rule.
  return achordion_opposite_hands(tap_hold_record, other_record);
}

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_QUOT: //å
        case KC_LBRC: //ä
        case KC_SCLN: //ö
        case KC_SLSH: //;
            add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
            return true;

        default:
            return false;  // Deactivate Caps Word.
    }
}
