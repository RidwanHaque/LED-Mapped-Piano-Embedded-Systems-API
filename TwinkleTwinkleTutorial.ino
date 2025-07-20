#include <FastLED.h>

// ---- LED Settings ----
#define NUM_LEDS    144
#define DATA_PIN    2
#define CHIPSET     WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  30 // A good visible brightness

CRGB leds[NUM_LEDS];

// ---- Song Data for "Twinkle, Twinkle, Little Star" ----
// This array holds the MIDI note numbers for the melody.
// Middle C is 60.
// int songNotes[] = {
//   60, 60, 67, 67, 69, 69, 67,   // Twin-kle Twin-kle Lit-tle Star
//   65, 65, 64, 64, 62, 62, 60    // How I Won-der What You Are
// };

// ---- Song Data for "Twinkle, Twinkle, Little Star" (One Octave Lower) ----
// This array holds the MIDI note numbers for the melody.
// The song now starts on C3 (MIDI note 48).
int songNotes[] = {
  48, 48, 55, 55, 57, 57, 55,   // C3, C3, G3, G3, A3, A3, G3
  53, 53, 52, 52, 50, 50, 48    // F3, F3, E3, E3, D3, D3, C3
};
// This array holds the duration for each note in milliseconds.
// 400ms = quarter note, 800ms = half note (at a simple tempo)
int noteDurations[] = {
  400, 400, 400, 400, 400, 400, 800,
  400, 400, 400, 400, 400, 400, 800
};

void setup() {
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  FastLED.clear();
  FastLED.show();
}

void loop() {
  // Get the total number of notes in the song
  int totalNotes = sizeof(songNotes) / sizeof(songNotes[0]);

  // The first note on your Keystation 49 is C2, which is MIDI note 36.
  const int STARTING_NOTE = 36;
  // You specified that each key is about 2 LEDs wide.
  const int LEDS_PER_KEY = 2;

  // Loop through each note of the song
  for (int i = 0; i < totalNotes; i++) {
    int currentNote = songNotes[i];
    int currentDuration = noteDurations[i];
    
    // --- Calculate the LED index based on its physical key position ---
    // This formula calculates the starting LED for a given piano key.
    long ledIndex = (currentNote - STARTING_NOTE) * LEDS_PER_KEY;

    // Make sure the calculated LED index is within the bounds of your strip
    if (ledIndex >= 0 && ledIndex < NUM_LEDS) {
      // Turn on the first LED for the key (Gold)
      leds[ledIndex] = CRGB::Green;
      
      // Turn on the second LED for the key to give it width,
      // but only if it's not the very last LED on the strip.
      if (ledIndex + 1 < NUM_LEDS) {
        leds[ledIndex + 1] = CRGB::Green;
      }
      
      FastLED.show();
      
      // Hold the note for its duration
      delay(currentDuration);
      
      // Turn off both LEDs for the key
      leds[ledIndex] = CRGB::Black;
      if (ledIndex + 1 < NUM_LEDS) {
        leds[ledIndex + 1] = CRGB::Black;
      }
      FastLED.show();

      // A short pause between notes
      delay(50); 
    }
  }

  // Wait for 2 seconds before repeating the song
  delay(2000);
}

