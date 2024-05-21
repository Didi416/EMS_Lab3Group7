#include <Arduino.h>
#include "pedometer.h"
#include "stpin.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "ADXL335.h"

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

int zero_x; //These variables will store the calibration values for the X, Y, and Z axes.
int zero_y;
int zero_z;
int max_x, max_y, max_z; // These variables are used to store the maximum values (when experiencing 1g acceleration) for each axis. 
float sensitivity;
ADXL335 accelerometer;

// Pedometer pedometer;
Pedometer pedometer(0, 1, 2); // Initialize with sensor pins
Stpin stpin;

void calibrateADXL(int* _x, int* _y, int* _z) { // loops takes pointers to _x_y_z as arguments
    int x, y, z;
    x = xValue;
    y = yValue;
    z = zValue;
    //int sum_x, sum_y, sum_z;
    accelerometer.getXYZ(&x, &y, &z);
    float ax, ay, az;
    accelerometer.getAcceleration(&ax, &ay, &az); // reads sensor data from adxl and checks if its within range
    if ((abs(ax) < 0.06) && (abs(ay) < 0.06)) {

        *_x = x;
        *_y = y;
        *_z = z;
    } else if ((abs(ax) < 0.06) && (abs(az) < 0.06)) {
        *_x = x;
        *_y = y;
        *_z = z;
    } else if ((abs(az) < 0.06) && (abs(ay) < 0.06)) {
        *_x = x;
        *_y = y;
        *_z = z;
    } else {
        Serial.println("calibrate error");
    }
}



void setup() {
  
  analogReference(EXTERNAL);
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

  //Button1
  pinMode(2, OUTPUT); // Either 5V to gate or 0V to gate  
  pinMode(7, INPUT);  // Button is read in from this pin

  //Button2
  pinMode(8, INPUT); // Reset button


    
     accelerometer.begin();
    int x, y, z;
    for (int i = 0; i < 20; i ++) {
        accelerometer.getXYZ(&x, &y, &z);
    }
    Serial.println("The calibration starts: ");
    Serial.println("First, make sure that Z-axis direction is straight up");
    Serial.println("press button to contuine");
    while (Serial.available() == 0);
    delay(100);
    while (Serial.available() > 0) {
        Serial.read();
    }
    calibrateADXL(&x, &y, &z);
    zero_x = x;
    zero_y = y;
    max_z = z;
    Serial.println("Second, make sure that X-axis direction is straight up");
    Serial.println("press button to contuine");
    while (Serial.available() == 0);
    delay(100);
    while (Serial.available() > 0) {
        Serial.read();
    }
    calibrateADXL(&x, &y, &z); // inbuild arduino function
    zero_z = z;
    float zero_xv, zero_yv, zero_zv;
    zero_xv = (float)zero_x * ADC_REF / ADC_AMPLITUDE;
    zero_yv = (float)zero_y * ADC_REF / ADC_AMPLITUDE;
    zero_zv = (float)zero_z * ADC_REF / ADC_AMPLITUDE;
    sensitivity = (float)(max_z - zero_z) * ADC_REF / ADC_AMPLITUDE;
    lcd.print("ZERO_X = ");
    Serial.println(zero_xv);
    lcd.print("ZERO_Y = ");
    Serial.println(zero_yv);
    lcd.print("ZERO_Z = ");
    Serial.println(zero_zv);
    lcd.print("SENSITIVITY = ");
    Serial.println(sensitivity, 2);
    Serial.println("please modified the macro definitions with these results in ADXL335.h");
}


void loop() {
<<<<<<< HEAD
  xValue = analogRead(testReadX);
  yValue = analogRead(testReadY);
  zValue = analogRead(testReadZ);
  
=======

>>>>>>> 4e07ed748fbb05fafa01942c1f4886a749d14213
  lcd.setCursor(0, 0);
  lcd.print("Steps: ");
  lcd.setCursor(7,0);  
  lcd.print(int(pedometer.stepAlgorithm(xValue, yValue, zValue))); //determines if a step has been taken based on axis 
  
  pedometer.stepAlgorithm(xValue, yValue, zValue);

  stpin.stControl(lcd);
  

<<<<<<< HEAD
delay(10);

  
}

=======
  delay(10);
}
>>>>>>> 4e07ed748fbb05fafa01942c1f4886a749d14213
