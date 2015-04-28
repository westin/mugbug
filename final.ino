#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

// thermometer vars
#define THERMOMETER A3
OneWire oneWire(THERMOMETER);
DallasTemperature sensors(&oneWire);

//lightring vars
#define LIGHT_RING 13
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, LIGHT_RING, NEO_GRB + NEO_KHZ800);

// pententiometer vars
#define PENT A1
int pentVal = 0;
int pentPercentage = 0;

// color vars
int red_arr[3] = {241, 81, 35};
uint32_t red = ring.Color(241, 81, 35);
uint32_t red2 = ring.Color(241 * .2 , 81 * .2 , 35 * .2 );

uint32_t green = ring.Color(13,207,139);

int blue_arr[3] = {126, 196, 243};
uint32_t blue = ring.Color(126, 196, 243);
uint32_t blue2 = ring.Color(126 * .4, 196 * .4, 243 * .4);

uint32_t dull_white = ring.Color(0,0,0);

uint32_t fade0 = ring.Color(127, 0, 0);
uint32_t fade1 = ring.Color(147,25,5);
uint32_t fade2 = ring.Color(166,51,10);
uint32_t fade3 = ring.Color(186,76,15);
uint32_t fade4 = ring.Color(206,101,21);
uint32_t fade5 = ring.Color(225,127,26);
uint32_t fade6 = ring.Color(245,152,31);
uint32_t fade7 = ring.Color(235,156,49);
uint32_t fade8 = ring.Color(196,130,78);
uint32_t fade9 = ring.Color(157,104,108);
uint32_t fade10 = ring.Color(117,78,137);
uint32_t fade11 = ring.Color(98,65,152);
uint32_t fade12 = ring.Color(59,39,181);
uint32_t fade13 = ring.Color(20,13,210);
uint32_t fade14 = ring.Color(0,0,225);


// general vars
int pentReading;
unsigned long time;
unsigned long switchTime;
uint32_t userColor = ring.Color(235,156,49);
int userPixel = 7;
boolean performedFlurish = false;
float currTemp;
float desiredTemp;

void setup(void)
{
  Serial.begin(9600);
  switchTime = millis();
  int pentReading = findPetentiometerPercentage();
  // thermometer setup
  sensors.begin();
  sensors.requestTemperatures();
  currTemp = sensors.getTempFByIndex(0);
  
  // lightring setup
  ring.begin();
  ring.setBrightness(15);
  ring.show();

}


void loop(void)
{ 
  time = millis();
//   Serial.println((time - switchTime), DEC);
//  Serial.println(round(desiredTemp/currTemp) * 100);
  if(abs(findPetentiometerPercentage() - pentReading) > 2){
    pentReading = findPetentiometerPercentage();
    tempSelection();
    switchTime = millis();
    performedFlurish = false;
  }
  else if((time - switchTime) > 1500){
    if(performedFlurish == false){
      
      sensors.requestTemperatures();
      currTemp = sensors.getTempFByIndex(0);
      
      fillPixelsFrom(userPixel, userColor);
      setRingToPercentage(ring.Color(30,0,0), 100 - round((desiredTemp - 100)/(currTemp - 100) * 100), userColor, 30);
      performedFlurish = true;
    }
   if((time % 5000) == 0){
//      Serial.println("Requesting temperatures...");
      sensors.requestTemperatures();
      currTemp = sensors.getTempFByIndex(0);
    //  Serial.println("DONE");
    //  pulseSection(getLightAmountFor(70), ring.numPixels(), blue_arr);
      Serial.print("Temperature is: ");
//      currTemp = sensors.getTempFByIndex(0);
      Serial.println(currTemp, DEC);
      Serial.print("Desired is: ");
      Serial.println(desiredTemp, DEC);
      Serial.print("Rounded that makes: ");
      Serial.println(round((desiredTemp - 100)/(currTemp - 100) * 100));
      setRingToPercentage(ring.Color(30,0,0), 100 - round((desiredTemp - 100)/(currTemp - 100) * 100), userColor, 30);
   }
  }
  // temperature requesting
//  Serial.println("Requesting temperatures...");
//  sensors.requestTemperatures();
////  Serial.println("DONE");
////  pulseSection(getLightAmountFor(70), ring.numPixels(), blue_arr);
//  Serial.print("Temperature is: ");
//  Serial.print(sensors.getTempFByIndex(0));

    // fade blue
//  pulseSection(getLightAmountFor(90), ring.numPixels(), blue_arr);
   
    // fade red
//  pulseSection(0, getLightAmountFor(90), red_arr);
//    tempSelection();
//  colorWipe(ring.Color(255, 0, 0), 50);
//  colorWipe(ring.Color(255, 0, 255), 50);
//  setRingToPercentage(red, findPetentiometerPercentage(), blue, 0);
//  setRingToPercentage(blue, 50, red);
//  setRingToPercentage(green, 50, red);
//  dimColor(red, 10);
}

int findPetentiometerPercentage()
{
    pentVal = analogRead(PENT);
    pentPercentage = (pentVal / 10.22);
    return pentPercentage;
}

int getLightAmountFor(uint8_t percentage)
{
  int lit = round(ring.numPixels() * (percentage * .01));
  return lit;
}

