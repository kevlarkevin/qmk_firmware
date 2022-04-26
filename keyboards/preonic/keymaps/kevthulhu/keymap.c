/* Copyright 2021 Kevin Sangwoo Hahn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "muse.h"

enum preonic_layers {
    _BASE,
    _GAME,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum preonic_keycodes {
    BASE = SAFE_RANGE,
    GAME,
    LOWER,
    RAISE,
    USSR,  // ☭
    BACKLIT
};


// Custom macros
#define CTL_ESC     CTL_T(KC_ESC)               // Tap for Esc, hold for Ctrl
#define SFT_ENT     SFT_T(KC_ENT)               // Tap for Enter, hold for Shift
#define HPR_TAB     ALL_T(KC_TAB)               // Tap for Tab, hold for Hyper (Super+Ctrl+Alt+Shift)
#define MEH_GRV     MEH_T(KC_GRV)               // Tap for Backtick, hold for Meh (Ctrl+Alt+Shift)


//aliases for clarity in layering
#define A_BSPC  LALT(KC_BSPC) // delete whole word in Mac
#define C_BSPC  LCTL(KC_BSPC) // delete whole word in PC
#define A_LEFT  LALT(KC_LEFT) // move cursor whole word back in Mac
#define A_RGHT  LALT(KC_RGHT) // move cursor whole word forward in Mac
#define C_RGHT  LCTL(KC_RGHT) // move cursor whole word back in PC
#define C_LEFT  LCTL(KC_LEFT) // move cursor whole word forward in PC
#define SftEnt  SFT_T(KC_ENT)
#define BS_GUI  LGUI_T(KC_BSPC)
#define BS_CTL  LCTL_T(KC_BSPC)
#define RECT    LCA_T(KC_DEL) // Ctrl+Alt is the basis for Rectangle WM

// internet browser tab shortcuts and window/application swapping for Mac and Win
#define GSL     LGUI(S(KC_LEFT)) // back one tab in Safari
#define GSR     LGUI(S(KC_RGHT)) // forward one tab in Safari
#define CTLPGUP LCTL(KC_PGUP)   // back one tab on PC
#define CTLPGDN LCTL(KC_PGDN)   // forward one tab on PC
#define G_TAB   LGUI(KC_TAB)    // Mac: switch applications
#define G_GRV   LGUI(KC_GRV)    // Mac: switch between windows within an application
#define A_TAB   LALT(KC_TAB)
#define C_TAB   LCTL(KC_TAB)

// Defines for Mac specific key accelerators used in map. Some of these are very awkward to type.
// These are the default bindings for Mac, some of which may be turned off or changed, check your
// settings in System Preferences
#define LS_TAB LSFT(KC_TAB) // Shift-TAB
#define MF_MENU LCTL(KC_F2) // Move focus to menu bar default keybinding in MacOS
#define MF_DOCK LCTL(KC_F3) // Move focus to dock
#define MF_TOOL LCTL(KC_F5) // Move focus to window toolbar
#define MF_STAT LCTL(KC_F8) // Move focus to status bar side of menu bar
#define MF_DRWR LALT(LGUI(KC_QUOTE)) // Move focus to window drawer
#define MF_LPAD LALT(LGUI(KC_Q)) // Show Launchpad
#define MF_NOTI LALT(LGUI(LCTL(KC_B))) // Show Notification Center
#define MF_DASH LALT(LGUI(LCTL(KC_C))) // Show Dashboard
#define MF_HELP LSFT(LGUI(KC_SLASH)) // Position in menu Help search box

// navigation layers for both Mac OS X and Windows
#define NAVMAC  LT(5, KC_ESC)
#define NAVPC   LT(6, KC_ESC)
#define NAVQUD  LT(7, KC_ESC)


#ifdef UNICODEMAP_ENABLE        // For Unicode characters larger than 0x8000. Send with X(<unicode>)
    enum Ext_Unicode{
        PENGUIN = 0,
        BOAR,
        MONKEY,
        DRAGON,
        CHICK,
        TUMBLER
    };
    const uint32_t PROGMEM unicode_map[] = {
        [PENGUIN]   = 0x1F427,
        [BOAR]      = 0x1F417,
        [MONKEY]    = 0x1F412,
        [DRAGON]    = 0x1F409,
        [CHICK]     = 0x1F425,
        [TUMBLER]   = 0x1F943
    };
    #define PENGY   X(PENGUIN)
    #define BOARY   X(BOAR)
    #define MNKY    X(MONKEY)
    #define DRGN    X(DRAGON)
    #define DUCK    X(CHICK)
    #define TMBL    X(TUMBLER)
#endif

// songs; https://github.com/qmk/qmk_firmware/blob/master/quantum/audio/song_list.h
#ifdef AUDIO_ENABLE
    float lower_tone[][2] = SONG(PLOVER_SOUND);
    float raise_tone[][2] = SONG(PLOVER_GOODBYE_SOUND);
    float gaming[][2] = SONG(WORKMAN_SOUND);
    float ussr[][2] = SONG(USSR_ANTHEM);
    float adjust[][2] = SONG(NO_SOUND);
    float my_song[][2] = SONG(NO_SOUND);
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* BASE
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | | \  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |MT(RSft,Enter)|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Ctrl | Alt  | CMD  |Lower | Bksp | Space|Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_BASE] = LAYOUT_preonic_grid(
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
  KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, MT(MOD_RSFT, KC_ENT),
  _______, KC_LCTL, KC_LALT, KC_LCMD, LOWER,   KC_BSPC, KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

/* Game
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | | \  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | Ctrl | Alt  | CMD  |Space |Space | Space| Base | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_GAME] = LAYOUT_preonic_grid(
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
  KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
  KC_LCTL, KC_LCTL, KC_LALT, KC_LCMD, KC_SPC,  KC_SPC,  KC_SPC,  BASE,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),


/* Lower: Symbols
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |   {  |   }  |  \   |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | | Home | End  |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |   _  |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_preonic_grid(
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LCBR, KC_RCBR, KC_BSLS,
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_BSLS,
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,S(KC_NUHS),S(KC_NUBS),KC_HOME,KC_END, _______,
  _______, _______, _______, _______, _______, _______, KC_UNDS, _______, _______, _______, _______, _______
),

/* Raise: Number keys, media, navigation
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO # |ISO / | Pg Up| Pg Dn| USSR |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Play |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_preonic_grid(
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
  KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, KC_PGUP, KC_PGDN, USSR,
  KC_MPLY, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC__VOLDOWN,KC__VOLUP,KC_MPLY
),

// move function keys here
/* Adjust (Lower + Raise): Function keys, RGB
 * ,-----------------------------------------------------------------------------------.
 * |      |brite+|brite-|allwin|      |      |      | Prev | Play | Next | mute |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Reset| Debug|      |      |      |      |TermOf|TermOn|      |      |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Base |      |Aud cy|Aud on|AudOff|AGnorm|AGswap| Base | Game |      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|MusOff|MidiOn|MidOff|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |Backlt|      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_preonic_grid(
  XXXXXXX, KC_BRMU, KC_BRMD, C(KC_UP),XXXXXXX, XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, KC__MUTE,XXXXXXX,
  XXXXXXX, RESET,   DEBUG,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TERM_ON, TERM_OFF,XXXXXXX, XXXXXXX, XXXXXXX,
  BASE,    XXXXXXX, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, BASE,    GAME,    XXXXXXX ,XXXXXXX, XXXXXXX,
  XXXXXXX, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  BACKLIT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
)


};

/*
  Preonic LEDs visible from the top:
    6 5 4 3
    7 8 1 2

  The two numbers: starting LED number, number of LEDs
*/
// define color overrides for the layers
const rgblight_segment_t PROGMEM base_layer[] = RGBLIGHT_LAYER_SEGMENTS({1, 8, 0, 0, 0});
const rgblight_segment_t PROGMEM game_layer[] = RGBLIGHT_LAYER_SEGMENTS({1, 8, HSV_CYAN});
const rgblight_segment_t PROGMEM lower_layer[] = RGBLIGHT_LAYER_SEGMENTS({1, 8, HSV_BLUE});
const rgblight_segment_t PROGMEM raise_layer[] = RGBLIGHT_LAYER_SEGMENTS({1, 8, HSV_RED});
const rgblight_segment_t PROGMEM adjust_layer[] = RGBLIGHT_LAYER_SEGMENTS({1, 8, HSV_PURPLE});


// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    base_layer,
    game_layer,
    lower_layer,
    raise_layer,
    adjust_layer
);

// do a thing on each keystroke
uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    mod_state = get_mods();
    switch (keycode) {
        case BASE:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_BASE);
            }
            return false;
            break;
        case GAME:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_GAME);
            }
            return false;
            break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case USSR:
            if (record->event.pressed) {
                PLAY_SONG (ussr);
            }
            return false;
            break;
        case BACKLIT:
            if (record->event.pressed) {
                register_code(KC_RSFT);
                #ifdef BACKLIGHT_ENABLE
                    backlight_step();
                #endif
                #ifdef RGBLIGHT_ENABLE
                    rgblight_step();
                #endif
                #ifdef __AVR__
                    writePinLow(E6);
                #endif
            } else {
                unregister_code(KC_RSFT);
                #ifdef __AVR__
                    writePinHigh(E6);
                #endif
            }
            return false;
            break;
        case KC_BSPC:
            {
            // Initialize a boolean variable that keeps track of the delete key status: registered or not?
            static bool delkey_registered;
            if (record->event.pressed) {
                // Detect the activation of either shift keys
                if (mod_state & MOD_MASK_SHIFT) {
                    // First temporarily cancel both shifts so that shift isn't applied to the KC_DEL keycode
                    del_mods(MOD_MASK_SHIFT);
                    register_code(KC_DEL);
                    // Update the boolean variable to reflect the status of KC_DEL
                    delkey_registered = true;
                    // Reapplying modifier state so that the held shift key(s)
                    // still work even after having tapped the Backspace/Delete key.
                    set_mods(mod_state);
                    return false;
                }
            } else { // on release of KC_BSPC
                // In case KC_DEL is still being sent even after the release of KC_BSPC
                if (delkey_registered) {
                    unregister_code(KC_DEL);
                    delkey_registered = false;
                    return false;
                }
            }
            // Let QMK process the KC_BSPC keycode as usual outside of shift
            return true;
        }
    }
    return true;
};

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {

    case _GAME:
        PLAY_SONG (gaming);
        break;
    case _RAISE:
        // PLAY_SONG (raise_tone);
        break;
    case _LOWER:
        // PLAY_SONG (lower_tone);
        break;
    case _ADJUST:
        // PLAY_SONG (adjust);
        break;
    default: //  for any other layers, or the default layer
        PLAY_SONG (my_song);
        break;
    }
    rgblight_set_layer_state(0, layer_state_cmp(state, _BASE));
    rgblight_set_layer_state(1, layer_state_cmp(state, _GAME));
    rgblight_set_layer_state(2, layer_state_cmp(state, _LOWER));
    rgblight_set_layer_state(3, layer_state_cmp(state, _RAISE));
    rgblight_set_layer_state(4, layer_state_cmp(state, _ADJUST));

    return state;
}


bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;


bool dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if (active) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            break;
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
    return true;
}


void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
        case RAISE:
        case LOWER:
            return false;
        default:
            return true;
      }
}
