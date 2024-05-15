// ADXL335.h

#ifndef ADXL335_H
#define ADXL335_H

#include <Arduino.h> // Include the Arduino library (if not already included)

class ADXL335 {
public:
    ADXL335(int xPin, int yPin, int zPin);
    void begin();
    float getXAcceleration();
    float getYAcceleration();
    float getZAcceleration();

private:
    int xpin = A0;
    int ypin = A1;
    int zpin = A2;
};

#endif // ADXL335_H
