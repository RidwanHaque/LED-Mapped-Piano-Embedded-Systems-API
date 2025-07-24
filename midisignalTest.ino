#include <FastLED.h>
#include <MIDI.h>

// ---- Simplified Settings ----
#define DATAPIN    2
#define NUMLEDS    144
#define BRIGHTNESS 20 // Use a safe, low brightness for testing
#define BAUDRATE   9600
// ----

// Create the LED array
CRGB leds[NUMLEDS];

// Create the MIDI instance with the correct baud rate
struct CustomBaud : public midi::DefaultSettings {
    static const long BaudRate = BAUDRATE;
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, CustomBaud);


// --- Simplified MIDI Handlers ---

// This function runs whenever ANY MIDI "Note On" message is received.
void handleNoteOn(byte channel, byte note, byte velocity) {
  // Turn the very first LED (index 0) to Red.
  leds[0] = CRGB::Red;
  FastLED.show();
}

// This function runs whenever ANY MIDI "Note Off" message is received.
void handleNoteOff(byte channel, byte note, byte velocity) {
  // Turn the very first LED (index 0) off (to black).
  leds[0] = CRGB::Black;
  FastLED.show();
}


// --- Main Setup and Loop ---

void setup() {
  // Initialize the LED strip
  FastLED.addLeds<WS2812B, DATAPIN, GRB>(leds, NUMLEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  // Start with all LEDs off
  FastLED.clear();
  FastLED.show();

  // Initialize MIDI and tell it which functions to call for Note On/Off
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI); // Listen on all MIDI channels
}

void loop() {
  // Continuously check for new MIDI messages
  MIDI.read();
}
