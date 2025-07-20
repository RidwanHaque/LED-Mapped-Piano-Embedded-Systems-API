// Include the Adafruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// --- CONFIGURATION ---
#define LED_PIN     16  // The GPIO pin on your ESP32 connected to the Data In pin of the strip
#define LED_COUNT   144 // The number of LEDs on your strip (3.2ft at 144/m is ~144)
#define BRIGHTNESS  60  // Set brightness to a value from 0 (off) to 255 (full).
                        // IMPORTANT: Start with a low value (e.g., 50) to limit current draw!

// Declare our NeoPixel strip object.
// Argument 1: Number of pixels in the strip
// Argument 2: ESP32 pin number
// Argument 3: Pixel type flags (NEO_GRB + NEO_KHZ800 is standard for WS2812B)
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Start the serial communication for debugging purposes (optional)
  Serial.begin(115200);

  strip.begin();                // INITIALIZE the NeoPixel strip object (REQUIRED)
  strip.setBrightness(BRIGHTNESS); // Set the overall brightness for the whole strip
  strip.show();                 // Turn all pixels off (clears the buffer)
  Serial.println("NeoPixel setup complete.");
}

void loop() {
  // --- EXAMPLE 1: Fill the entire strip with a solid color ---
  Serial.println("Filling strip with Red");
  fillStrip(strip.Color(255, 0, 0)); // Red
  delay(1000); // Wait for 1 second

  Serial.println("Filling strip with Green");
  fillStrip(strip.Color(0, 255, 0)); // Green
  delay(1000); // Wait for 1 second

  Serial.println("Filling strip with Blue");
  fillStrip(strip.Color(0, 0, 255)); // Blue
  delay(1000); // Wait for 1 second

  // --- EXAMPLE 2: A chasing light effect ("color wipe") ---
  Serial.println("Performing color wipe...");
  colorWipe(strip.Color(255, 255, 0), 10); // Yellow, with a 10ms delay between pixels
  colorWipe(strip.Color(0, 0, 0), 10);     // Turn pixels off one by one

  // --- EXAMPLE 3: Rainbow cycle effect ---
  Serial.println("Performing rainbow cycle...");
  rainbow(10); // Rainbow cycle with a 10ms delay between steps
}


// --- HELPER FUNCTIONS ---

/**
 * @brief Fills the entire LED strip with a single color.
 * @param color The color to set, created with strip.Color(R, G, B).
 */
void fillStrip(uint32_t color) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color); // Set color for each pixel
  }
  strip.show(); // IMPORTANT: This sends the updated colors to the strip!
}

/**
 * @brief Fills the strip pixel by pixel with a color.
 * @param color The color to set.
 * @param wait The delay in milliseconds between each pixel lighting up.
 */
void colorWipe(uint32_t color, int wait) {
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

/**
 * @brief Displays a rainbow pattern that cycles along the strip.
 * @param wait The delay in milliseconds between each step of the cycle.
 */
void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) {
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    delay(wait);
  }
}
