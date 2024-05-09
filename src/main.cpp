#include "arduino.h"
#include <Wire.h>              // for I2C
#include <LiquidCrystal_I2C.h> // the LCD display over I2C
#include <SPI.h> //communication with SPI devices
#define SS_PIN 45
#define RST_PIN 29  

int xValue = 0;
int yValue = 0;
int zValue = 0;
const int testReadX = A0;
const int testReadY = A1;
const int testReadZ = A2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  xValue = analogRead(testReadX);
  yValue = analogRead(testReadY);
  zValue = analogRead(testReadZ);

//  Serial.print("x-axis = ");
  Serial.println(xValue);
//  Serial.print("y-axis = ");
  Serial.println(yValue);
//  Serial.print("z-axis = ");
  Serial.println(zValue);
  delay(2);
}