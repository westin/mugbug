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
int red_arr[3] = {241, 81, 35};
uint32_t red = ring.Color(241, 81, 35);
uint32_t red2 = ring.Color(241 * .2 , 81 * .2 , 35 * .2 );

int blue_arr[3] = {126, 196, 243};
uint32_t blue = ring.Color(126, 196, 243);
uint32_t blue2 = ring.Color(126 * .4, 196 * .4, 243 * .4);

uint32_t green = ring.Color(72, 186, 123);

void setup(void)
{
  Serial.begin(9600);
  
  // thermometer setup
  sensors.begin();
  
  // lightring setup
  ring.begin();
  ring.setBrightness(24);
  ring.show(); // Initialize all pixels to 'off'
  setRingToPercentage(red, 90, blue);
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

    // fade blue
  pulseSection(getLightAmountFor(90), ring.numPixels(), blue_arr);
  
    // fade red
//  pulseSection(0, getLightAmountFor(90), red_arr);

//  colorWipe(ring.Color(255, 0, 0), 50);
//  colorWipe(ring.Color(255, 0, 255), 50);
//  setRingToPercentage(red, 90, blue);
//  setRingToPercentage(blue, 50, red);
//  setRingToPercentage(green, 50, red);
//  dimColor(red, 10);
}

int getLightAmountFor(uint8_t percentage)
{
  int lit = round(ring.numPixels() * (percentage * .01));
  return lit;
}

int pulseSection(int from, int to, int color[3])
{
  int colorScale = 40;
  for(uint16_t i=100; i>=colorScale; i--) {
    for(uint16_t j=from; j<to; j++) {
      ring.setPixelColor(j, ring.Color(color[0] * (i * .01), color[1] * (i * .01), color[2] * (i * .01)));
      ring.show();
    }
    delay(20);
  }
  for(uint16_t i=colorScale; i<=100; i++) {
    for(uint16_t j=from; j<to; j++) {
      ring.setPixelColor(j, ring.Color(color[0] * (i * .01), color[1] * (i * .01), color[2] * (i * .01)));
      ring.show();
    }
    delay(30);
  }
}

void setRingToPercentage(uint32_t color,uint8_t percentage, uint32_t off_color) 
{
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

void setColorFade(int pixel, int color1[3], int color2[3])
{
  int fade_color[3];
  for (int i = 0; i < 3; i++){
    fade_color[i] = (color1[i] + color2[i])/2;
  }
  ring.setPixelColor(pixel, fade_color[0], fade_color[1], fade_color[2]);
  ring.show();
}

//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<ring.numPixels(); i++) {
//      ring.setPixelColor(i, c);
//      ring.show();
//      delay(wait);
//  }
//}

//uint32_t Wheel(byte WheelPos) {
//  WheelPos = 255 - WheelPos;
//  if(WheelPos < 85) {
//   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  } else if(WheelPos < 170) {
//    WheelPos -= 85;
//   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  } else {
//   WheelPos -= 170;
//   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//  }
//}
