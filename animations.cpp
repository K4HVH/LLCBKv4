#include <arduinoFFT.h>
#include "animations.hpp"
#include "definitions.hpp"

iter it;

#define SAMPLES         512           // Must be a power of 2
#define SAMPLING_FREQ   40000         // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define NOISE           0             // Used as a crude noise filter, values below this are ignored

double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;
ArduinoFFT FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQ);


int checkWithDelay(int time) {
  for (int i = 0; i <= time; i++) {
    delay(5);
    if (globals.halt) {
      globals.halt = false;
      return 1;
    }
  }
  return 0;
}

int fixOverflow(int ring, bool large, int input) {
  int fixedVal = 0;
  if (large) {
    if (input > (rings.large.relative.ringEndLed_r[ring] - 1)) {
      fixedVal = input - rings.large.ringLedCount[ring];
    } else if (input < (rings.large.relative.ringStartLed_r[ring] - 1)) {
      fixedVal = input + rings.large.ringLedCount[ring];
    } else {
      fixedVal = input;
    }
  } else {
    if (input > (rings.small.relative.ringEndLed_r[ring] - 1)) {
      fixedVal = input - rings.small.ringLedCount[ring];
    } else if (input < (rings.small.relative.ringStartLed_r[ring] - 1)) {
      fixedVal = input + rings.small.ringLedCount[ring];
    } else {
      fixedVal = input;
    }
  }
  return fixedVal;
}

void fadeInOut(int ring, int prev, bool isFirstInstance, int speed) {
  // Number of steps for fading
  int numSteps = speed; // Adjust the number of steps for faster fading

  // Fade in and fade out
  for (int i = 0; i < numSteps; i++)
  {
    for (const int & value: it.LEDS_LARGE(ring, ring, 1)) {
      int brightness = map(i, 0, numSteps - 1, 0, 255 - (31.9 * ring));
      globals.rLeds[value] = CRGB(brightness, brightness, brightness);
      globals.lLeds[value] = CRGB(brightness, brightness, brightness);
    }

    for (const int & value: it.LEDS_SMALL(ring, ring, 1)) {
      int brightness = map(i, 0, numSteps - 1, 0, 255 - (31.9 * ring));
      if (ring > 3) {
        globals.rLeds[value] = CRGB(brightness, 0, 0);
        globals.lLeds[value] = CRGB(brightness, 0, 0);
      } else {
        globals.rLeds[value] = CRGB(brightness, brightness, brightness);
        globals.lLeds[value] = CRGB(brightness, brightness, brightness);
      }
    }

    // Fade out the previous ring, excluding the first instance
    if (!isFirstInstance)
    {
      for (const int & value: it.LEDS_LARGE(prev, prev, 1)) {
        int brightness = map(numSteps - i - 1, 0, numSteps - 1, 0, 255 - (31.9 * ring));
        globals.rLeds[value] = CRGB(brightness, brightness, brightness);
        globals.lLeds[value] = CRGB(brightness, brightness, brightness);
      }

      for (const int & value: it.LEDS_SMALL(prev, prev, 1)) {
        int brightness = map(numSteps - i - 1, 0, numSteps - 1, 0, 255 - (31.9 * ring));
        if (ring > 2) {
          globals.rLeds[value] = CRGB(brightness, 0, 0);
          globals.lLeds[value] = CRGB(brightness, 0, 0);
        } else {
          globals.rLeds[value] = CRGB(brightness, brightness, brightness);
          globals.lLeds[value] = CRGB(brightness, brightness, brightness);
        }
      }
    }

    FastLED.show();
  }
}

void fadeOut(int ring, int speed) {
  int numSteps = speed;

  for (int i = 0; i < numSteps; i++)
  {
    for (const int & value: it.LEDS_BOTH(ring, ring, ring, ring, 1)) {
      int brightness = map(numSteps - i - 1, 0, numSteps - 1, 0, 255);
      globals.rLeds[value] = CRGB(brightness, brightness, brightness);
      globals.lLeds[value] = CRGB(brightness, brightness, brightness);
    }

    FastLED.show();
  }
}

int eqFix(int ring) {
  switch (ring) {
    case 0:
      return globals.web.bands.Band0 * 100;
      break;
    case 1:
      return globals.web.bands.Band1 * 100;
      break;
    case 2:
      return globals.web.bands.Band2 * 100;
      break;
    case 3:
      return globals.web.bands.Band3 * 100;
      break;
    case 4:
      return globals.web.bands.Band4 * 100;
      break;
    case 5:
      return globals.web.bands.Band5 * 100;
      break;
    case 6:
      return globals.web.bands.Band6 * 100;
      break;
    case 7:
      return globals.web.bands.Band7 * 100;
      break;
    case 8:
      return globals.web.bands.Band8 * 100;
      break;
    default:
      return 1000;
      break;
  }
}

///////////////////////////////////////////////////////////////////// Style 1   --   Static (Classic)

