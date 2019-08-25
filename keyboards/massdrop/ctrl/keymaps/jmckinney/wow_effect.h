RGB_MATRIX_EFFECT(wow)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "keyutil.h"

#define NUM_ACCENTS 19
#define NUM_GENERAL 16
#define NUM_LAYER1 24

static uint8_t wow_ability_keys[NUM_ACCENTS] = {17, 18, 19, 20, 21, 37, 38, 51, 53, 54, 66, 50, 63, 74, 76, 77, 80, 83, 81};
static uint8_t wow_general_keys[NUM_GENERAL] = {33, 34, 35, 36, 39, 40, 42, 43, 52, 56, 58, 59, 68, 69, 70, 79};
static uint8_t wow_layer1_keys[NUM_LAYER1] = {13, 30, 31, 32, 34, 35, 36, 37, 38, 40, 41, 47, 48, 49, 50, 51, 52, 53, 54, 55, 64, 68, 69, 81};

static bool wow(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  for (uint8_t i = led_min; i < led_max; i++) {
    RGB_MATRIX_TEST_LED_FLAGS();
    rgb_matrix_set_color_all(0, 0, 0);
  }

  /*  accent color  */
    HSV wow_ability_hsv;
    wow_ability_hsv.h = 227;
    wow_ability_hsv.s = 255;
    wow_ability_hsv.v = 255;
    RGB wow_ability_rgb = hsv_to_rgb(wow_ability_hsv);

  /*  blue underglow  */
  HSV underglow = rgb_matrix_config.hsv;
  uint16_t time = scale16by8(g_rgb_counters.tick, rgb_matrix_config.speed / 24);
  underglow.v = scale8(abs8(sin8(time) - 128) * 2, underglow.v);
  if (underglow.v < 62) {
    underglow.v = 62;
  }

  RGB underglow_rgb = hsv_to_rgb(underglow);
  for (uint8_t i = 87; i < 120; i++) {
    rgb_matrix_set_color(i, underglow_rgb.r, underglow_rgb.g, underglow_rgb.b);
  }

  /*  layer 1 (fn mod)  */
  if (layer) {
    /*  accent underglow  */
    underglow.h = wow_ability_hsv.h;
    underglow.s = wow_ability_hsv.s;
    underglow_rgb = hsv_to_rgb(underglow);
    for (uint8_t i = 94; i < 109; i++) {
      rgb_matrix_set_color(i, underglow_rgb.r, underglow_rgb.g, underglow_rgb.b);
    }

    underglow.v = 255;
    underglow_rgb = hsv_to_rgb(underglow);
    for (uint8_t i = 0; i < NUM_LAYER1; ++i) {
      rgb_matrix_set_color(wow_layer1_keys[i], underglow_rgb.r, underglow_rgb.g, underglow_rgb.b);
    }
  } else {
    /*  ability keybinds  */
    for (uint8_t i = 0; i < NUM_ACCENTS; ++i) {
      rgb_matrix_set_color(wow_ability_keys[i], wow_ability_rgb.r, wow_ability_rgb.g, wow_ability_rgb.b);
    }

    /*  general keybinds  */
    RGB rgb = hsv_to_rgb(rgb_matrix_config.hsv);
    for (uint8_t i = 0; i < NUM_GENERAL; ++i) {
      rgb_matrix_set_color(wow_general_keys[i], rgb.r, rgb.g, rgb.b);
    }

    /*  shift mod  */
    HSV shift_hsv;
    shift_hsv.h = 18;
    shift_hsv.s = 255;
    shift_hsv.v = 255;
    RGB shift_rgb = hsv_to_rgb(shift_hsv);
  
    /*  ctrl mod  */
    HSV ctrl_hsv;
    ctrl_hsv.h = 85;
    ctrl_hsv.s = 255;
    ctrl_hsv.v = 255;
    RGB ctrl_rgb = hsv_to_rgb(ctrl_hsv);
    
    /*  alt mod  */
    HSV alt_hsv;
    alt_hsv.h = 33;
    alt_hsv.s = 255;
    alt_hsv.v = 255;
    RGB alt_rgb = hsv_to_rgb(alt_hsv);

    #define ALT (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))
    #define CTRL (get_mods() & MOD_BIT(KC_LCTRL) || get_mods() & MOD_BIT(KC_RCTRL))
    #define SHIFT (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))

    HSV accent_hsv = underglow;

    if (CTRL && ALT && SHIFT) {
      for (uint8_t i = 0; i < NUM_ACCENTS; ++i) {
        rgb_matrix_set_color(wow_ability_keys[i], 255, 255, 255);
      }
      accent_hsv.h = 0;
      accent_hsv.s = 0;
    }

    else if (ALT && SHIFT) {
      for (uint8_t i = 0; i < NUM_ACCENTS; ++i) {
        rgb_matrix_set_color(wow_ability_keys[i], 255, 158, 0);
      }
      accent_hsv.h = 25;
      accent_hsv.s = 255;
    }

    else if (CTRL && ALT) {
      for (uint8_t i = 0; i < NUM_ACCENTS; ++i) {
        rgb_matrix_set_color(wow_ability_keys[i], 128, 255, 0);
      }
      accent_hsv.h = 59;
      accent_hsv.s = 255;
    }

    else if (CTRL && SHIFT) {
      for (uint8_t i = 0; i < NUM_ACCENTS; ++i) {
        rgb_matrix_set_color(wow_ability_keys[i], 170, 139, 0);
      }
      accent_hsv.h = 52;
      accent_hsv.s = 255;
    }

    else if (SHIFT) {
      for (uint8_t i = 0; i < NUM_ACCENTS; ++i) {
        rgb_matrix_set_color(wow_ability_keys[i], shift_rgb.r, shift_rgb.g, shift_rgb.b);
      }
      rgb_matrix_set_color(33, shift_rgb.r, shift_rgb.g, shift_rgb.b);
      rgb_matrix_set_color(30, shift_rgb.r, shift_rgb.g, shift_rgb.b);
      rgb_matrix_set_color(46, shift_rgb.r, shift_rgb.g, shift_rgb.b);
      rgb_matrix_set_color(47, shift_rgb.r, shift_rgb.g, shift_rgb.b);
      accent_hsv = shift_hsv;
    }

    else if (CTRL) {
      for (uint8_t i = 0; i < NUM_ACCENTS; ++i) {
        rgb_matrix_set_color(wow_ability_keys[i], ctrl_rgb.r, ctrl_rgb.g, ctrl_rgb.b);
      }
      accent_hsv = ctrl_hsv;
    }

    else if (ALT) {
      for (uint8_t i = 0; i < NUM_ACCENTS; ++i) {
        rgb_matrix_set_color(wow_ability_keys[i], alt_rgb.r, alt_rgb.g, alt_rgb.b);
      }
      accent_hsv = alt_hsv;
    }

    else { 
      accent_hsv.h = wow_ability_hsv.h;
      accent_hsv.s = wow_ability_hsv.s;
    }

    /*  accent underglow  */
    underglow.h = accent_hsv.h;
    underglow.s = accent_hsv.s;
    underglow_rgb = hsv_to_rgb(underglow);
    for (uint8_t i = 94; i < 109; i++) {
      rgb_matrix_set_color(i, underglow_rgb.r, underglow_rgb.g, underglow_rgb.b);
    }
  }

  return led_max < DRIVER_LED_TOTAL;
}

#endif