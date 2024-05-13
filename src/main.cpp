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

//LiquidCrystal_I2C lcd(0x27, 16, 2);

Pedometer pedometer;
double time = 1;

void setup() {
  Serial.begin(9600);
  pedometer = Pedometer(0, 1, 2); // Initialize with sensor pins

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // lcd.begin(16, 2);
  // lcd.backlight();
  // lcd.clear();
  // lcd.setCursor(0, 0);
}

void loop() {
  xValue = analogRead(testReadX);
  yValue = analogRead(testReadY);
  zValue = analogRead(testReadZ);

  // lcd.setCursor(0, 0);

  pedometer.getAxisData(xValue, yValue, zValue); //reads and returns the x, y and z values

  // lcd.print(int(pedometer.stepAlgorithm(xValue, yValue, zValue))); //determines if a step has been taken based on axis data
  pedometer.stepAlgorithm(xValue, yValue, zValue);

delay(10);

}