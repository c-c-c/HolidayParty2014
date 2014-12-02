#include <Adafruit_NeoPixel.h>

#define N_PIXELS  60  // Number of pixels you are using
#define LED_PIN   2  // NeoPixel LED strand is connected to GPIO #0 / D0
#define SENSOR_PIN   3  // pressure sensor

Adafruit_NeoPixel  strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

//int sensorPin = 3;    // select the input pin for the potentiometer
//int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(LED_PIN, OUTPUT);  
  
//  Serial.begin(9600);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(SENSOR_PIN);
//  Serial.println(sensorValue) ;
  uint8_t val = uint8_t(constrain(sensorValue * 0.249, 0, 255));
  strip.setBrightness(val);
  uint16_t i;
  for(i=0; i< strip.numPixels(); i++) {
    uint32_t col = strip.Color(constrain(sin(val), 0, 255), constrain(1/val, 0, 255), val);
    strip.setPixelColor(i,col);
  }
  strip.show(); // Update strip 
}
