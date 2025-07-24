#include <FastLED.h>
#include <MIDI.h>

// ---- User Settings ----
#define DATAPIN       2
#define NUMLEDS       144  // Total LEDs on the physical strip
#define BRIGHTNESS    10
#define BAUDRATE      9600
#define NOTESTART     36   // Keystation 49 MK3's lowest note

// --- YOUR CUSTOM SETTING ---
#define LEDS_ON_PIANO 95   // The number of LEDs covering your piano keys
// ---------------------------

// --- Define your piano's specific MIDI note range ---
const int PIANO_KEY_COUNT = 49;
const int PIANO_HIGHEST_NOTE = NOTESTART + PIANO_KEY_COUNT - 1; // This will be 84

// Continuous Controller Numbers
static const uint8_t
  redCC = 21,
  greenCC = 22,
  blueCC = 23;

// Settings
static const uint8_t
  Pin = DATAPIN,
  NumLEDs = NUMLEDS,
  maxBright = BRIGHTNESS;

// LED Color Values
uint8_t
  rVal = 22,
  gVal = 0,
  bVal = 222;

// Build the LED strip
CRGB leds[NumLEDs];
boolean ledState[NumLEDs];

// Create the MIDI Instance
struct CustomBaud : public midi::DefaultSettings {
  static const long BaudRate = BAUDRATE;
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, CustomBaud);

void setup() {
  FastLED.addLeds<WS2812B, Pin, GRB>(leds, NumLEDs);
  FastLED.setBrightness(maxBright);
  FastLED.show();
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  MIDI.read();
}

// --- CORRECTED SCALING LOGIC ---
void handleNoteOn(byte channel, byte note, byte velocity) {
  // Check if the note is from our piano keyboard
  if (note >= NOTESTART && note <= PIANO_HIGHEST_NOTE) {
    // If it is, map its range [36..84] to the LED range [0..94]
    int ledIndex = map(note, NOTESTART, PIANO_HIGHEST_NOTE, 0, LEDS_ON_PIANO - 1);
    ledON(ledIndex);
    show();
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  // Use the exact same mapping logic to turn the LED off
  if (note >= NOTESTART && note <= PIANO_HIGHEST_NOTE) {
    int ledIndex = map(note, NOTESTART, PIANO_HIGHEST_NOTE, 0, LEDS_ON_PIANO - 1);
    ledOFF(ledIndex);
    show();
  }
}
// --- END OF CORRECTIONS ---

void handleControlChange(byte channel, byte number, byte value) {
  switch (number) {
    case redCC: rVal = map(value, 0, 127, 0, 255); redraw(); break;
    case greenCC: gVal = map(value, 0, 127, 0, 255); redraw(); break;
    case blueCC: bVal = map(value, 0, 127, 0, 255); redraw(); break;
  }
}

void ledON(uint8_t index) {
  ledState[index] = 1;
  setLED(index, rVal, gVal, bVal);
}

void ledOFF(uint8_t index) {
  ledState[index] = 0;
  setLED(index, 0, 0, 0);
}

void setLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  leds[index].r = r;
  leds[index].g = g;
  leds[index].b = b;
}

void redraw() {
  for (int i = 0; i < NumLEDs; i++) {
    if (ledState[i] == 1) {
      ledON(i);
    }
  }
  show();
}

void show() {
  FastLED.show();
}
