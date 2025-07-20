#include <FastLED.h>

// ---- User Settings ----
#define DATAPIN 2
#define NUMLEDS 100
#define BRIGHTNESS 30
#define BAUDRATE 9600 // For Hairless
#define NOTESTART 10
// ----

// Continuous Controller Numbers
static const uint8_t
redCC = 21,
greenCC = 22,
blueCC = 23;

// Settings
static const uint8_t
Pin = DATAPIN,
NumLEDs = NUMLEDS,
minNote = NOTESTART,
maxNote = minNote + NumLEDs,
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
#include <MIDI.h>
// #include <MIDI.hpp>
struct CustomBaud : public midi::DefaultSettings {
    static const long BaudRate = BAUDRATE;
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, CustomBaud);

void setup() {
  FastLED.addLeds<WS2812B, Pin, GRB>(leds, NumLEDs);
  FastLED.setBrightness(maxBright);
  FastLED.show(); //I think this goes here
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);

}



void loop() {
  MIDI.read(); // Check for MIDI messages every loop
}





void handleNoteOn(byte channel, byte note, byte velocity) {
  // Check if note is in range
  // if(note < minNote || note > maxNote){
  //   return;
  // }

  int myNum = round((note - minNote) * 0.83);
  ledON(myNum);
  ledON(myNum + 1);
  // ledON(myNum - 1);
  show();
}


void handleNoteOff(byte channel, byte note, byte velocity) {
  // Check if note is in range
  // if(note < minNote || note > maxNote){
  //   return;
  // }

  int myNum = round((note - minNote)*0.83);
  ledOFF(myNum);
  ledOFF(myNum + 1);
  // ledOFF(myNum - 1);
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