void anims::style1::mode0() {
  while (!globals.shutdown) {
    // clear the LEDs
    for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();
    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style1::mode10() {
  while (!globals.shutdown) {
    // clear the LEDs
    for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }

    // outer ring
    for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
      globals.rLeds[value] = globals.ledCol;
      globals.lLeds[value] = globals.ledCol;
    }

    // Push updated colors
    FastLED.show();
    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style1::mode15() {
  // clear the LEDs
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.lLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.lLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style1::mode16() {
  // clear the LEDs
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.rLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style1::mode17() {
  // clear the LEDs
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.rLeds[value] = globals.indiCol;
      globals.lLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style1::mode20() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {

    for (const int & value: it.LEDS_LARGE(1, 4, 1)) {
      globals.rLeds[value] = globals.ledCol;
      globals.lLeds[value] = globals.ledCol;
    }

    for (const int & value: it.LEDS_SMALL(1, 2, 1)) {
      globals.lLeds[value] = globals.ledCol;
      globals.rLeds[value] = globals.ledCol;
    }

    // Push updated colors
    FastLED.show();
    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style1::mode25() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(1, 4, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(1, 2, 1)) {
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.lLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.lLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style1::mode26() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(1, 4, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(1, 2, 1)) {
    globals.rLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.rLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.rLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style1::mode27() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(1, 4, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(1, 2, 1)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.lLeds[value] = globals.indiCol;
      globals.rLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.lLeds[value] = 0x000000;
      globals.rLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 2   --   Static (Whole)

void anims::style2::mode10() {
  while (!globals.shutdown) {
    // clear the LEDs
    for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }

    // outer ring
    for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
      globals.rLeds[value] = globals.ledCol;
      globals.lLeds[value] = globals.ledCol;
    }

    // Push updated colors
    FastLED.show();
    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style2::mode15() {
  // clear the LEDs
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    // clear the LEDs
    for (const int & value: it.LEDS_SMALL(1, 3, 1)) {
      globals.lLeds[value] = globals.ledCol;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.lLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.lLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style2::mode16() {
  // clear the LEDs
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    // clear the LEDs
    for (const int & value: it.LEDS_SMALL(1, 3, 1)) {
      globals.rLeds[value] = globals.ledCol;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.rLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style2::mode17() {
  // clear the LEDs
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    // clear the LEDs
    for (const int & value: it.LEDS_SMALL(1, 3, 1)) {
      globals.rLeds[value] = globals.ledCol;
      globals.lLeds[value] = globals.ledCol;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.rLeds[value] = globals.indiCol;
      globals.lLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style2::mode20() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {

    for (const int & value: it.LEDS_LARGE(1, 8, 1)) {
      globals.rLeds[value] = globals.ledCol;
      globals.lLeds[value] = globals.ledCol;
    }

    // Push updated colors
    FastLED.show();
    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style2::mode25() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(1, 8, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    // clear the LEDs

    for (const int & value: it.LEDS_SMALL(1, 3, 1)) {
      globals.lLeds[value] = globals.ledCol;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.lLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
      globals.lLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style2::mode26() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(1, 8, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    // clear the LEDs

    for (const int & value: it.LEDS_SMALL(1, 3, 1)) {
      globals.rLeds[value] = globals.ledCol;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.rLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

void anims::style2::mode27() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(1, 8, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    // clear the LEDs

    for (const int & value: it.LEDS_SMALL(1, 3, 1)) {
      globals.lLeds[value] = globals.ledCol;
      globals.rLeds[value] = globals.ledCol;
    }

    for (const int & value: it.LEDS_SMALL(4, 6, 1)) {
      globals.lLeds[value] = globals.indiCol;
      globals.rLeds[value] = globals.indiCol;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }

    for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
      globals.lLeds[value] = 0x000000;
      globals.rLeds[value] = 0x000000;
    }

    // Push updated colors
    FastLED.show();

    if (checkWithDelay(60)) {
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 3   --   Animated

void anims::style3::mode10() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {

    for (int i = 0; i < 8; i++) {

      for (const int & value: it.LEDS_BOTH(1, 1, 1, 1, 8)) {
        globals.rLeds[value + i] = globals.ledCol;
        globals.lLeds[value - i + 7] = globals.ledCol;
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }
      
      for (const int & value: it.LEDS_BOTH(1, 1, 1, 1, 8)) {
        globals.rLeds[value + i] = 0x000000;
        globals.lLeds[value - i + 7] = 0x000000;
      }

      if (globals.halt) {
        globals.halt = false;
        return;
      }
    }
  }
}

void anims::style3::mode15() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {

    for (int i = 0; i < 8; i++) {
      
      for (const int & value: it.LEDS_LARGE(1, 1, 8)) {
        globals.rLeds[value + i] = globals.ledCol;
        globals.lLeds[value - i + 7] = globals.ledCol;
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.rLeds[value + i] = globals.ledCol;
      }

      if (i > 0) {
        for (const int & value: it.LEDS_SMALL(6 - (i - 1), 6 - (i - 1), 1)) {
          if ((7 - i) >= 4) {
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.lLeds[value] = globals.ledCol;
          }
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      for (const int & value: it.LEDS_LARGE(1, 1, 8)) {
        globals.rLeds[value + i] = 0x000000;
        globals.lLeds[value - i + 7] = 0x000000;
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.rLeds[value + i] = 0x000000;
      }

      if (i == 7) {
        for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
          globals.lLeds[value] = 0x000000;
        }
      }

      if (globals.halt) {
        globals.halt = false;
        return;
      }
    }
  }
}

void anims::style3::mode16() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {

    for (int i = 0; i < 8; i++) {
      
      for (const int & value: it.LEDS_LARGE(1, 1, 8)) {
        globals.rLeds[value + i] = globals.ledCol;
        globals.lLeds[value - i + 7] = globals.ledCol;
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.lLeds[value - i + 7] = globals.ledCol;
      }

      if (i > 0) {
        for (const int & value: it.LEDS_SMALL(6 - (i - 1), 6 - (i - 1), 1)) {
          if ((7 - i) >= 4) {
            globals.rLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
          }
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      for (const int & value: it.LEDS_LARGE(1, 1, 8)) {
        globals.rLeds[value + i] = 0x000000;
        globals.lLeds[value - i + 7] = 0x000000;
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.lLeds[value - i + 7] = 0x000000;
      }

      if (i == 7) {
        for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
          globals.rLeds[value] = 0x000000;
        }
      }

      if (globals.halt) {
        globals.halt = false;
        return;
      }
    }
  }
}

void anims::style3::mode17() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {

    for (int i = 0; i < 8; i++) {
      
      for (const int & value: it.LEDS_LARGE(1, 1, 8)) {
        globals.rLeds[value + i] = globals.ledCol;
        globals.lLeds[value - i + 7] = globals.ledCol;
      }

      if (i > 0) {
        for (const int & value: it.LEDS_SMALL(6 - (i - 1), 6 - (i - 1), 1)) {
          if ((7 - i) >= 4) {
            globals.rLeds[value] = globals.indiCol;
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
            globals.lLeds[value] = globals.ledCol;
          }
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      for (const int & value: it.LEDS_LARGE(1, 1, 8)) {
        globals.rLeds[value + i] = 0x000000;
        globals.lLeds[value - i + 7] = 0x000000;
      }

      if (i == 7) {
        for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
      }

      if (globals.halt) {
        globals.halt = false;
        return;
      }
    }
  }
}

void anims::style3::mode20() {
  // clear the interior LEDs from any residual animations
  if (!globals.compBrakeAnim) {
    for (const int & value: it.LEDS_LARGE(1, 8, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }
  }

  for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 8; i++) {
      
      for (const int & value: it.LEDS_LARGE(8 - i, 8 - i, 1)) {
        globals.rLeds[value] = globals.ledCol;
        globals.lLeds[value] = globals.ledCol;
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.lLeds[value - i + 7] = globals.ledCol;
        globals.rLeds[value + i] = globals.ledCol;
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      if (i == 7) {
        globals.compBrakeAnim = true;
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.lLeds[value - i + 7] = 0x000000;
        globals.rLeds[value + i] = 0x000000;
      }

      if (globals.halt) {
        globals.halt = false;
        return;
      }
    }
  }
}

void anims::style3::mode25() {
  // clear the interior LEDs from any residual animations
  if (!globals.compBrakeAnim) {
    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }
  } else {
    for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 8; i++) {

      for (const int & value: it.LEDS_LARGE(8 - i, 8 - i, 1)) {
        globals.rLeds[value] = globals.ledCol;
        globals.lLeds[value] = globals.ledCol;
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.rLeds[value + i] = globals.ledCol;
      }

      if (i > 0) {
        for (const int & value: it.LEDS_SMALL(6 - (i - 1), 6 - (i - 1), 1)) {
          if ((7 - i) >= 4) {
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.lLeds[value] = globals.ledCol;
          }
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      if (i == 7) {
        globals.compBrakeAnim = true;

        for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
          globals.lLeds[value] = 0x000000;
        }
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.rLeds[value + i] = 0x000000;
      }

      if (globals.halt) {
        globals.halt = false;
        return;
      }
    }
  }
}

void anims::style3::mode26() {
  // clear the interior LEDs from any residual animations
  if (!globals.compBrakeAnim) {
    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }
  } else {
    for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 8; i++) {

      for (const int & value: it.LEDS_LARGE(8 - i, 8 - i, 1)) {
        globals.rLeds[value] = globals.ledCol;
        globals.lLeds[value] = globals.ledCol;
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.lLeds[value - i + 7] = globals.ledCol;
      }

      if (i > 0) {
        for (const int & value: it.LEDS_SMALL(6 - (i - 1), 6 - (i - 1), 1)) {
          if ((7 - i) >= 4) {
            globals.rLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
          }
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      if (i == 7) {
        globals.compBrakeAnim = true;

        for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
          globals.rLeds[value] = 0x000000;
        }
      }

      for (const int & value: it.LEDS_SMALL(1, 1, 8)) {
        globals.lLeds[value - i + 7] = 0x000000;
      }

      if (globals.halt) {
        globals.halt = false;
        return;
      }
    }
  }
}

void anims::style3::mode27() {
  // clear the interior LEDs from any residual animations
  if (!globals.compBrakeAnim) {
    for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }
  } else {
    for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }
  }

  // outer ring
  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 8; i++) {

      for (const int & value: it.LEDS_LARGE(8 - i, 8 - i, 1)) {
        globals.rLeds[value] = globals.ledCol;
        globals.lLeds[value] = globals.ledCol;
      }

      if (i > 0) {
        for (const int & value: it.LEDS_SMALL(6 - (i - 1), 6 - (i - 1), 1)) {
          if ((7 - i) >= 4) {
            globals.rLeds[value] = globals.indiCol;
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
            globals.lLeds[value] = globals.ledCol;
          }
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      if (i == 7) {
        globals.compBrakeAnim = true;

        for (const int & value: it.LEDS_SMALL(1, 6, 1)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
      }

      if (globals.halt) {
        globals.halt = false;
        return;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 4   --   Nismo

void anims::style4::mode10() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.lLeds[value] = 0x000000;
    globals.rLeds[value] = 0x000000;
  }

  for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(4, 4, 1)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(4, 4, 1)) {
    globals.lLeds[value] = globals.redCol;
    globals.rLeds[value] = globals.redCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 15; i++) {
      // Large
      if (i < 6) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value - i + 5] = globals.ledCol;
          globals.rLeds[fixOverflow(1, true, value + i + 1)] = globals.ledCol;
        }
      } else if (i > 5 && i < 11) { 
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value - i + 10] = globals.ledCol;
          globals.rLeds[fixOverflow(2, true, value + i - 5)] = globals.ledCol;
        }
      } else if (i > 10 && i < 15) { 
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value - i + 14] = globals.ledCol;
          globals.rLeds[fixOverflow(3, true, value + i - 10)] = globals.ledCol;
        }
      }

      // Small
      if (i < 4) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value - i + 3] = globals.ledCol;
          globals.rLeds[fixOverflow(1, false, value + i + 1)] = globals.ledCol;
        }
      } else if (i > 5 && i < 9) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value - i + 8] = globals.ledCol;
          globals.rLeds[fixOverflow(2, false, value + i - 2)] = globals.ledCol;
        }
      } else if (i > 10 && i < 13) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value - i + 12] = globals.ledCol;
          globals.rLeds[fixOverflow(3, false, value + i - 8)] = globals.ledCol;
        }
      }

      // Hide last led
      if (i == 0) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
      } else if (i == 6) {
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
      } else if (i == 11) {
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      // Large
      if (i < 6 && i != 5) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value - i + 5] = 0x000000;
          globals.rLeds[fixOverflow(1, true, value + i + 1)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 11 && i != 10) {
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value - i + 10] = 0x000000;
          globals.rLeds[fixOverflow(2, true, value + i - 5)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 15 && i != 14) {
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value - i + 14] = 0x000000;
          globals.rLeds[fixOverflow(3, true, value + i - 10)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      }

      // Small
      if (i < 4 && i != 3) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value - i + 3] = 0x000000;
          globals.rLeds[fixOverflow(1, false, value + i + 1)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 9 && i != 8) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value - i + 8] = 0x000000;
          globals.rLeds[fixOverflow(2, false, value + i - 2)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 13 && i != 12) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value - i + 12] = 0x000000;
          globals.rLeds[fixOverflow(3, false, value + i - 8)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      }
    }

    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style4::mode15() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.lLeds[value] = 0x000000;
    globals.rLeds[value] = 0x000000;
  }

  for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(4, 4, 1)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(4, 4, 1)) {
    globals.rLeds[value] = globals.redCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 15; i++) {
      // Large
      if (i < 6) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value - i + 5] = globals.ledCol;
          globals.rLeds[fixOverflow(1, true, value + i + 1)] = globals.ledCol;
        }
      } else if (i > 5 && i < 11) { 
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value - i + 10] = globals.ledCol;
          globals.rLeds[fixOverflow(2, true, value + i - 5)] = globals.ledCol;
        }
      } else if (i > 10 && i < 15) { 
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value - i + 14] = globals.ledCol;
          globals.rLeds[fixOverflow(3, true, value + i - 10)] = globals.ledCol;
        }
      }

      // Small
      if (i < 4) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[fixOverflow(1, false, value + i + 1)] = globals.ledCol;
        }
      } else if (i > 5 && i < 9) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[fixOverflow(2, false, value + i - 2)] = globals.ledCol;
        }
      } else if (i > 10 && i < 13) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[fixOverflow(3, false, value + i - 8)] = globals.ledCol;
        }
      }

      // SMALL (LED SIDE)
      if (i < 7 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(6 - i, 6 - i, 1)) {
          if ((6 - i) >= 4) {
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.lLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(13 - i, 13 - i, 1)) {
          globals.lLeds[value] = 0x000000;
        }
        if (i == 13 && globals.indiout) {
          globals.indiout = false;
        }
      } else if (i < 7 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i, i, 1)) {
          if (i >= 4) {
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.lLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i - 7, i - 7, 1)) {
          globals.lLeds[value] = 0x000000;
        }
        if (i == 13 && !globals.indiout) {
          globals.indiout = true;
        }
      }

      // Hide last led
      if (i == 0) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[value] = 0x000000;
        }
      } else if (i == 6) {
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[value] = 0x000000;
        }
      } else if (i == 11) {
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[value] = 0x000000;
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      // Large
      if (i < 6 && i != 5) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value - i + 5] = 0x000000;
          globals.rLeds[fixOverflow(1, true, value + i + 1)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 11 && i != 10) {
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value - i + 10] = 0x000000;
          globals.rLeds[fixOverflow(2, true, value + i - 5)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 15 && i != 14) {
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value - i + 14] = 0x000000;
          globals.rLeds[fixOverflow(3, true, value + i - 10)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      }

      // Small
      if (i < 4 && i != 3) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[fixOverflow(1, false, value + i + 1)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 9 && i != 8) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[fixOverflow(2, false, value + i - 2)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 13 && i != 12) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[fixOverflow(3, false, value + i - 8)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[value] = 0x000000;
        }
      }
    }

    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style4::mode16() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.lLeds[value] = 0x000000;
    globals.rLeds[value] = 0x000000;
  }

  for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(4, 4, 1)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(4, 4, 1)) {
    globals.lLeds[value] = globals.redCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 15; i++) {
      // Large
      if (i < 6) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value - i + 5] = globals.ledCol;
          globals.rLeds[fixOverflow(1, true, value + i + 1)] = globals.ledCol;
        }
      } else if (i > 5 && i < 11) { 
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value - i + 10] = globals.ledCol;
          globals.rLeds[fixOverflow(2, true, value + i - 5)] = globals.ledCol;
        }
      } else if (i > 10 && i < 15) { 
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value - i + 14] = globals.ledCol;
          globals.rLeds[fixOverflow(3, true, value + i - 10)] = globals.ledCol;
        }
      }

      // Small
      if (i < 4) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value - i + 3] = globals.ledCol;
        }
      } else if (i > 5 && i < 9) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value - i + 8] = globals.ledCol;
        }
      } else if (i > 10 && i < 13) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value - i + 12] = globals.ledCol;
        }
      }

      // SMALL (LED SIDE)
      if (i < 7 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(6 - i, 6 - i, 1)) {
          if ((6 - i) >= 4) {
            globals.rLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(13 - i, 13 - i, 1)) {
          globals.rLeds[value] = 0x000000;
        }
        if (i == 13 && globals.indiout) {
          globals.indiout = false;
        }
      } else if (i < 7 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i, i, 1)) {
          if (i >= 4) {
            globals.rLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i - 7, i - 7, 1)) {
          globals.rLeds[value] = 0x000000;
        }
        if (i == 13 && !globals.indiout) {
          globals.indiout = true;
        }
      }

      // Hide last led
      if (i == 0) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value] = 0x000000;
        }
      } else if (i == 6) {
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value] = 0x000000;
        }
      } else if (i == 11) {
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value] = 0x000000;
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      // Large
      if (i < 6 && i != 5) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value - i + 5] = 0x000000;
          globals.rLeds[fixOverflow(1, true, value + i + 1)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 11 && i != 10) {
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value - i + 10] = 0x000000;
          globals.rLeds[fixOverflow(2, true, value + i - 5)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 15 && i != 14) {
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value - i + 14] = 0x000000;
          globals.rLeds[fixOverflow(3, true, value + i - 10)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      }

      // Small
      if (i < 4 && i != 3) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value - i + 3] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 9 && i != 8) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value - i + 8] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 13 && i != 12) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value - i + 12] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value] = 0x000000;
        }
      }
    }

    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style4::mode17() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(0, 8, 0, 6, 1)) {
    globals.lLeds[value] = 0x000000;
    globals.rLeds[value] = 0x000000;
  }

  for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_LARGE(4, 4, 1)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 15; i++) {
      // Large
      if (i < 6) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value - i + 5] = globals.ledCol;
          globals.rLeds[fixOverflow(1, true, value + i + 1)] = globals.ledCol;
        }
      } else if (i > 5 && i < 11) { 
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value - i + 10] = globals.ledCol;
          globals.rLeds[fixOverflow(2, true, value + i - 5)] = globals.ledCol;
        }
      } else if (i > 10 && i < 15) { 
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value - i + 14] = globals.ledCol;
          globals.rLeds[fixOverflow(3, true, value + i - 10)] = globals.ledCol;
        }
      }

      // SMALL (LED SIDE)
      if (i < 7 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(6 - i, 6 - i, 1)) {
          if ((6 - i) >= 4) {
            globals.rLeds[value] = globals.indiCol;
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
            globals.lLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(13 - i, 13 - i, 1)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        if (i == 13 && globals.indiout) {
          globals.indiout = false;
        }
      } else if (i < 7 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i, i, 1)) {
          if (i >= 4) {
            globals.rLeds[value] = globals.indiCol;
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
            globals.lLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i - 7, i - 7, 1)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
        if (i == 13 && !globals.indiout) {
          globals.indiout = true;
        }
      }

      // Hide last led
      if (i == 0) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
      } else if (i == 6) {
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
      } else if (i == 11) {
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.rLeds[value] = 0x000000;
          globals.lLeds[value] = 0x000000;
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      // Large
      if (i < 6 && i != 5) {
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value - i + 5] = 0x000000;
          globals.rLeds[fixOverflow(1, true, value + i + 1)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(1, 1, 6)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 11 && i != 10) {
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value - i + 10] = 0x000000;
          globals.rLeds[fixOverflow(2, true, value + i - 5)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(2, 2, 5)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 15 && i != 14) {
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value - i + 14] = 0x000000;
          globals.rLeds[fixOverflow(3, true, value + i - 10)] = 0x000000;
        }
        for (const int & value: it.LEDS_LARGE(3, 3, 4)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      }
    }

    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style4::mode20() {
  // clear the interior LEDs from any residual animations
  if (!globals.compBrakeAnim) {
    for (const int & value: it.LEDS_LARGE(0, 8, 1)) {
      globals.lLeds[value] = 0x000000;
      globals.rLeds[value] = 0x000000;
    }
  }

  for (const int & value: it.LEDS_SMALL(0, 6, 1)) {
    globals.lLeds[value] = 0x000000;
    globals.rLeds[value] = 0x000000;
  }

  for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
    globals.lLeds[value] = globals.ledCol;
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(4, 4, 1)) {
    globals.lLeds[value] = globals.redCol;
    globals.rLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 15; i++) {

      // Large
      if (i < 10) {
        for (const int & value: it.LEDS_LARGE(9 - i, 9 - i, 1)) {
          globals.lLeds[value] = globals.ledCol;
          globals.rLeds[value] = globals.ledCol;
        }
      }

      // Small
      if (i < 4) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value - i + 3] = globals.ledCol;
          globals.rLeds[fixOverflow(1, false, value + i + 1)] = globals.ledCol;
        }
      } else if (i > 5 && i < 9) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value - i + 8] = globals.ledCol;
          globals.rLeds[fixOverflow(2, false, value + i - 2)] = globals.ledCol;
        }
      } else if (i > 10 && i < 13) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value - i + 12] = globals.ledCol;
          globals.rLeds[fixOverflow(3, false, value + i - 8)] = globals.ledCol;
        }
      }

      // Hide last led
      if (i == 0) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i == 6) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i == 11) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      if (i == 9) {
        globals.compBrakeAnim = true;
      }

      // Small
      if (i < 4 && i != 3) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value - i + 3] = 0x000000;
          globals.rLeds[fixOverflow(1, false, value + i + 1)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 9 && i != 8) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value - i + 8] = 0x000000;
          globals.rLeds[fixOverflow(2, false, value + i - 2)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 13 && i != 12) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value - i + 12] = 0x000000;
          globals.rLeds[fixOverflow(3, false, value + i - 8)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
      }
    }

    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style4::mode25() {
  // clear the interior LEDs from any residual animations
  if (!globals.compBrakeAnim) {
    for (const int & value: it.LEDS_LARGE(0, 8, 1)) {
      globals.lLeds[value] = 0x000000;
      globals.rLeds[value] = 0x000000;
    }
  }

  for (const int & value: it.LEDS_SMALL(0, 6, 1)) {
    globals.lLeds[value] = 0x000000;
    globals.rLeds[value] = 0x000000;
  }

  for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
    globals.rLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(4, 4, 1)) {
    globals.rLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 15; i++) {

      // Large
      if (i < 10) {
        for (const int & value: it.LEDS_LARGE(9 - i, 9 - i, 1)) {
          globals.lLeds[value] = globals.ledCol;
          globals.rLeds[value] = globals.ledCol;
        }
      }

      // Small
      if (i < 4) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[fixOverflow(1, false, value + i + 1)] = globals.ledCol;
        }
      } else if (i > 5 && i < 9) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[fixOverflow(2, false, value + i - 2)] = globals.ledCol;
        }
      } else if (i > 10 && i < 13) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[fixOverflow(3, false, value + i - 8)] = globals.ledCol;
        }
      }

      // SMALL (LED SIDE)
      if (i < 7 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(6 - i, 6 - i, 1)) {
          if ((6 - i) >= 4) {
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.lLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(13 - i, 13 - i, 1)) {
          globals.lLeds[value] = 0x000000;
        }
        if (i == 13 && globals.indiout) {
          globals.indiout = false;
        }
      } else if (i < 7 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i, i, 1)) {
          if (i >= 4) {
            globals.lLeds[value] = globals.indiCol;
          } else {
            globals.lLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i - 7, i - 7, 1)) {
          globals.lLeds[value] = 0x000000;
        }
        if (i == 13 && !globals.indiout) {
          globals.indiout = true;
        }
      }

      // Hide last led
      if (i == 0) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[value] = 0x000000;
        }
      } else if (i == 6) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[value] = 0x000000;
        }
      } else if (i == 11) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[value] = 0x000000;
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      if (i == 9) {
        globals.compBrakeAnim = true;
      }

      // Small
      if (i < 4 && i != 3) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[fixOverflow(1, false, value + i + 1)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 9 && i != 8) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[fixOverflow(2, false, value + i - 2)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.rLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 13 && i != 12) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[fixOverflow(3, false, value + i - 8)] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.rLeds[value] = 0x000000;
        }
      }
    }

    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style4::mode26() {
  // clear the interior LEDs from any residual animations
  if (!globals.compBrakeAnim) {
    for (const int & value: it.LEDS_LARGE(0, 8, 1)) {
      globals.lLeds[value] = 0x000000;
      globals.rLeds[value] = 0x000000;
    }
  }

  for (const int & value: it.LEDS_SMALL(0, 6, 1)) {
    globals.lLeds[value] = 0x000000;
    globals.rLeds[value] = 0x000000;
  }

  for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
    globals.lLeds[value] = globals.ledCol;
  }

  for (const int & value: it.LEDS_SMALL(4, 4, 1)) {
    globals.lLeds[value] = globals.redCol;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 15; i++) {

      // Large
      if (i < 10) {
        for (const int & value: it.LEDS_LARGE(9 - i, 9 - i, 1)) {
          globals.lLeds[value] = globals.ledCol;
          globals.rLeds[value] = globals.ledCol;
        }
      }

      // Small
      if (i < 4) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value - i + 3] = globals.ledCol;
        }
      } else if (i > 5 && i < 9) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value - i + 8] = globals.ledCol;
        }
      } else if (i > 10 && i < 13) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value - i + 12] = globals.ledCol;
        }
      }

      // SMALL (LED SIDE)
      if (i < 7 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(6 - i, 6 - i, 1)) {
          if ((6 - i) >= 4) {
            globals.rLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(13 - i, 13 - i, 1)) {
          globals.rLeds[value] = 0x000000;
        }
        if (i == 13 && globals.indiout) {
          globals.indiout = false;
        }
      } else if (i < 7 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i, i, 1)) {
          if (i >= 4) {
            globals.rLeds[value] = globals.indiCol;
          } else {
            globals.rLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i - 7, i - 7, 1)) {
          globals.rLeds[value] = 0x000000;
        }
        if (i == 13 && !globals.indiout) {
          globals.indiout = true;
        }
      }

      // Hide last led
      if (i == 0) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value] = 0x000000;
        }
      } else if (i == 6) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value] = 0x000000;
        }
      } else if (i == 11) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value] = 0x000000;
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      if (i == 9) {
        globals.compBrakeAnim = true;
      }

      // Small
      if (i < 4 && i != 3) {
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value - i + 3] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(1, 1, 4)) {
          globals.lLeds[value] = 0x000000;
        }
      } else if (i > 5 && i < 9 && i != 8) {
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value - i + 8] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(2, 2, 3)) {
          globals.lLeds[value] = 0x000000;
        }
      } else if (i > 10 && i < 13 && i != 12) {
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value - i + 12] = 0x000000;
        }
        for (const int & value: it.LEDS_SMALL(3, 3, 2)) {
          globals.lLeds[value] = 0x000000;
        }
      }
    }

    if (checkWithDelay(5)) {
      return;
    }
  }
}

