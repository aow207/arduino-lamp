int PIN = 3;
int totalLEDs = 11;
int ledFadeTime = 5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(totalLEDs, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  rgbFadeInAndOut(0, 0, 255, ledFadeTime); // Blue
}

void rgbFadeInAndOut(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
  for(uint8_t b = 0; b <255; b++) {
     for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, red * b/255, green * b/255, blue * b/255);
     }

     strip.show();
     delay(wait);
  };

  for(uint8_t b=255; b > 0; b--) {
     for(uint8_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, red * b/255, green * b/255, blue * b/255);
     }
     strip.show();
     delay(wait);
  };
};
