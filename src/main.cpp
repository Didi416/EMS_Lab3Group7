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

bool button1Pressed = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pedometer pedometer;
Pedometer pedometer; // Initialize with sensor pins A0, A1, and A2
Stpin stpin;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  stpin = Stpin();
  pedometer = Pedometer(0, 1, 2);
  
  //LEDs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);

  pinMode(2, OUTPUT); // Either 5V to gate or 0V to gate  
  //Button1
  pinMode(7, INPUT);  // Button is read in from this pin
  //Button2
  pinMode(8, INPUT); // Reset button

  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  
}

void loop() {

  lcd.setCursor(0, 0);
  lcd.print("Steps: ");
  lcd.setCursor(7,0);
  pedometer.getAxisData(xValue,yValue,zValue);  
  lcd.print(int(pedometer.stepAlgorithm(xValue, yValue, zValue))); //determines if a step has been taken based on axis 
  
  // pedometer.stepAlgorithm(xValue, yValue, zValue);

  if (digitalRead(8) == LOW){
    pedometer.resetStepCount();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RESET");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.clear();
  }
  
  stpin.stControl(lcd);

  delay(10);
}