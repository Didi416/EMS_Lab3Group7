#include "Arduino.h"
#include "pedometer.h"


Pedometer::Pedometer() {

}
void Pedometer::getAxisData(int& x, int& y, int& z) {
    // Code to read the 3 axis data from the sensor
    // Assign the values to the provided variables
    // For example:
    x = analogRead(xPin);
    y = analogRead(yPin);  
    z = analogRead(zPin);
}

int Pedometer::StepAlgorithm(int _x, int _y, int _z, int _stepsCount) {
    // Calculate the magnitude of the acceleration vector
    float magnitude = sqrt(_x * _x + _y * _y + _z * _z);

    // Define the size of the moving window
    const int windowSize = 10;

    // Initialize a static array to hold the last N magnitudes
    static float magnitudes[windowSize] = {0};
    static int index = 0;

    // Replace the oldest magnitude with the new one
    magnitudes[index] = magnitude;
    index = (index + 1) % windowSize;

    // Calculate the average magnitude
    float sum = 0;
    for (int i = 0; i < windowSize; i++) {
        sum += magnitudes[i];
    }
    float average = sum / windowSize;

    // Set threshold values based on the moving average
    float highThreshold = average * 1.5; // Adjust this multiplier based on your requirements
    float lowThreshold = average * 0.8; // Adjust this multiplier based on your requirements

    // Initialize peak detection state variables
    static int consecutiveSteps = 0;

    // Check if the magnitude exceeds the high threshold
    if (magnitude > highThreshold) {
        consecutiveSteps++;
    } else {
        consecutiveSteps = 0;
    }

    // Check if the consecutive steps count reaches the desired number
    if (consecutiveSteps >= 6) {
        // Increment the step count
        _stepsCount++;
        // Reset the consecutive steps count
        consecutiveSteps = 0;
    }

    // Return the updated step count
    return _stepsCount;
}