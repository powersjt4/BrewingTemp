// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdint.h>
#include <TouchScreen.h>



#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#ifndef USE_ADAFRUIT_SHIELD_PINOUT 
 #error "This sketch is intended for use with the TFT LCD Shield. Make sure that USE_ADAFRUIT_SHIELD_PINOUT is #defined in the Adafruit_TFTLCD.h library file."
#endif

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define ONE_WIRE_BUS 2

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//DeviceAddress brewTherm = {0x28, 0x9E, 0xBD, 0x59, 0x07, 0x00, 0x00, 0x2E}; // Short Probe

DeviceAddress brewTherm = {0x28, 0xFF, 0x17, 0x82, 0xC2, 0x15, 0x01, 0xEA};// Long Probe

int relayPin = 3;
int setTemp = 70;
int tempF;
float tempC;
int oldSet;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

  uint16_t identifier = tft.readID();


void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode (relayPin, OUTPUT);

  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  uint16_t identifier = tft.readID();
  sensors.setResolution(brewTherm, 10);
  tft.begin(identifier);
  testFillScreen();
  tft.fillRect(0, 200, 400, 60,YELLOW);
  tft.fillRect(0, 260, 400, 60,BLUE);

}

void loop() {

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);
  
  sensors.requestTemperatures();
  printTemperature(brewTherm);
  if(setTemp != oldSet){
  tft.fillRect(0, 40, 400, 40,BLACK);
  tft.setCursor(0, 40);
  tft.setTextSize(5); tft.setTextColor(YELLOW);
  tft.println(setTemp);
  }
  //delay(250); 
  
  Serial.print("\n\r");
  digitalWrite (relayPin, LOW);
  Serial.print("*******************************************\n\r");
  relayControl(brewTherm);
  tft.setTextColor(YELLOW); tft.setTextSize(5);
  tft.setCursor(0, 20);
  Serial.println(setTemp);
  

  setTemperature();

}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  return micros() - start;
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempOld = tempC;
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    Serial.print("C: ");
    Serial.print(tempC);
    Serial.print(" F: ");
    Serial.print(DallasTemperature::toFahrenheit(tempC));
    if(tempOld != tempC){
    tft.fillRect(0, 0, 400, 40,BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(5); tft.setTextColor(GREEN);
    tft.println(DallasTemperature::toFahrenheit(tempC));
    }
  }
}


void relayControl(DeviceAddress deviceAddress)
{

  tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
  tempF = (DallasTemperature::toFahrenheit(tempC));
  if(tempF<setTemp )
  { 
  //delay (250);
  digitalWrite (relayPin, HIGH);
  Serial.println("Relay is on\n\r");
  }else{
  //delay(250);
  digitalWrite (relayPin, LOW);
  Serial.println("Relay is off\n\r");  
  }
    }
}

void setTemperature()
{   oldSet=setTemp;
    TSPoint p = ts.getPoint();
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    Serial.print("X = "); Serial.print(p.x);
    Serial.print("\tY = "); Serial.print(p.y);
    Serial.print("\tPressure = "); Serial.println(p.z);
  }
  if(p.y > 260 && p.y>=200 && p.z>0){
    setTemp++;
  }else if (p.y<260 && p.z>0){
   setTemp--;
 }else{
  p.y=0;  
 }
   Serial.println(" This is tempF: ");Serial.print(tempF); Serial.print("\n\r");Serial.println("This is setTemp: " ); Serial.print(setTemp);Serial.print("\n\r");;
}


