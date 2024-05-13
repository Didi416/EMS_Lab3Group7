#include "arduino.h"
#include <Wire.h>              // for I2C
#include <LiquidCrystal_I2C.h> // the LCD display over I2C
#include <SPI.h> //communication with SPI devices
#include "pedometer.h"
#define SS_PIN 45
#define RST_PIN 29  

double xValue = 0;
double yValue = 0;
double zValue = 0;
const int testReadX = A0;
const int testReadY = A1;
const int testReadZ = A2;

Pedometer pedometer;
double time = 1;

//LiquidCrystal_I2C lcd(0x27, 18, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pedometer = Pedometer(0, 1, 2); // Initialize with sensor pins
  // lcd.begin(16, 2); //Start and initialize the lcd display
  // lcd.clear(); //Clear any text from the screen
  // lcd.backlight();
  // lcd.setCursor(0,1);
  // lcd.print("Starting...");
}

void loop() {
  // put your main code here, to run repeatedly:
  xValue = analogRead(testReadX);
  yValue = analogRead(testReadY);
  zValue = analogRead(testReadZ);

// //  Serial.print("x-axis = ");
//   Serial.println(xValue);
// //  Serial.print("y-axis = ");
//   Serial.println(yValue);
// //  Serial.print("z-axis = ");
//   Serial.println(zValue);
//   delay(2);

  // lcd.clear(); //Clear any text from the screen
  // lcd.setCursor(0,0);
  pedometer.getAxisData(xValue, yValue, zValue);
  // Serial.print(xValue);
  // Serial.print(yValue);
  // Serial.print(zValue);
  pedometer.stepAlgorithm(xValue, yValue, zValue);

  // lcd.setCursor(0,1);
  // lcd.print(xValue);
  // lcd.setCursor(4,1);
  // lcd.print(yValue);
  // lcd.setCursor(8,1);
  // lcd.print(zValue);
  // delay(100);

delay(10);

}