#include "Calibration.h"
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

// Assuming an external LCD object
extern LiquidCrystal_I2C lcd;

// Calibration parameters
int axisMin[3], axisMax[3];

// Constants
const int numSamples = 20;
const unsigned long delayTime = 1000;
const unsigned long messageDisplayTime = 2000;

// Buffer for LCD messages
char lcdBuffer[32];

Calibration::Calibration(){

}

// Main calibration function
int Calibration::calibrate(){
    delay(2000);
    calibrateAxisX(axisMin[0], axisMax[0]);
    calibrateAxisY(axisMin[1], axisMax[1]);
    calibrateAxisZ(axisMin[2], axisMax[2]);

    writeEEPROM();
    
    //delay(messageDisplayTime);
    lcd.clear();
    return 1;
}

// Function to calibrate a specific axis
void Calibration::calibrateAxisX(int &minVal, int &maxVal) {
    int highSum = 0, lowSum = 0;

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("X - Calibrating");
    for (int i = 0; i < numSamples; i++) {
        lowSum += readSensor('X');
    }
    delay(delayTime);
    minVal = lowSum / numSamples;

    for (int i = 0; i < numSamples; i++) {
        highSum += readSensor('X');
    }
    delay(delayTime);
    maxVal = highSum / numSamples;

    lcd.clear();
    lcd.setCursor(0,0);

    lcd.print("XAxis Complete");
    lcd.clear();

}
void Calibration::calibrateAxisY(int &minVal, int &maxVal) {
    int highSum = 0, lowSum = 0;

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Y - Calibrating");
    for (int i = 0; i < numSamples; i++) {
        lowSum += readSensor('Y');
    }
    delay(delayTime);
    minVal = lowSum / numSamples;

    for (int i = 0; i < numSamples; i++) {
        highSum += readSensor('Y');
    }
    delay(delayTime);
    maxVal = highSum / numSamples;

    lcd.clear();
    lcd.setCursor(0,0);

    lcd.print("YAxis Complete");
    lcd.clear();

}
void Calibration::calibrateAxisZ(int &minVal, int &maxVal) {
    int highSum = 0, lowSum = 0;

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Z - Calibrating");
    for (int i = 0; i < numSamples; i++) {
        lowSum += readSensor('Z');
    }
    delay(delayTime);
    minVal = lowSum / numSamples;

    for (int i = 0; i < numSamples; i++) {
        highSum += readSensor('Z');
    }
    delay(delayTime);
    maxVal = highSum / numSamples;

    lcd.clear();
    lcd.setCursor(0,0);

    lcd.print("ZAxis Complete");
    lcd.clear();

}

// Function to read sensor value based on the specified axis
int Calibration::readSensor(char axis) {
    switch (axis) {
        case 'X': return analogRead(FILTERED_X_AXIS_PIN);
        case 'Y': return analogRead(FILTERED_Y_AXIS_PIN);
        case 'Z': return analogRead(FILTERED_Z_AXIS_PIN);
        default: return -1;
    }
}

double Calibration::getCalibratedReading(char axis) {
    int pin, minVal, maxVal;
    switch (axis) {
        case 'X': pin = FILTERED_X_AXIS_PIN; minVal = axisMin[0]; maxVal = axisMax[0]; break;
        case 'Y': pin = FILTERED_Y_AXIS_PIN; minVal = axisMin[1]; maxVal = axisMax[1]; break;
        case 'Z': pin = FILTERED_Z_AXIS_PIN; minVal = axisMin[2]; maxVal = axisMax[2]; break;
        default: return -1;
    }
    return map(analogRead(pin), 0, 1024, minVal, maxVal);
}

void Calibration::readEEPROM() {
    for (int i = 0; i < 3; i++) {
        axisMin[i] = EEPROM.read(2 * i);
        axisMax[i] = EEPROM.read(2 * i + 1);
    }
}

void Calibration::writeEEPROM() {
    for (int i = 0; i < 3; i++) {
        EEPROM.write(2 * i, axisMin[i]);
        EEPROM.write(2 * i + 1, axisMax[i]);
    }
}

void Calibration::waitForButtonPress() {
    while (digitalRead(7) == 0);
}

void Calibration::showCalibrationMessage(char axis, const char* messageFormat) {
    sprintf(lcdBuffer, messageFormat, axis);
    lcd.setCursor(0,0);
    printLCD(lcdBuffer);
    lcd.clear();

}

void Calibration::printLCD(const char* message) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(message);
    lcd.clear();
}
