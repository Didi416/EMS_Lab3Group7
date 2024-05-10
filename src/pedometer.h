#ifndef PEDOMETER_H
#define PEDOMETER_H

#include <Arduino.h> 
#include <math.h> // For sqrt function

// Constants
const int WINDOW_SIZE = 10;
const int THRESHOLD_ORDER = 4;
const int ONE_SECOND = 50; // Number of cycles in a second
const int INITIAL_THRESHOLD = 60; // Initial threshold value
const int SENSITIVITY = 6; // Sensitivity for step detection

// Pedometer Class Implementation
class Pedometer {
  public:
    Pedometer(); // Default constructor
    Pedometer(int xPin, int yPin, int zPin); // Overloaded constructor
    void getAxisData(int &x, int &y, int &z);
    int calculateMagnitude(int x, int y, int z);
    void updateMagnitudes(float magnitude);
    void findMaxAndMin();
    void updateThresholdLevel();
    int stepAlgorithm(int x, int y, int z, int stepCount);

  private:
    int xPin_, yPin_, zPin_; // Sensor pins
    float magnitudes[WINDOW_SIZE]; // Circular buffer for magnitudes
    int index; // Index for circular buffer
    int indexThreshold; // Index for dynamic threshold buffer
    int bufferDynamicThreshold; // Dynamic threshold buffer sum
    int bufferDynamicThresholdArray[THRESHOLD_ORDER]; // Dynamic threshold values
    int stepsCount; // Step count
    int possibleStepsCount; // Possible step count (pre-regulation)
    int minMaxSampleCounter; // Counter for samples between min and max
    int stepToStepSamples; // Samples between steps
    int maxFlag; // Flag to indicate a max was found
    int thresholdFlag; // Flag for threshold detection
    int thresholdCounterFlag; // Counter for threshold checks
    int oldThreshold; // Previous threshold value
    int lastMax; // Last detected max
    int lastMin; // Last detected min
    bool regulationMode; // Regulation mode indicator
    int windowMax; // Current window max
    int windowMin; // Current window min
    int indexWindowMax; // Index for the max in the window
    int indexWindowMin; // Index for the min in the window
};

#endif //PEDOMETER_H