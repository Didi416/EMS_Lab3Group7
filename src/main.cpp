#include <Arduino.h>
#include "pedometer.h"
#include "stpin.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

#define SS_PIN 45
#define RST_PIN 29  

double xValue = 0;
double yValue = 0;
double zValue = 0;
const int testReadX = A0;
const int testReadY = A1;
const int testReadZ = A2;

bool button1Pressed = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

Pedometer pedometer;
Stpin stpin;
double time = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("hi 1");

  pedometer = Pedometer(0, 1, 2); // Initialize with sensor pins
  stpin = Stpin();
  Serial.println("hi");

  // pinMode(2, OUTPUT);
  // digitalWrite(2, HIGH); // Turns off the ST routine
  // pinMode(7, INPUT);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  //LEDs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //Button1
  pinMode(2, OUTPUT); // Either 5V to gate or 0V to gate  
  pinMode(7, INPUT);  // Button is read in from this pin

  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
}

void loop() {
  xValue = analogRead(testReadX);
  yValue = analogRead(testReadY);
  zValue = analogRead(testReadZ);

  lcd.setCursor(0, 0);
  
  pedometer.getAxisData(xValue, yValue, zValue); //reads and returns the x, y and z values
  
  lcd.print(int(pedometer.stepAlgorithm(xValue, yValue, zValue))); //determines if a step has been taken based on axis data
  pedometer.stepAlgorithm(xValue, yValue, zValue);

  stpin.stControl(lcd);
  

delay(10);

}