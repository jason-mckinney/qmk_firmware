RGB_MATRIX_EFFECT(split_yellow_blue)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "keyutil.h"

#define NUM_YELLOW 43
#define NUM_LAYER1 24

static uint8_t layer1_keys[NUM_LAYER1] = {13, 30, 31, 32, 34, 35, 36, 37, 38, 40, 41, 47, 48, 49, 50, 51, 52, 53, 54, 55, 64, 68, 69, 81};

static uint8_t yellow_keys[NUM_YELLOW] = {
   0,  1,  2,  3,  4,  5,  6,
  16, 17, 18, 19, 20, 21, 22, 23, 24,
  33, 34, 35, 36, 37, 38, 39, 40,
  50, 51, 52, 53, 54, 55, 56, 57,
  63, 64, 65, 66, 67, 68, 69,
  76, 77, 78, 79,
};

static bool split_yellow_blue(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  /*  accent color  */
  HSV yellow_hsv;
  yellow_hsv.h = 33;
  yellow_hsv.s = 255;
  yellow_hsv.v = 255;
  RGB yellow_rgb = hsv_to_rgb(yellow_hsv);

  rgb_matrix_set_color_all(0, 0, 0);

  if(layer) {
    for (uint8_t i = 0; i < NUM_LAYER1; ++i) {
      rgb_matrix_set_color(layer1_keys[i], yellow_rgb.r, yellow_rgb.g, yellow_rgb.b);
    }
  } else {
    /*  blue keys  */
    RGB blue_rgb = hsv_to_rgb(rgb_matrix_config.hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
      RGB_MATRIX_TEST_LED_FLAGS();
      rgb_matrix_set_color_all(blue_rgb.r, blue_rgb.g, blue_rgb.b);
    }

    /*  yellow keys  */
    for (uint8_t i = 0; i < NUM_YELLOW; ++i) {
      rgb_matrix_set_color(yellow_keys[i], yellow_rgb.r, yellow_rgb.g, yellow_rgb.b);
    }
  }

  /*  underglow  */
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

  underglow.h = yellow_hsv.h;
  underglow.s = yellow_hsv.s;
  underglow_rgb = hsv_to_rgb(underglow);
  for (uint8_t i = 94; i < 109; i++) {
    rgb_matrix_set_color(i, underglow_rgb.r, underglow_rgb.g, underglow_rgb.b);
  }

  return led_max < DRIVER_LED_TOTAL;
}

#endif