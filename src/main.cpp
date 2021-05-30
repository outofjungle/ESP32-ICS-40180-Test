#include <Arduino.h>
#include <FastLED.h>

#define CLOCK_PIN 18
#define DATA_PIN 23
#define AUDIO_PIN 32

#define AUD_MAX 4096

#define NUM_LEDS 10
#define DELAY 20

CRGB leds[NUM_LEDS];

void update_color(uint32_t min, uint32_t max)
{
  uint32_t peak2peak = max - min;
  uint32_t amp = map(peak2peak, 0, AUD_MAX, 0, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Blue);

  Serial.printf("min=%d, max=%d, amp=%d\n", min, max, amp);

  for (int i; i <= amp; i++)
  {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
}

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  FastLED.setBrightness(0xFF);
  while (!Serial)
  {
  }
}

void loop()
{
  static bool do_update;
  static uint32_t last_updated;
  static uint32_t signal;
  static uint32_t min;
  static uint32_t max;

  if (do_update)
  {
    update_color(min, max);

    min = max = 2048;
    last_updated = millis();
    do_update = false;
  }

  if (millis() - last_updated > DELAY)
  {
    do_update = true;
  }

  signal = analogRead(AUDIO_PIN);
  if (signal < AUD_MAX)
  {
    if (signal < min)
    {
      min = signal;
    }
    else if (signal > max)
    {
      max = signal;
    }
  }
}
