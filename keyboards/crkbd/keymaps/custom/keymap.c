// Custom Corne V4.1 keymap with Colemak base
// Features: Custom thumb clusters, A-layer, Nav layer with numpad, Symbol layer

#include QMK_KEYBOARD_H

// Layer definitions
enum layers {
    _COLEMAK = 0,
    _A_LAYER,    // A-held layer with backspace on N
    _NAV,        // Navigation + numpad
    _SYMBOL,     // Symbols
    _FN,         // Function keys
};

// Custom keycodes
enum custom_keycodes {
    REPEAT = SAFE_RANGE,
};

// Home row mods
#define HOME_R LALT_T(KC_R)
#define HOME_S LCTL_T(KC_S)
#define HOME_T LSFT_T(KC_T)
#define HOME_N RSFT_T(KC_N)
#define HOME_E RCTL_T(KC_E)
#define HOME_I RALT_T(KC_I)

// Dual function keys
#define CMD_V LGUI_T(KC_V)
#define CMD_M RGUI_T(KC_M)
#define A_LAYER LT(_A_LAYER, KC_A)

// Thumb cluster
#define ESC_CMD LGUI_T(KC_ESC)
#define TAB_SFT LSFT_T(KC_TAB)
#define RPT_SYM LT(_SYMBOL, REPEAT)  // Repeat on tap, Symbol on hold
#define ENT_CMD RGUI_T(KC_ENT)
#define SPC_FN LT(_FN, KC_SPC)
#define MIN_NAV LT(_NAV, KC_MINS)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_COLEMAK] = LAYOUT_split_3x6_3(
        // Left half
        KC_NO,   KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,
        KC_NO,   A_LAYER, HOME_R,  HOME_S,  HOME_T,  KC_D,
        KC_NO,   KC_Z,    KC_X,    KC_C,    CMD_V,   KC_B,
                          ESC_CMD, TAB_SFT, RPT_SYM,
        
        // Right half
        KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_NO,
        KC_H,    HOME_N,  HOME_E,  HOME_I,  KC_O,    KC_NO,
        KC_K,    CMD_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_NO,
        ENT_CMD, SPC_FN,  MIN_NAV
    ),
    
    [_A_LAYER] = LAYOUT_split_3x6_3(
        // Left half - mostly transparent
        KC_NO,   _______, _______, _______, _______, _______,
        KC_NO,   _______, _______, _______, _______, _______,
        KC_NO,   _______, _______, _______, _______, _______,
                          _______, _______, _______,
        
        // Right half - N position is backspace
        _______, _______, _______, _______, _______, KC_NO,
        _______, KC_BSPC, _______, _______, _______, KC_NO,
        _______, _______, _______, _______, _______, KC_NO,
        _______, _______, _______
    ),
    
    [_NAV] = LAYOUT_split_3x6_3(
        // Left half - Numpad
        KC_NO,   KC_0,    KC_1,    KC_2,    KC_3,    _______,
        KC_NO,   KC_4,    KC_5,    KC_6,    KC_7,    _______,
        KC_NO,   KC_8,    KC_9,    _______, _______, _______,
                          _______, _______, _______,
        
        // Right half - Navigation
        _______, KC_HOME, KC_UP,   KC_END,  KC_PGUP, KC_NO,
        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PGDN, KC_NO,
        _______, _______, _______, _______, _______, KC_NO,
        _______, _______, _______
    ),
    
    [_SYMBOL] = LAYOUT_split_3x6_3(
        // Left half - mostly transparent
        KC_NO,   _______, _______, _______, _______, _______,
        KC_NO,   _______, _______, _______, _______, _______,
        KC_NO,   _______, _______, _______, _______, _______,
                          _______, _______, _______,
        
        // Right half - Symbols
        KC_MINS, KC_LPRN, KC_LCBR, KC_RCBR, KC_EQL,  KC_NO,
        KC_QUOT, _______, _______, KC_BSLS, _______, KC_NO,
        KC_RPRN, KC_GRV,  KC_LBRC, KC_RBRC, _______, KC_NO,
        _______, _______, _______
    ),
    
    [_FN] = LAYOUT_split_3x6_3(
        // Left half - F1-F12
        KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,
        KC_NO,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
        KC_NO,   KC_F11,  KC_F12,  _______, _______, _______,
                          _______, _______, _______,
        
        // Right half - Media controls
        KC_MPRV, KC_MNXT, KC_VOLU, KC_VOLD, KC_MUTE, KC_NO,
        _______, _______, _______, _______, _______, KC_NO,
        _______, _______, _______, _______, _______, KC_NO,
        _______, _______, _______
    )
};

// Repeat key implementation
uint16_t last_keycode = KC_NO;
uint8_t last_mods = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case REPEAT:
            if (record->event.pressed) {
                if (last_keycode != KC_NO) {
                    register_mods(last_mods);
                    register_code(last_keycode);
                }
            } else {
                if (last_keycode != KC_NO) {
                    unregister_code(last_keycode);
                    unregister_mods(last_mods);
                }
            }
            return false;
        default:
            // Store last keycode for repeat
            if (record->event.pressed && keycode < 256) {
                last_keycode = keycode;
                last_mods = get_mods();
            }
            return true;
    }
}

// Home row mod configuration
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HOME_R:
        case HOME_S:
        case HOME_T:
        case HOME_N:
        case HOME_E:
        case HOME_I:
        case CMD_V:
        case CMD_M:
            return 200;  // Adjust this value if needed
        default:
            return TAPPING_TERM;
    }
}

// Enable permissive hold for home row mods
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HOME_R:
        case HOME_S:
        case HOME_T:
        case HOME_N:
        case HOME_E:
        case HOME_I:
        case CMD_V:
        case CMD_M:
        case A_LAYER:
            return true;
        default:
            return false;
    }
}