#ifndef PEDOMETER_H
#define PEDOMETER_H

#include <Arduino.h> 
#include <math.h> // For sqrt function

// Constants
const int WINDOW_SIZE = 14;
const int THRESHOLD_ORDER = 4;
const int ONE_SECOND = 41; // Number of cycles in a second
const double INITIAL_THRESHOLD = 1070; // Initial threshold value
const double SENSITIVITY = 160; // Sensitivity for step detection

// Pedometer Class Implementation
class Pedometer {
  public:
    Pedometer(); // Default constructor
    Pedometer(int xPin, int yPin, int zPin); // Overloaded constructor
    void getAxisData(double &x, double &y, double &z);
    double calculateMagnitude(double x, double y, double z);
    void updateMagnitudes(double magnitude);
    void findMaxAndMin();
    void updateThresholdLevel();
    int stepAlgorithm(int x, int y, int z);
    void resetStepCount();

  private:
    int xPin_, yPin_, zPin_; // Sensor pins
    double magnitudes[WINDOW_SIZE]; // Circular buffer for magnitudes
    int index; // Index for circular buffer
    int indexThreshold; // Index for dynamic threshold buffer
    int bufferDynamicThreshold; // Dynamic threshold buffer sum
    int bufferDynamicThresholdArray[THRESHOLD_ORDER]; // Dynamic threshold values
    int stepsCount; // Step count
    int minMaxSampleCounter; // Counter for samples between min and max
    int maxFlag; // Flag to indicate a max was found
    int oldThreshold; // Previous threshold value
    int lastMax; // Last detected max
    int lastMin; // Last detected min
    int windowMax; // Current window max
    int windowMin; // Current window min
    int windowMaxIndex; // Index for the max in the window
    int windowMinIndex; // Index for the min in the window
};

#endif //PEDOMETER_H