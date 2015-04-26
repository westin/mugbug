#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

// thermometer vars
#define THERMOMETER A2
OneWire oneWire(THERMOMETER);
DallasTemperature sensors(&oneWire);

//lightring vars
#define LIGHT_RING 6
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, LIGHT_RING, NEO_GRB + NEO_KHZ800);


// color vars
uint32_t red = ring.Color(241, 81, 35);
uint32_t blue = ring.Color(126, 196, 243);
uint32_t green = ring.Color(72, 186, 123);

void setup(void)
{
  Serial.begin(9600);
  
  // thermometer setup
  sensors.begin();
  
  // lightring setup
  ring.begin();
  ring.setBrightness(34);
  ring.show(); // Initialize all pixels to 'off'
}


void loop(void)
{ 
  // temperature requesting
//  Serial.print("Requesting temperatures...");
//  sensors.requestTemperatures();
//  Serial.println("DONE");
//  
//  Serial.print("Temperature is: ");
//  Serial.print(sensors.getTempFByIndex(0));
  
//  colorWipe(ring.Color(255, 0, 0), 50);
//  colorWipe(ring.Color(255, 0, 255), 50);
  setRingToPercentage(red, 50, blue);
  setRingToPercentage(blue, 50, green);
  setRingToPercentage(green, 50, red);
}

void setRingToPercentage(uint32_t color,uint8_t percentage, uint32_t off_color) {
  int lights = round(ring.numPixels() * (percentage * .01));
  for(uint16_t i=0; i<lights; i++) {
        ring.setPixelColor(i, color);
        ring.show();
        delay(100);
  }
  for(uint16_t i=lights; i<ring.numPixels(); i++) {
        ring.setPixelColor(i, off_color);
        ring.show();
        delay(100);
  }
}

//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<ring.numPixels(); i++) {
//      ring.setPixelColor(i, c);
//      ring.show();
//      delay(wait);
//  }
//}
