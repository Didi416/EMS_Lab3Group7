#include "pedometer.h"

//testing variables
 int prevSteps;

Pedometer::Pedometer() {
    Pedometer(int(A0), int(A1), int(A2)); // Initialize buffer 
}

Pedometer::Pedometer(int xPin, int yPin, int zPin) 
  : xPin_(xPin), yPin_(yPin), zPin_(zPin), index(0), 
    indexThreshold(0), bufferDynamicThreshold(INITIAL_THRESHOLD * THRESHOLD_ORDER),
    stepsCount(0), 
    minMaxSampleCounter(0),
    maxFlag(0), 
    oldThreshold(INITIAL_THRESHOLD),
    windowMax(0), windowMin(0),
    windowMaxIndex(0), windowMinIndex(0)
{
  for (int i = 0; i < THRESHOLD_ORDER; i++) {
    bufferDynamicThresholdArray[i] = INITIAL_THRESHOLD;
  }
  for (int i = 0; i < WINDOW_SIZE; i++) {
    magnitudes[i] = 0;
  }
}

void Pedometer::getAxisData(double &x, double &y, double &z) {
  x = analogRead(xPin_); // Read sensor data
  y = analogRead(yPin_);  
  z = analogRead(zPin_);
}

double Pedometer::calculateMagnitude(double x, double y, double z) {
  return sqrt(x * x + y * y + z * z); // Calculate magnitude
}

void Pedometer::updateMagnitudes(double magnitude) {
  magnitudes[index] = magnitude; // Update circular buffer
  index = (index + 1) % WINDOW_SIZE; // Increment index with wrapping
}

void Pedometer::findMaxAndMin() {
  windowMax = magnitudes[0];
  windowMin = magnitudes[0];
  windowMaxIndex = 0;
  windowMinIndex = 0;

  for (int i = 1; i < WINDOW_SIZE; i++) {
    if (magnitudes[i] > windowMax) {
      windowMax = magnitudes[i];
      windowMaxIndex = i;
    } else if (magnitudes[i] < windowMin) {
      windowMin = magnitudes[i];
      windowMinIndex = i;
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
  // for (int i = 0; i < THRESHOLD_ORDER; i++) {
  //   Serial.print(bufferDynamicThresholdArray[i]);
  //   Serial.print(" ");
  // }
  Serial.println();
  indexThreshold = (indexThreshold + 1) % THRESHOLD_ORDER; // Increment buffer index
}

int counter = 0;
unsigned long st = millis();

int Pedometer::stepAlgorithm(int x, int y, int z) {
  
  counter++;
  if (millis() - st > 10000) {
    //Serial.println(counter);
  }

  // Calculate acceleration magnitude
  double magnitude = calculateMagnitude(x, y, z);

  // Update magnitudes in the rolling window
  updateMagnitudes(magnitude);

  // Find the max and min in the current window
  findMaxAndMin();

  if (maxFlag == 0) { 
    // Check if the current index corresponds to the middle of the window
    int expectedMaxIndex = (index + (WINDOW_SIZE / 2)) % WINDOW_SIZE;
    if (windowMaxIndex == expectedMaxIndex) {
      maxFlag = 1; // Flag that we found a max
      lastMax = windowMax; // Store the last max
      minMaxSampleCounter = 0; // Reset the sample counter
    }
  } else {
    // If the min index matches the expected position, we've found the min
    int expectedMinIndex = (index + (WINDOW_SIZE / 2)) % WINDOW_SIZE;
    if (windowMinIndex == expectedMinIndex) {
      lastMin = windowMin; // Update the last min
      maxFlag = 0; // Reset max flag

      int difference = lastMax - lastMin; // Calculate the difference

      if (difference > SENSITIVITY) {
        // The detected difference is significant, update threshold
        updateThresholdLevel();

        // If the new data falls within the threshold and sensitivity, consider it a step
        if ((lastMax > (oldThreshold + (SENSITIVITY / 2))) && (lastMin < (oldThreshold - (SENSITIVITY / 2)))) {
          stepsCount += 2; // Increment the step count
          minMaxSampleCounter = 0; // Reset sample counter
        }
      }
    } else {
      
      if (minMaxSampleCounter > ONE_SECOND) {
        // Reset everything if it takes too long to find a min
        maxFlag = 0;
        minMaxSampleCounter = 0;
      }
    }

    minMaxSampleCounter++;
  }

  int upperSensitivity = oldThreshold + (SENSITIVITY / 2);
  int lowerSensitivity = oldThreshold - (SENSITIVITY / 2);


  // Send data to the Serial Plotter
  // Serial.print(((stepsCount % 2) * 200) + 1000);
  // Serial.print(" ");
  Serial.print(stepsCount);
  Serial.print(" ");
  Serial.print(upperSensitivity);
  Serial.print(" ");
  Serial.print(lowerSensitivity);
  Serial.print(" ");
  Serial.print(magnitude);
  Serial.println();


  return stepsCount; // Return the current step count
}

