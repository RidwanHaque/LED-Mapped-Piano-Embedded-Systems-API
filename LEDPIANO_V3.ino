#include <FastLED.h>
#include <MIDI.h>

// ---- User Settings ----
#define DATAPIN    2      // The ESP32 pin connected to the LED strip's data line
#define NUM_LEDS   144
#define BRIGHTNESS 10     // Brightness from 0-255. Be careful not to set this too high.
#define BAUDRATE   9600   // Must match the baud rate in Hairless
#define NOTE_START 21     // The first MIDI note (A0)


#define VOLTS      5
#define MAX_AMPS   3500 // 500mA is the limit for a standard USB 2.0 port

// ----

// Continuous Controller Numbers for changing colors via MIDI
static const uint8_t redCC = 21, greenCC = 22, blueCC = 23;

// Settings
static const uint8_t Pin = DATAPIN, NumLEDs = NUM_LEDS, minNote = NOTE_START, maxBright = BRIGHTNESS;
const int maxNote = minNote + NumLEDs;

// LED Color Values
uint8_t rVal = 22, gVal = 0, bVal = 222;

// Build the LED strip and state array
CRGB leds[NumLEDs];
boolean ledState[NumLEDs];

// Create the MIDI instance for the main Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

// --- MIDI Handler Functions ---

void handleNoteOn(byte channel, byte note, byte velocity) {
  if (velocity == 0) { // Some keyboards send Note On with velocity 0 for Note Off
    handleNoteOff(channel, note, velocity);
    return;
  }
  // ---- MODIFICATION START ----
  // Added debugging prints to see incoming data in the Serial Monitor
  Serial.print("Note On Received: ");
  Serial.print(note);
  
  // Corrected the mapping for your 49-key keyboard (notes 36 to 84)
  long myNum = map(note, 36, 84, 0, NUM_LEDS - 1);

  Serial.print("  >> Mapped to LED Index: ");
  Serial.println(myNum);
  // ---- MODIFICATION END ----

  if (myNum >= 0 && myNum < NumLEDs) {
    ledON(myNum);
    if (myNum + 1 < NumLEDs) ledON(myNum + 1);
  }
  show();
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  // ---- MODIFICATION START ----
  // Corrected the mapping for your 49-key keyboard (notes 36 to 84)
  long myNum = map(note, 36, 84, 0, NUM_LEDS - 1);
  // ---- MODIFICATION END ----

  if (myNum >= 0 && myNum < NumLEDs) {
    ledOFF(myNum);
    if (myNum + 1 < NumLEDs) ledOFF(myNum + 1);
  }
  show();
}

void handleControlChange(byte channel, byte number, byte value) {
  switch (number) {
    case redCC:
      rVal = map(value, 0, 127, 0, 255);
      redraw();
      break;
    case greenCC:
      gVal = map(value, 0, 127, 0, 255);
      redraw();
      break;
    case blueCC:
      bVal = map(value, 0, 127, 0, 255);
      redraw();
      break;
  }
}

// --- Empty Handlers to Catch and Ignore Unused MIDI Messages ---

void handleAfterTouchChannel(byte channel, byte pressure) {}
void handleAfterTouchPoly(byte channel, byte note, byte pressure) {}
void handleProgramChange(byte channel, byte program) {}
void handlePitchBend(byte channel, int bend) {}
void handleSystemExclusive(byte* array, unsigned size) {}
void handleTimeCodeQuarterFrame(byte data) {}
void handleSongPosition(unsigned int beats) {}
void handleSongSelect(byte songnumber) {}
void handleTuneRequest() {}
void handleClock() {}
void handleStart() {}
void handleContinue() {}
void handleStop() {}
void handleSystemReset() {}

// --- LED Control Functions ---

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
      setLED(i, rVal, gVal, bVal);
    }
  }
  show();
}

void show() {
  FastLED.show();
}

// --- Main Setup and Loop ---

void setup() {
  Serial.begin(BAUDRATE);
  FastLED.addLeds<WS2812B, Pin, GRB>(leds, NumLEDs);


  // --- Power regulation added ---
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);


  FastLED.setBrightness(maxBright);
  FastLED.show();

  // Connect MIDI handlers
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);

  // Connect the empty handlers to ignore other message types
  MIDI.setHandleAfterTouchChannel(handleAfterTouchChannel);
  MIDI.setHandleAfterTouchPoly(handleAfterTouchPoly);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.setHandlePitchBend(handlePitchBend);
  MIDI.setHandleSystemExclusive(handleSystemExclusive);
  MIDI.setHandleTimeCodeQuarterFrame(handleTimeCodeQuarterFrame);
  MIDI.setHandleSongPosition(handleSongPosition);
  MIDI.setHandleSongSelect(handleSongSelect);
  MIDI.setHandleTuneRequest(handleTuneRequest);
  MIDI.setHandleClock(handleClock);
  MIDI.setHandleStart(handleStart);
  MIDI.setHandleContinue(handleContinue);
  MIDI.setHandleStop(handleStop);
  MIDI.setHandleSystemReset(handleSystemReset);
  
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  MIDI.read();
}

