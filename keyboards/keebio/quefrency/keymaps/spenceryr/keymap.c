#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum KEYMAP_LAYERS {
    _KEYMAP_LAYER_BASE,
    _KEYMAP_LAYER_FN1
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_KEYMAP_LAYER_BASE] = LAYOUT_65_with_macro(
    KC_F1,   KC_F2,   KC_ESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC, KC_AUDIO_MUTE, \
    KC_F3,   KC_F4,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, \
    KC_F5,   KC_F6,   KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_PGUP, \
    KC_F7,   KC_F8,   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN, \
    KC_F9,   KC_F10,  KC_LCTL, MO(_KEYMAP_LAYER_FN1), KC_LGUI, KC_LALT, KC_SPC,           MO(_KEYMAP_LAYER_FN1),KC_SPC,  KC_RALT, KC_RCTL, KC_RGUI, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [_KEYMAP_LAYER_FN1] = LAYOUT_65_with_macro(
    _______, _______, KC_TILD, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,  _______, _______, \
    _______, _______, RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______
  )
};

const key_override_t mute_key_override = ko_make_basic(MOD_MASK_CTRL, KC_AUDIO_MUTE, KC_MEDIA_PLAY_PAUSE);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &mute_key_override,
    NULL // Null terminate the array of overrides!
};

#ifdef RGBLIGHT_ENABLE
#ifdef RGBLIGHT_LAYERS

enum RGB_LAYERS {
    _RGB_LAYER_CAPS_LOCK,
    _RGB_LAYER_FN1
};

const rgblight_segment_t PROGMEM _rgb_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, HSV_WHITE},
    {14, 2, HSV_WHITE}
);

const rgblight_segment_t PROGMEM _rgb_fn1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {8, 8, 106, 255, 128}
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM _rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    _rgb_capslock_layer,
    _rgb_fn1_layer
);

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(_RGB_LAYER_CAPS_LOCK, led_state.caps_lock);
    return true;
}

#endif // RGBLIGHT_ENABLE
#endif // RGBLIGHT_LAYERS

layer_state_t layer_state_set_user(layer_state_t state) {
    #ifdef RGBLIGHT_ENABLE
    #ifdef RGBLIGHT_LAYERS
    rgblight_set_layer_state(_RGB_LAYER_FN1, layer_state_cmp(state, _KEYMAP_LAYER_FN1));
    #endif // RGBLIGHT_ENABLE
    #endif // RGBLIGHT_LAYERS
    return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    else if (index == 1) {
        if (clockwise) {
            if (keyboard_report->mods & MOD_BIT(KC_LCTL)) {
                tap_code(KC_MEDIA_NEXT_TRACK);
            } else {
                tap_code(KC_VOLU);
            }
        } else {
            if (keyboard_report->mods & MOD_BIT(KC_LCTL)) {
                tap_code(KC_MEDIA_PREV_TRACK);
            } else {
                tap_code(KC_VOLD);
            }
        }
    }
    return true;
}

void eeconfig_init_user(void) {
#ifdef BACKLIGHT_ENABLE
    backlight_enable();
    backlight_level(3);
#endif // BACKLIGHT_ENABLE
#ifdef RGBLIGHT_ENABLE
    rgblight_enable(); // Enable RGB by default
    rgblight_sethsv(106, 255, 255);
#ifdef RGBLIGHT_ANIMATIONS
    rgblight_mode(RGBLIGHT_MODE_SNAKE);
#endif // RGBLIGHT_ANIMATIONS
#endif // RGBLIGHT_ENABLE
}

void keyboard_post_init_user(void) {
    // Enable the LED layers
    #ifdef RGBLIGHT_ENABLE
    #ifdef RGBLIGHT_LAYERS
    rgblight_layers = _rgb_layers;
    #endif // RGBLIGHT_LAYERS

    #endif // RGBLIGHT_ENABLE
}