int pulseSection(int from, int to, int color[3])
{
  int colorScale = 70;
  for(uint16_t i=100; i>=colorScale; i = i - 3) {
    for(uint16_t j=from; j<to; j++) {
      ring.setPixelColor(j, ring.Color(color[0] * (i * .01), color[1] * (i * .01), color[2] * (i * .01)));
      ring.show();
    }
    delay(20);
  }
  for(uint16_t i=colorScale; i<=100; i = i + 3) {
    for(uint16_t j=from; j<to; j++) {
      ring.setPixelColor(j, ring.Color(color[0] * (i * .01), color[1] * (i * .01), color[2] * (i * .01)));
      ring.show();
    }
    delay(30);
  }
}

void setRingToPercentage(uint32_t color,uint8_t percentage, uint32_t off_color, uint8_t wait) 
{
  int lights = round(ring.numPixels() * (percentage * .01));
  for(uint16_t i=0; i<lights; i++) {
        ring.setPixelColor(i, color);
        ring.show();
        delay(wait);
  }
  for(uint16_t i=lights; i<ring.numPixels(); i++) {
        ring.setPixelColor(i, off_color);
        ring.show();
        delay(wait);
  }
}

void tempSelection()
{
  int perc = findPetentiometerPercentage();
  if(perc < 6){
    ring.setPixelColor(1, fade14);
    colorAllPixelsBut(1, dull_white);
    userColor = fade14;
    userPixel = 1;
    desiredTemp = 113;
    
  }
  if(perc > 6 && perc < 12){
    ring.setPixelColor(2, fade13);
    colorAllPixelsBut(2, dull_white);
    userColor = fade13;
    userPixel = 2;
    desiredTemp = 117;
  }
  if(perc > 12 && perc < 20){
    ring.setPixelColor(3, fade12);
    colorAllPixelsBut(3, dull_white);
    userColor = fade12;
    userPixel = 3;
    desiredTemp = 121;
  }
  if(perc > 20 && perc < 26){
    ring.setPixelColor(4, fade11);
    colorAllPixelsBut(4, dull_white);
    userColor = fade11;
    userPixel = 4;
    desiredTemp = 125;
  }
  if(perc > 26 && perc < 32){
    ring.setPixelColor(5, fade10);
    colorAllPixelsBut(5, dull_white);
    userColor = fade10;
    userPixel = 5;
    desiredTemp = 130;
  }
  if(perc > 32 && perc < 40){
    ring.setPixelColor(6, fade9);
    colorAllPixelsBut(6, dull_white);
    userColor = fade9;
    userPixel =6;
    desiredTemp = 135;
  }
  if(perc > 40 && perc < 47){
    ring.setPixelColor(7, fade8);
    colorAllPixelsBut(7, dull_white);
    userColor = fade8;
    userPixel = 7;
    desiredTemp = 140;
  }
  if(perc > 47 && perc < 53){
    ring.setPixelColor(8, fade7);
    colorAllPixelsBut(8, dull_white);
    userColor = fade7;
    userPixel = 8;
    desiredTemp = 144;
  }
  if(perc > 53 && perc < 60){
    ring.setPixelColor(9, fade6);
    colorAllPixelsBut(9, dull_white);
    userColor = fade6;
    userPixel = 9;
    desiredTemp = 148;
  }
  if(perc > 60 && perc < 66){
    ring.setPixelColor(10, fade5);
    colorAllPixelsBut(10, dull_white);
    userColor = fade5;
    userPixel = 10;
    desiredTemp = 155;
  }
  if(perc > 66 && perc < 72){
    ring.setPixelColor(11, fade4);
    colorAllPixelsBut(11, dull_white);
    userColor = fade4;
    userPixel = 11;
    desiredTemp = 160;
  }
  if(perc > 72 && perc < 78){
    ring.setPixelColor(12, fade3);
    colorAllPixelsBut(12, dull_white);
    userColor = fade3;
    userPixel = 12;
    desiredTemp = 164;
  }
  if(perc > 78 && perc < 86){
    ring.setPixelColor(13, fade2);
    colorAllPixelsBut(13, dull_white);
    userColor = fade2;
    userPixel = 13;
    desiredTemp = 168;
  }
  if(perc > 86 && perc < 92){
    ring.setPixelColor(14, fade1);
    colorAllPixelsBut(14, dull_white);
    userColor = fade1;
    userPixel = 14;
    desiredTemp = 172;
  }
  if(perc > 92){
    ring.setPixelColor(15, fade0);
    colorAllPixelsBut(15, dull_white);
    userColor = fade0;
    userPixel = 15;
    desiredTemp = 176;
  }
  ring.show();
}

void colorAllPixelsBut(int but, uint32_t c) {
  for(uint16_t i=0; i<ring.numPixels(); i++) {
    if(i != but){
      ring.setPixelColor(i, c);
      ring.show();
    }
  }
}

void fullColor(uint32_t c) {
  for(uint16_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, c);
      ring.show();
  }
}

void fillPixelsFrom(int start, uint32_t color)
{
  for(uint16_t i = start; i < 16; i++) {
      ring.setPixelColor(i, color);
      ring.show();
      delay(30);
  }
}
