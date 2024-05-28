#include <Arduino.h>
#include "pedometer.h"
#include "stpin.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "calibration.h"

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

// Pedometer pedometer;
Pedometer pedometer(0, 1, 2); // Initialize with sensor pins A0, A1, and A2
Stpin stpin;

unsigned long currentStepTime;
unsigned long previousStepTime;
unsigned long currentBlinkTime;
unsigned long previousBlinkTime;
double timeInterval = 0;
int stepCount;
int prevStepCount;
double cadence;
String paceIdentification;
unsigned long ledState = 0;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  stpin = Stpin();
  
  //LEDs
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);

  //Step Pace Identification
  previousStepTime = millis();
  previousBlinkTime = millis();
  prevStepCount = 0;
  cadence = 1.0;
  paceIdentification = "Stationary";

  //Button1
  pinMode(2, OUTPUT); // Either 5V to gate or 0V to gate  
  pinMode(7, INPUT);  // Button is read in from this pin

  //Button2
  pinMode(8, INPUT); // Reset button
  
  // Read calibration data from EEPROM
  readEEPROM();
}

void loop() {

  // Check if the calibration button is pressed
  if (digitalRead(7) == LOW) { // check the button pls
    calibrate();
  }

  xValue = getCalibratedReading('X');
  yValue = getCalibratedReading('Y');
  zValue = getCalibratedReading('Z');
  
  lcd.setCursor(0, 0);
  lcd.print("Steps: ");
  lcd.setCursor(7,0);
  currentStepTime = currentBlinkTime = millis();
  
  pedometer.getAxisData(xValue,yValue,zValue);  
  stepCount = int(pedometer.stepAlgorithm(xValue, yValue, zValue)); //determines if a step has been taken based on axis 

  if(prevStepCount != stepCount){
    timeInterval = (currentStepTime - previousStepTime)/2;
    cadence = 60.0/(timeInterval/1000);
    Serial.println(timeInterval);
    Serial.println(cadence);
    pedometer.paceIdentification(cadence, paceIdentification, ledState);
    prevStepCount = stepCount;
    previousStepTime = currentStepTime;
    lcd.clear();
  }
  lcd.print(stepCount);
  lcd.setCursor(0,1);
  lcd.print(paceIdentification);

  if((currentBlinkTime - previousBlinkTime) > ledState && paceIdentification != "Stationary/Slow Pace"){
    pedometer.paceBlink(ledState, previousBlinkTime);
  }
  else if(paceIdentification == "Stationary/Slow Pace"){
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
  }
  
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