void anims::style4::mode27() {
  // clear the interior LEDs from any residual animations
  if (!globals.compBrakeAnim) {
    for (const int & value: it.LEDS_LARGE(0, 8, 1)) {
      globals.lLeds[value] = 0x000000;
      globals.rLeds[value] = 0x000000;
    }
  }

  for (const int & value: it.LEDS_SMALL(0, 6, 1)) {
    globals.lLeds[value] = 0x000000;
    globals.rLeds[value] = 0x000000;
  }

  while (!globals.shutdown) {
    for (int i = 0; i < 15; i++) {

      // Large
      if (i < 10) {
        for (const int & value: it.LEDS_LARGE(9 - i, 9 - i, 1)) {
          globals.lLeds[value] = globals.ledCol;
          globals.rLeds[value] = globals.ledCol;
        }
      }

      // SMALL (LED SIDE)
      if (i < 7 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(6 - i, 6 - i, 1)) {
          if ((6 - i) >= 4) {
            globals.lLeds[value] = globals.indiCol;
            globals.rLeds[value] = globals.indiCol;
          } else {
            globals.lLeds[value] = globals.ledCol;
            globals.rLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && globals.indiout) {
        for (const int & value: it.LEDS_SMALL(13 - i, 13 - i, 1)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
        if (i == 13 && globals.indiout) {
          globals.indiout = false;
        }
      } else if (i < 7 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i, i, 1)) {
          if (i >= 4) {
            globals.lLeds[value] = globals.indiCol;
            globals.rLeds[value] = globals.indiCol;
          } else {
            globals.lLeds[value] = globals.ledCol;
            globals.rLeds[value] = globals.ledCol;
          }
        }
      } else if (i > 6 && i < 14 && !globals.indiout) {
        for (const int & value: it.LEDS_SMALL(i - 7, i - 7, 1)) {
          globals.lLeds[value] = 0x000000;
          globals.rLeds[value] = 0x000000;
        }
        if (i == 13 && !globals.indiout) {
          globals.indiout = true;
        }
      }

      // Push updated colors
      FastLED.show();
      if (checkWithDelay(8)) {
        return;
      }

      if (i == 9) {
        globals.compBrakeAnim = true;
      }
    }

    if (checkWithDelay(5)) {
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 5   --   Test

void anims::style5::mode10() {
  // clear the interior LEDs from any residual animations
  for (const int & value: it.LEDS_BOTH(1, 8, 1, 6, 1)) {
    globals.rLeds[value] = 0x000000;
    globals.lLeds[value] = 0x000000;
  }

  for (const int & value: it.LEDS_BOTH(0, 0, 0, 0, 1)) {
    globals.rLeds[value] = globals.ledCol;
    globals.lLeds[value] = globals.ledCol;
  }

  while (!globals.shutdown) {


    if (checkWithDelay(5)) {
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 6   --   Lightshow

void anims::style6::mode10() {
  while (!globals.shutdown) {
    static uint16_t sPseudotime = 0;
    static uint16_t sLastMillis = 0;
    static uint16_t sHue16 = 0;
  
    uint8_t sat8 = beatsin16(20, 220, 250);
    uint8_t brightdepth = beatsin88(341, 230, 255);
    uint16_t brightnessthetainc16 = beatsin16(3, (1 * 256), (100 * 256), 0, 65535/2);
    uint8_t msmultiplier = beatsin88(147, 23, 60);

    uint16_t hue16 = sHue16;
    uint16_t hueinc16 = beatsin88(113, 1, 3000);
    
    uint16_t ms = millis();
    uint16_t deltams = ms - sLastMillis;
    sLastMillis = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16 += deltams * beatsin88(400, 5, 9);
    uint16_t brightnesstheta16 = sPseudotime;
    
    // Process both strips with separate hue progression
    uint16_t hue16_right = hue16;
    uint16_t hue16_left = hue16;
    
    for(uint16_t i = 0; i < globals.totalLEDs; i++) {
      // Right strip - clockwise
      {
        uint8_t hue8 = hue16_right / 256;
        brightnesstheta16 += brightnessthetainc16;
        uint16_t b16 = sin16(brightnesstheta16) + 32768;
        uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
        uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
        bri8 += (255 - brightdepth);
        
        CRGB newcolor = CHSV(hue8, sat8, bri8);
        uint16_t pixelnumber = (globals.totalLEDs-1) - i;
        
        nblend(globals.rLeds[pixelnumber], newcolor, 64);
        
        hue16_right += hueinc16;
      }
      
      // Left strip - counter-clockwise
      {
        uint8_t hue8 = hue16_left / 256;
        uint16_t b16 = sin16(brightnesstheta16) + 32768;
        uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
        uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
        bri8 += (255 - brightdepth);
        
        CRGB newcolor = CHSV(hue8, sat8, bri8);
        
        nblend(globals.lLeds[i], newcolor, 64);
        
        hue16_left += hueinc16;
      }
    }

    FastLED.show();

    if (globals.halt) {
      globals.halt = false;
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 7   --   Music (Peaks)

void anims::style7::mode10() {
  // FFT setup
  unsigned int sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
  
  // State arrays for visualization
  byte peak[9] = {0};
  int oldRingHeights[9] = {0};
  int ringValues[9] = {0};
  
  // Color management
  uint8_t baseHue = 0;            // Base color hue
  uint8_t hueSpread = 220;        // Total hue spread across rings
  uint8_t hueStep = hueSpread / 9; // Hue step between rings
  uint8_t pulseOffset = 0;        // For color pulsing effect
  
  // Peak fall control
  const uint8_t peakFallDelay = 60;  // ms between peak falls
  const uint8_t peakFallRate = 1;    // how many LEDs the peak falls per delay
  unsigned long lastPeakFall = 0;
  
  while (!globals.shutdown) {
    // Clear previous frame
    for (const int &value : it.LEDS_BOTH(0, 8, 0, 8, 1)) {
      globals.rLeds[value] = 0x000000;
      globals.lLeds[value] = 0x000000;
    }
    
    // Reset ring values
    memset(ringValues, 0, sizeof(ringValues));
    
    // Sample audio and perform FFT
    for (int i = 0; i < SAMPLES; i++) {
      newTime = micros();
      vReal[i] = analogRead(34);
      vImag[i] = 0;
      while (micros() - newTime < sampling_period_us) { /* wait */ }
    }
    
    FFT.dcRemoval();
    FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.compute(FFT_FORWARD);
    FFT.complexToMagnitude();
    
    // Process FFT data into frequency bands
    for (int i = 2; i < (SAMPLES/2); i++) {
      if (vReal[i] > NOISE) {
        if (i<=4)             ringValues[0] += (int)vReal[i];
        else if (i<=8)        ringValues[1] += (int)vReal[i];
        else if (i<=16)       ringValues[2] += (int)vReal[i];
        else if (i<=32)       ringValues[3] += (int)vReal[i];
        else if (i<=64)       ringValues[4] += (int)vReal[i];
        else if (i<=128)      ringValues[5] += (int)vReal[i];
        else if (i<=188)      ringValues[6] += (int)vReal[i];
        else if (i<=256)      ringValues[7] += (int)vReal[i];
        else                  ringValues[8] += (int)vReal[i];
      }
    }
    
    // Update peaks and draw visualization
    unsigned long currentMillis = millis();
    for (int ring = 0; ring < 9; ring++) {
      // Scale the values based on ring
      int ringHeight = ringValues[ring] / eqFix(ring);
      ringHeight = min(ringHeight, rings.large.ringLedCount[ring]);
      
      // Smooth transitions
      ringHeight = ((oldRingHeights[ring] * 2) + ringHeight) / 3;
      oldRingHeights[ring] = ringHeight;
      
      // Update peaks
      if (ringHeight > peak[ring]) {
        peak[ring] = ringHeight;
      }
      
      // Calculate colors
      uint8_t mainHue = baseHue + (ring * hueStep);
      uint8_t peakHue = mainHue + 128; // Complementary color for peaks
      uint8_t sat = 255;
      uint8_t val = 255;
      
      // Draw large ring visualization
      for (const int &value : it.LEDS_LARGE_BAR_INV(ring, ringHeight)) {
        // Add brightness variation based on height
        uint8_t brightness = map(value, 0, ringHeight, 128, 255);
        globals.rLeds[value] = CHSV(mainHue, sat, brightness);
        globals.lLeds[value] = CHSV(mainHue, sat, brightness);
      }
      
      // Draw small ring visualization with slight delay effect
      int smallHeight = ringHeight * 0.75; // Smaller bars for inner rings
      for (const int &value : it.LEDS_SMALL_BAR_INV(ring, smallHeight)) {
        uint8_t brightness = map(value, 0, smallHeight, 128, 255);
        globals.rLeds[value] = CHSV(mainHue + 32, sat, brightness);
        globals.lLeds[value] = CHSV(mainHue + 32, sat, brightness);
      }
      
      // Draw peaks with complementary colors
      if (peak[ring] > 0) {
        for (const int &value : it.LEDS_LARGE_PEAK_INV(ring, peak[ring])) {
          globals.rLeds[value] = CHSV(peakHue, sat, val);
          globals.lLeds[value] = CHSV(peakHue, sat, val);
        }
        for (const int &value : it.LEDS_SMALL_PEAK_INV(ring, peak[ring] * 0.75)) {
          globals.rLeds[value] = CHSV(peakHue + 32, sat, val);
          globals.lLeds[value] = CHSV(peakHue + 32, sat, val);
        }
      }
    }
    
    // Handle peak fall
    if (currentMillis - lastPeakFall > peakFallDelay) {
      lastPeakFall = currentMillis;
      for (int i = 0; i < 9; i++) {
        if (peak[i] > 0) peak[i] = max(0, peak[i] - peakFallRate);
      }
    }
    
    // Rotate base hue slowly
    EVERY_N_MILLISECONDS(50) {
      baseHue++;
      pulseOffset++;
    }
    
    FastLED.show();
    
    if (checkWithDelay(1)) {
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 8   --   Music (No Peaks)

///////////////////////////////////////////////////////////////////// Style 9   --   Sinelon

void anims::style9::mode10() {
  const int TOTAL_PATH = globals.totalLEDs * 2; // Total path length across both strips
  static int last_pos = 0;
  
  while (!globals.shutdown) {
    fadeToBlackBy(globals.rLeds, globals.totalLEDs, 40);
    fadeToBlackBy(globals.lLeds, globals.totalLEDs, 40);
    
    // Generate position over complete path length
    int pos = beatsin16(10, 0, TOTAL_PATH - 1);
    
    while (last_pos != pos) {
      if (last_pos < pos) {
        last_pos++;
      } else {
        last_pos--;
      }
      
      // Determine which strip and position
      if (last_pos < globals.totalLEDs) {
        // Left strip - forward direction
        int ledPos = last_pos;
        if (ledPos >= globals.totalLEDs - 21) {
          globals.lLeds[ledPos] = globals.redCol;
        } else {
          globals.lLeds[ledPos] = globals.ledCol;
        }
      } else {
        // Right strip - reverse direction
        int ledPos = TOTAL_PATH - last_pos - 1;
        if (ledPos >= globals.totalLEDs - 21) {
          globals.rLeds[ledPos] = globals.redCol;
        } else {
          globals.rLeds[ledPos] = globals.ledCol;
        }
      }
    }
    
    FastLED.show();
    
    if (checkWithDelay(2)) {
      last_pos = 0;
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 10   --   Concentric

void anims::style10::mode10() {
  //bool isFirstInstance = true;
  while (!globals.shutdown) {
    fadeInOut(8, 0, true, 30);
    if (checkWithDelay(1)) { return; }
    fadeInOut(7, 8, false, 20);
    if (checkWithDelay(1)) { return; }
    fadeInOut(6, 7, false, 15);
    if (checkWithDelay(1)) { return; }
    fadeInOut(5, 6, false, 15);
    if (checkWithDelay(1)) { return; }
    fadeInOut(4, 5, false, 15);
    if (checkWithDelay(1)) { return; }
    fadeInOut(3, 4, false, 15);
    if (checkWithDelay(1)) { return; }
    fadeInOut(2, 3, false, 10);
    if (checkWithDelay(1)) { return; }
    fadeInOut(1, 2, false, 10);
    if (checkWithDelay(1)) { return; }
    fadeInOut(0, 1, false, 10);
    if (checkWithDelay(1)) { return; }
    fadeOut(0, 60);
    if (checkWithDelay(190)) {
      return;
    }
    //isFirstInstance = false;
  }
}

///////////////////////////////////////////////////////////////////// Style 11   --   Demo

void demo::mode10() {
  anims an;

  while (!globals.shutdown) {
    switch (globals.demoIndex) {
      case 0:
        an.style1.mode10();
        break;
      case 1:
        an.style1.mode15();
        break;
      case 2:
        an.style1.mode16();
        break;
      case 3:
        an.style1.mode17();
        break;
      case 4:
        an.style1.mode20();
        break;
      case 5:
        an.style1.mode25();
        break;
      case 6:
        an.style1.mode26();
        break;
      case 7:
        an.style1.mode27();
        break;
      case 8:
        an.style2.mode10();
        break;
      case 9:
        an.style2.mode15();
        break;
      case 10:
        an.style2.mode16();
        break;
      case 11:
        an.style2.mode17();
        break;
      case 12:
        an.style2.mode20();
        break;
      case 13:
        an.style2.mode25();
        break;
      case 14:
        an.style2.mode26();
        break;
      case 15:
        an.style2.mode27();
        break;
      case 16:
        an.style3.mode10();
        break;
      case 17:
        an.style3.mode15();
        break;
      case 18:
        an.style3.mode16();
        break;
      case 19:
        an.style3.mode17();
        break;
      case 20:
        an.style3.mode20();
        break;
      case 21:
        an.style3.mode25();
        break;
      case 22:
        an.style3.mode26();
        break;
      case 23:
        an.style3.mode27();
        break;
      case 24:
        an.style4.mode10();
        break;
      case 25:
        an.style4.mode15();
        break;
      case 26:
        an.style4.mode16();
        break;
      case 27:
        an.style4.mode17();
        break;
      case 28:
        an.style4.mode20();
        break;
      case 29:
        an.style4.mode25();
        break;
      case 30:
        an.style4.mode26();
        break;
      case 31:
        an.style4.mode27();
        break;
      case 32:
        an.style5.mode10();
        break;
      case 33:
        //an.style5.mode15();
        break;
      case 34:
        //an.style5.mode16();
        break;
      case 35:
        //an.style5.mode17();
        break;
      case 36:
        //an.style5.mode20();
        break;
      case 37:
        //an.style5.mode25();
        break;
      case 38:
        //an.style5.mode26();
        break;
      case 39:
        //an.style5.mode27();
        break;
      case 40:
        an.style6.mode10();
        break;
      case 41:
        an.style7.mode10();
        break;
      case 42:
        //an.style8.mode10();
        break;
      case 43:
        an.style9.mode10();
        break;
      case 44:
        an.style10.mode10();
        break;
      case 45:
        an.style13.mode10();
        break;
      case 46:
        an.style14.mode10();
        break;
      case 47:
        an.style15.mode10();
        break;
      case 48:
        an.style16.mode10();
        break;
    }

    if (checkWithDelay(2)) {
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 13   --   Diagonal

void anims::style13::mode10() {
  while (!globals.shutdown) {
    int offset = beat8(30);

    for (uint8_t i = 0; i < 241; i++) {
      globals.lLeds[i] = ColorFromPalette(RainbowColors_p, offset + ringmap.large.coordsX[i] + ringmap.large.coordsY[i]);
      globals.rLeds[i] = ColorFromPalette(RainbowColors_p, offset - ringmap.large.coordsX[i] + ringmap.large.coordsY[i]);
    }

    for (uint8_t i = 0; i < 133; i++) {
      globals.lLeds[i+241] = ColorFromPalette(RainbowColors_p, offset + ringmap.small.coordsX[i] + ringmap.small.coordsY[i]);
      globals.rLeds[i+241] = ColorFromPalette(RainbowColors_p, offset - ringmap.small.coordsX[i] + ringmap.small.coordsY[i]);
    }

    FastLED.show();

    if (globals.halt) {
      globals.halt = false;
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 14   --   Spiral

void anims::style14::mode10() {
  while (!globals.shutdown) {
    int offset = beat8(30);

    for (uint8_t i = 0; i < 241; i++) {
      globals.lLeds[i] = ColorFromPalette(RainbowColors_p, offset + ringmap.large.angles[i]);
      globals.rLeds[i] = ColorFromPalette(RainbowColors_p, offset - ringmap.large.angles[i]);
    }

    for (uint8_t i = 0; i < 133; i++) {
      globals.lLeds[i+241] = ColorFromPalette(RainbowColors_p, offset + ringmap.small.angles[i]);
      globals.rLeds[i+241] = ColorFromPalette(RainbowColors_p, offset - ringmap.small.angles[i]);
    }

    FastLED.show();

    if (globals.halt) {
      globals.halt = false;
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 15   --   Inwards

void anims::style15::mode10() {
  while (!globals.shutdown) {
    int offset = beat8(30);

    for (uint8_t i = 0; i < 241; i++) {
      globals.lLeds[i] = ColorFromPalette(RainbowColors_p, offset + ringmap.large.radii[i]);
      globals.rLeds[i] = ColorFromPalette(RainbowColors_p, offset + ringmap.large.radii[i]);
    }

    for (uint8_t i = 0; i < 133; i++) {
      globals.lLeds[i+241] = ColorFromPalette(RainbowColors_p, offset + ringmap.small.radii[i]);
      globals.rLeds[i+241] = ColorFromPalette(RainbowColors_p, offset + ringmap.small.radii[i]);
    }

    FastLED.show();

    if (globals.halt) {
      globals.halt = false;
      return;
    }
  }
}

///////////////////////////////////////////////////////////////////// Style 16   --   Outwards

void anims::style16::mode10() {
  while (!globals.shutdown) {
    int offset = beat8(30);

    for (uint8_t i = 0; i < 241; i++) {
      globals.lLeds[i] = ColorFromPalette(RainbowColors_p, offset - ringmap.large.radii[i]);
      globals.rLeds[i] = ColorFromPalette(RainbowColors_p, offset - ringmap.large.radii[i]);
    }

    for (uint8_t i = 0; i < 133; i++) {
      globals.lLeds[i+241] = ColorFromPalette(RainbowColors_p, offset - ringmap.small.radii[i]);
      globals.rLeds[i+241] = ColorFromPalette(RainbowColors_p, offset - ringmap.small.radii[i]);
    }

    FastLED.show();

    if (globals.halt) {
      globals.halt = false;
      return;
    }
  }
}