#define FASTLED_ESP32_FLASH_LOCK 1
#define FASTLED_EXPERIMENTAL_ESP32_RGBW_ENABLED 1
// #define FASTLED_ESP32_I2S true

#include <FastLED.h>
#include "rings.hpp"

inline struct globals {
  int totalLEDs = 374;
  CRGB rLeds[374];
  CRGB lLeds[374];

  bool shutdown = false;
  bool halt = false;
  bool compBrakeAnim = false;
  bool indiout = true;

  int desiredStyle = 0;
  int desiredMode = 0;
  int runningStyle = 0;
  int runningMode = 0;

  int backupStyle = 0;
  bool backupStaticState = false;
  bool backupOverrideState = false;

  int ledCol = 0xFFFFFF;
  int redCol = 0xFF0000;
  int indiCol = 0xFF5000;

  int demoTimer = 0;
  int demoIndex = 0;

  struct {
    uint8_t tab = 1;
    uint8_t overrideStyle = 1;
    struct {
      uint8_t Band0;
      uint8_t Band1;
      uint8_t Band2;
      uint8_t Band3;
      uint8_t Band4;
      uint8_t Band5;
      uint8_t Band6;
      uint8_t Band7;
      uint8_t Band8;
    } bands;
  } web;
} globals;

class iter {
  public:
    std::vector<int> LEDS_BOTH(int r1start, int r1stop, int r2start, int r2stop, int step);
    std::vector<int> LEDS_LARGE(int r1start, int r1stop, int step);
    std::vector<int> LEDS_SMALL(int r1start, int r1stop, int step);
    std::vector<int> LEDS_BOTH_BAR(int r1start, int r1stop, int r2start, int r2stop);
    std::vector<int> LEDS_LARGE_BAR(int r1start, int r1stop);
    std::vector<int> LEDS_SMALL_BAR(int r1start, int r1stop);
    std::vector<int> LEDS_BOTH_BAR_INV(int r1start, int r1stop, int r2start, int r2stop);
    std::vector<int> LEDS_LARGE_BAR_INV(int r1start, int r1stop);
    std::vector<int> LEDS_SMALL_BAR_INV(int r1start, int r1stop);
    std::vector<int> LEDS_BOTH_PEAK(int r1start, int r1offset, int r2start, int r2offset);
    std::vector<int> LEDS_LARGE_PEAK(int r1start, int r1offset);
    std::vector<int> LEDS_SMALL_PEAK(int r1start, int r1offset);
    std::vector<int> LEDS_BOTH_PEAK_INV(int r1start, int r1offset, int r2start, int r2offset);
    std::vector<int> LEDS_LARGE_PEAK_INV(int r1start, int r1offset);
    std::vector<int> LEDS_SMALL_PEAK_INV(int r1start, int r1offset);
};