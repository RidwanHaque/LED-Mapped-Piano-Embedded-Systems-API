#include <FastLED.h>
#include <MIDI.h>

// ---- Corrected User Settings ----
#define DATAPIN    2
#define NUM_LEDS   144
#define BRIGHTNESS 64     // A reasonable starting brightness (0-255)
#define BAUDRATE   9600   // Must match Hairless

// Power settings for your external power supply
#define VOLTS      5
#define MAX_AMPS   3600   // 8A limit for a 10A power supply

// Continuous Controller numbers for changing colors via MIDI
const uint8_t redCC = 21, greenCC = 22, blueCC = 23;

// LED Color Values
uint8_t rVal = 22, gVal = 0, bVal = 222;

// Build the LED strip and state array
CRGB leds[NUM_LEDS];
boolean ledState[NUM_LEDS];

// Create a standard, reliable MIDI instance
MIDI_CREATE_DEFAULT_INSTANCE();

// --- Main Setup and Loop ---

void setup() {
  // CRITICAL: Start the serial port so MIDI data can be received
  Serial.begin(BAUDRATE); 
  
  // Setup FastLED
  FastLED.addLeds<WS2812B, DATAPIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // --- START OF STARTUP ANIMATION TEST ---
  // This is the known-working animation from your first program.
  // It will run once to confirm FastLED is working correctly.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Cyan;
    FastLED.show();
    delay(20); // A short delay for a quick visual test
  }
  delay(1000); // Wait a second after the test
  FastLED.clear();
  FastLED.show();
  // --- END OF STARTUP ANIMATION TEST ---

  // Now, connect the MIDI handler functions
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  
  // Tell the MIDI library to start listening on all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  MIDI.read();
}

// --- MIDI Handler Functions ---

void handleNoteOn(byte channel, byte note, byte velocity) {
  if (velocity == 0) {
    handleNoteOff(channel, note, velocity);
    return;
  }
  
  // Correctly map your 49-key keyboard (notes 36-84) to your LED strip
  long myNum = map(note, 36, 84, 0, NUM_LEDS - 1);

  if (myNum >= 0 && myNum < NUM_LEDS) {
    ledON(myNum);
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  long myNum = map(note, 36, 84, 0, NUM_LEDS - 1);

  if (myNum >= 0 && myNum < NUM_LEDS) {
    ledOFF(myNum);
  }
}

void handleControlChange(byte channel, byte number, byte value) {
  switch (number) {
    case redCC:   rVal = map(value, 0, 127, 0, 255); redraw(); break;
    case greenCC: gVal = map(value, 0, 127, 0, 255); redraw(); break;
    case blueCC:  bVal = map(value, 0, 127, 0, 255); redraw(); break;
  }
}

// --- LED Control Functions ---

void ledON(uint8_t index) {
  ledState[index] = true;
  leds[index].setRGB(rVal, gVal, bVal);
  FastLED.show();
}

void ledOFF(uint8_t index) {
  ledState[index] = false;
  leds[index] = CRGB::Black;
  FastLED.show();
}

void redraw() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (ledState[i] == true) {
      leds[i].setRGB(rVal, gVal, bVal);
    }
  }
  FastLED.show();
}
