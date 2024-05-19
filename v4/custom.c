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

uint16_t pwtab_key_registered = 0;
bool is_alt_tab_active = false;
bool is_ctl_tab_active = false;

enum {
    Windows,
    MacOs,
    Mcw_state_count
} current_os = MacOs;

enum custom_keycodes {          // Make sure have the awesome keycode ready
  ALT_TAB = SAFE_RANGE,
  SFT_ALT_TAB,
  ATAB_LEFT,
  ATAB_RIGHT,
  ATAB_UP,
  ATAB_DOWN,
  CTAB_LEFT,
  CTAB_RIGHT,
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
    MC_LOCKSCREEN,
    MC_WINDOWMAX,
    MC_WINDOWMIN,
    MC_WINDOWLEFT,
    MC_WINDOWRIGHT,
    MC_WINDOWCLOSE,
    MC_STOPDEF //this is the end of the list of MC keycodes, it also prints the current value of the toggle with a send string.
};
//KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                                   KC_NO, KC_NO, LSFT(KC_RBRC), KC_NO, KC_NO, KC_NO, 
//KC_TRNS, KC_NO, LSFT(KC_3), LSFT(KC_5), RALT(KC_7), RALT(KC_0),               KC_NUBS, LSFT(KC_NUBS), LSFT(KC_0), KC_PPLS, KC_PAST, KC_NO, 
//KC_TRNS, RALT(KC_2), RALT(KC_4), LSFT(KC_2), LSFT(KC_8), LSFT(KC_9),          RALT(KC_MINS), LSFT(KC_7), LSFT(KC_MINS), LSFT(KC_1), RALT(KC_RBRC), KC_NO, 
//KC_TRNS, KC_NO, RALT(KC_NUBS), LSFT(KC_6), RALT(KC_8), RALT(KC_9), KC_TRNS,   KC_TRNS, LSFT(KC_EQL), KC_EQL, LSFT(KC_5), LSFT(KC_NUHS), KC_NO, KC_TRNS, 
//KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
// Cmd = GUI, 
uint16_t mcwin_keys[MC_STOPDEF-MC_TOGGLE][Mcw_state_count] = {
    [MC_BSLSH-MC_TOGGLE-1] = {RALT(KC_MINS), RSFT(RALT(KC_7))},
    [MC_LCURL-MC_TOGGLE-1] = {RALT(KC_7), RSFT(RALT(KC_8))},
    [MC_RCURL-MC_TOGGLE-1] = {RALT(KC_0), RSFT(RALT(KC_9))},
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
    [MC_LOCKSCREEN-MC_TOGGLE-1] = {LGUI(KC_L), LCTL(LGUI(KC_Q))}, //Lock screen GUI+L on Win GUI+CTL+Q mac
    [MC_WINDOWMAX-MC_TOGGLE-1] = {LGUI(KC_UP), LCTL(LGUI(KC_Z))}, //Maximize window GUI+UP on Win GUI+CTL+Z mac
    [MC_WINDOWMIN-MC_TOGGLE-1] = {LGUI(KC_DOWN), LGUI(KC_M)}, //Close window GUI+down on Win GUI+M mac
    [MC_WINDOWLEFT-MC_TOGGLE-1] = {LGUI(KC_LEFT), RALT(LCTL(LGUI(KC_L)))}, //Move window left GUI+Left on Win custom GUI+CTL+ALT+L mac
    [MC_WINDOWRIGHT-MC_TOGGLE-1] = {LGUI(KC_RIGHT), RALT(LCTL(LGUI(KC_R)))}, //Move window right GUI+R on Win custom GUI+CTL+ALT+R mac
    [MC_WINDOWCLOSE-MC_TOGGLE-1] = {LGUI(KC_W), LGUI(KC_W)}, //Close window GUI+W on Win GUI+W mac
    [MC_APPTABMOD-MC_TOGGLE-1] = {KC_LALT, KC_LGUI} //key used as a modifier to have tab act as application switcher
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
    pwtab_key_registered = 0;
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

    return true;
}

void send_tab_key(uint16_t key) 
{
    register_code16(key);
    pwtab_key_registered = key;//We need to unregister this keypress again later.
}

void handle_alttab_press(uint16_t keycode) 
{
    if (!is_alt_tab_active && !(keycode == ATAB_LEFT || keycode == ATAB_RIGHT)) {//if this is the first press of the alt tab(app switch)
        is_alt_tab_active = true;
        register_code16(getMcWinKey(MC_APPTABMOD));// activate the relevant modifier key for the os
        send_tab_key(KC_TAB);// press tab once to open the menu for the OS.
        return;
    }
    if (keycode == ALT_TAB) 
        send_tab_key(KC_TAB);
    else if (keycode == SFT_ALT_TAB) 
        send_tab_key(KC_LSFT);
    else if (keycode == ATAB_LEFT) // We are going to hijack this button for window tap
        if (is_alt_tab_active)
            send_tab_key(KC_LEFT);
        else 
            send_tab_key(LGUI(KC_NUBS));
    else if (keycode == ATAB_RIGHT)
        if (is_alt_tab_active)
            send_tab_key(KC_RIGHT);
        else 
            send_tab_key(LSFT(LGUI(KC_NUBS)));
    else if (keycode == ATAB_DOWN)
        send_tab_key(KC_DOWN);
    else if (keycode == ATAB_UP)
        send_tab_key(KC_UP);
}

void handle_ctltab_press(uint16_t keycode) 
{
    if (!is_ctl_tab_active) {//if this is the first press of the alt tab(app switch)
        is_ctl_tab_active = true;
        register_code(KC_LCTL);// activate the relevant modifier key for the os
    }
    if (keycode == CTAB_RIGHT)
        send_tab_key(KC_TAB);
    else if (keycode == CTAB_LEFT)
        send_tab_key(LSFT(KC_TAB));
}

void handle_app_switching(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed)
    {
        if (keycode >= ALT_TAB && keycode <= ATAB_DOWN)
            handle_alttab_press(keycode);
        else if (keycode == CTAB_LEFT || keycode == CTAB_RIGHT)
            handle_ctltab_press(keycode);
    }
    else if (pwtab_key_registered != 0)
    {
        unregister_code16(pwtab_key_registered);
        pwtab_key_registered = 0;
    }
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    handle_app_switching(keycode, record);
    switch (keycode){ 
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

void ll_finished(tap_dance_state_t *state, void *user_data) {
    lltap_state.state = hold_me(state);
    switch (lltap_state.state) {
        case TD_SINGLE_HOLD: layer_on(_WINNAV); break;
        case TD_DOUBLE_HOLD: layer_on(_SELECT); break;
        default:
            break;
    }
}

void ll_reset(tap_dance_state_t *state, void *user_data) {
    switch (lltap_state.state) {
        case TD_SINGLE_HOLD: layer_off(_WINNAV); break;
        case TD_DOUBLE_HOLD: layer_off(_SELECT); break;
        default:
            break;
    }
    lltap_state.state = TD_NONE;
}

// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
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
