#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>

#define FILTERED_X_AXIS_PIN A0
#define FILTERED_Y_AXIS_PIN A1
#define FILTERED_Z_AXIS_PIN A2
class Calibration {
public:

    Calibration();
    int calibrate();
    void calibrateAxisX(int &minVal, int &maxVal);
    void calibrateAxisY(int &minVal, int &maxVal);
    void calibrateAxisZ(int &minVal, int &maxVal);
    int readSensor(char axis);
    double getCalibratedReading(char axis);
    void readEEPROM();
    void writeEEPROM();
    void waitForButtonPress();
    void showCalibrationMessage(char axis, const char* message);
    void printLCD(const char* message);

};
extern char lcdBuffer[32];

#endif // CALIBRATION_H