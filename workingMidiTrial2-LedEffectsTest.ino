#include <FastLED.h>
#include <MIDI.h>

// ---- User Settings ----
#define DATAPIN       2
#define NUMLEDS       144      // Total LEDs on the physical strip
#define BRIGHTNESS    20       // Global brightness cap (0-255)
#define BAUDRATE      115200   // Increased for better performance
#define NOTESTART     36       // Keystation 49 MK3's lowest note

// --- YOUR CUSTOM SETTINGS ---
#define LEDS_ON_PIANO 95       // The number of LEDs covering your piano keys
#define FADE_SPEED    25       // How fast the lights fade. Higher is faster. Try 10-40.
// ---------------------------

// --- Define your piano's specific MIDI note range ---
const int PIANO_KEY_COUNT = 49;
const int PIANO_HIGHEST_NOTE = NOTESTART + PIANO_KEY_COUNT - 1; // This will be 84

// --- Core Project Variables ---
CRGB leds[NUMLEDS];                  // The array of all physical LEDs
bool isNoteOn[LEDS_ON_PIANO] = {false}; // Tracks which notes are held down
CRGB keyColor[LEDS_ON_PIANO];        // Stores the random color for each active note

// --- MIDI Library Setup ---
struct CustomBaud : public midi::DefaultSettings {
  static const long BaudRate = BAUDRATE;
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, CustomBaud);

// --- Random Color Generation ---
CRGB getRandomColor() {
  // Returns a new, random color.
  return CRGB(random(0, 256), random(0, 256), random(0, 256));
}

// =================================================================
//  SETUP: Runs once when the board powers on
// =================================================================
void setup() {
  // Seed the random number generator for more varied colors
  randomSeed(analogRead(A0));

  // CORRECTED: Using all-caps NUMLEDS
  FastLED.addLeds<WS2812B, DATAPIN, GRB>(leds, NUMLEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(); // Start with a clean, black strip
  FastLED.show();

  // Initialize the MIDI library
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
  // Turn off the default MIDI Thru feature to stop the TX light from flashing
  MIDI.turnThruOff();

  // Tell the MIDI library which functions to call for different messages
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
}

// =================================================================
//  LOOP: Runs continuously after setup
// =================================================================
void loop() {
  MIDI.read(); // Check for incoming MIDI messages

  // Loop through all the LEDs that are on the piano
  for (int i = 0; i < LEDS_ON_PIANO; i++) {
    // If the note for this LED is NOT being held down...
    if (!isNoteOn[i]) {
      // ...then fade its color towards black.
      leds[i].nscale8(255 - FADE_SPEED);
    }
  }

  FastLED.show(); // Send the updated colors to the LED strip
}

// =================================================================
//  MIDI HANDLER: Note On
// =================================================================
void handleNoteOn(byte channel, byte note, byte velocity) {
  // Some keyboards send a "Note On" with velocity 0 instead of a "Note Off" message
  if (velocity == 0) {
    handleNoteOff(channel, note, velocity);
    return;
  }
  
  // Check if the note is within our piano's range
  if (note >= NOTESTART && note <= PIANO_HIGHEST_NOTE) {
    int ledIndex = map(note, NOTESTART, PIANO_HIGHEST_NOTE, 0, LEDS_ON_PIANO - 1);
    
    // Set the flag to TRUE for this note so it doesn't fade
    isNoteOn[ledIndex] = true;
    
    // Generate and store a new random color for this key press
    keyColor[ledIndex] = getRandomColor();
    
    // Map MIDI velocity to brightness
    uint8_t brightness = map(velocity, 1, 127, 40, 255);
    
    // Set the LED to the new random color, scaled by the key press brightness
    leds[ledIndex] = keyColor[ledIndex].nscale8(brightness);
  }
}

// =================================================================
//  MIDI HANDLER: Note Off
// =================================================================
void handleNoteOff(byte channel, byte note, byte velocity) {
  // Check if the note is within our piano's range
  if (note >= NOTESTART && note <= PIANO_HIGHEST_NOTE) {
    int ledIndex = map(note, NOTESTART, PIANO_HIGHEST_NOTE, 0, LEDS_ON_PIANO - 1);
    
    // Set the flag to FALSE for this note, which allows it to be faded in the main loop
    isNoteOn[ledIndex] = false;
  }
}
