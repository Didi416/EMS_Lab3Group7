#include "pedometer.h"

Pedometer::Pedometer() {
    Pedometer(0, 1, 2); // Initialize buffer 
}

Pedometer::Pedometer(int xPin, int yPin, int zPin) 
  : xPin_(xPin), yPin_(yPin), zPin_(zPin), index(0), 
    indexThreshold(0), bufferDynamicThreshold(INITIAL_THRESHOLD * THRESHOLD_ORDER),
    stepsCount(0), possibleStepsCount(0), 
    minMaxSampleCounter(0), stepToStepSamples(0), 
    maxFlag(0), thresholdFlag(0), thresholdCounterFlag(0), 
    oldThreshold(INITIAL_THRESHOLD), regulationMode(false),
    windowMax(0), windowMin(0),
    indexWindowMax(0), indexWindowMin(0)
{
  memset(magnitudes, 0, sizeof(magnitudes)); // Initialize magnitudes
  memset(bufferDynamicThresholdArray, INITIAL_THRESHOLD, sizeof(bufferDynamicThresholdArray)); // Initialize buffer
}

void Pedometer::getAxisData(int &x, int &y, int &z) {
  x = analogRead(xPin_); // Read sensor data
  y = analogRead(yPin_);  
  z = analogRead(zPin_);
}

int Pedometer::calculateMagnitude(int x, int y, int z) {
  return sqrt(x * x + y * y + z * z); // Calculate magnitude
}

void Pedometer::updateMagnitudes(float magnitude) {
  magnitudes[index] = magnitude; // Update circular buffer
  index = (index + 1) % WINDOW_SIZE; // Increment index with wrapping
}

void Pedometer::findMaxAndMin() {
  windowMax = magnitudes[0];
  windowMin = magnitudes[0];
  int indexWindowMax = 0;
  int indexWindowMin = 0;

  for (int i = 1; i < WINDOW_SIZE; i++) {
    if (magnitudes[i] > windowMax) {
      windowMax = magnitudes[i];
      indexWindowMax = i;
    } else if (magnitudes[i] < windowMin) {
      windowMin = magnitudes[i];
      indexWindowMin = i;
    }
  }
}

void Pedometer::updateThresholdLevel() {
  // Calculate new threshold based on last min and max
  int newThreshold = (lastMax + lastMin) / 2;
  bufferDynamicThreshold -= bufferDynamicThresholdArray[indexThreshold];
  bufferDynamicThreshold += newThreshold;

  oldThreshold = bufferDynamicThreshold / THRESHOLD_ORDER; // Updated old threshold
  bufferDynamicThresholdArray[indexThreshold] = newThreshold; // Update buffer
  indexThreshold = (indexThreshold + 1) % THRESHOLD_ORDER; // Increment buffer index
}

int Pedometer::stepAlgorithm(int x, int y, int z, int _stepsCount) {
  // Calculate acceleration magnitude
  float magnitude = calculateMagnitude(x, y, z);

  // Update magnitudes in the rolling window
  updateMagnitudes(magnitude);

  // Find the max and min in the current window
  findMaxAndMin();

  if (maxFlag == 0) { 
    // Check if the current index corresponds to the middle of the window
    int expectedMaxIndex = (index + (WINDOW_SIZE / 2)) % WINDOW_SIZE;
    if (indexWindowMax == expectedMaxIndex) {
      maxFlag = 1; // Flag that we found a max
      lastMax = windowMax; // Store the last max
      minMaxSampleCounter = 0; // Reset the sample counter
    }
  } else {
    // If the min index matches the expected position, we've found the min
    int expectedMinIndex = (index + (WINDOW_SIZE / 2)) % WINDOW_SIZE;
    if (indexWindowMin == expectedMinIndex) {
      lastMin = windowMin; // Update the last min
      maxFlag = 0; // Reset max flag
      minMaxSampleCounter = 0; // Reset sample counter

      int difference = lastMax - lastMin; // Calculate the difference

      if (difference > SENSITIVITY) {
        // The detected difference is significant, update threshold
        updateThresholdLevel();

        // If the new data falls within the threshold and sensitivity, consider it a step
        if ((lastMax > (oldThreshold + (SENSITIVITY / 2))) && (lastMin < (oldThreshold - (SENSITIVITY / 2)))) {
          if (thresholdFlag) { // If a threshold flag was set
            thresholdFlag = 0; // Reset the threshold flag
            stepsCount++; // Increment the step count
          }
        }
      }
    } else {
      // If the sample counter exceeds a certain threshold, reset to avoid false positives
      minMaxSampleCounter++;

      if (minMaxSampleCounter > ONE_SECOND) {
        // Reset everything if it takes too long to find a min
        minMaxSampleCounter = 0;
        maxFlag = 0;
        possibleStepsCount = 0;
        stepToStepSamples = 0;
        regulationMode = 0;
      }
    }
  }

  int upperSensitivity = oldThreshold + (SENSITIVITY / 2);
  int lowerSensitivity = oldThreshold - (SENSITIVITY / 2);

  // Send data to the Serial Plotter
  Serial.print(stepsCount);
  Serial.print(",");
  Serial.print(possibleStepsCount);
  Serial.print(",");
  Serial.print(oldThreshold);
  Serial.print(",");
  Serial.print(upperSensitivity);
  Serial.print(",");
  Serial.print(lowerSensitivity);
  Serial.println(); // End of line to separate readings


  return stepsCount; // Return the current step count
}

