#include "Arduino.h"
#include "pedometer.h"

const int windowSize = 10;
static float magnitudes[windowSize] = {0};
int minMaxSampleCounter= 0; //Count the samples between max and min to discard step if it would be very long.
float magnitude = 0;
static int consecutiveSteps = 0;
int maxFlag = 0;

int index, indexWindowMax, indexWindowMin, indexThreshold;

// maxFlag --> Maximum detected
// thresholdFlag --> it indicates that the Threshold has been overcome
// thresholdCounterFlag --> it indicates the number of times that the Threshold has been overcome
int maxFlag, thresholdFlag, thresholdCounterFlag;

bool regulationMode;

int lastMin, lastMax;
int windowMax, windowMin;

int difference;

int sensitivity = 6;
int newThreshold, oldThreshold;
int thresholdOrder = 4;
int bufferDynamicThreshold = 60 * thresholdOrder; // bufferDynamicThreshold--> Threshold Buffer
int buffer_dynamic_threshold[4]; // -->Buffer to store the Threshold data 

int stepToStepSamples;

int stepsCount, possibleStepsCount;

int oneSecond = 50; //number of cycles ina second


Pedometer::Pedometer() {
    Pedometer(0, 1, 2);
}

Pedometer::Pedometer(int xPin, int yPin, int zPin) {
    xPin_ = xPin;
    yPin_ = yPin;
    zPin_ = zPin;
}

void Pedometer::getAxisData(int& x, int& y, int& z) {
    // Code to read the 3 axis data from the sensor
    // Assign the values to the provided variables
    // For example:
    x = analogRead(xPin_);
    y = analogRead(yPin_);  
    z = analogRead(zPin_);
}

int Pedometer::StepAlgorithm(int _x, int _y, int _z, int _stepsCount) {
    stepsCount = _stepsCount;

    // Calculate the magnitude of the acceleration vector
    magnitude = sqrt(_x * _x + _y * _y + _z * _z);

    // Replace the oldest magnitude with the new one
    magnitudes[index] = magnitude;
    index = (index + 1) % windowSize;

    //find max
    windowMax = magnitudes[0];
    indexWindowMax = 0;
    for (int i = 1; i < windowSize; i++) {
        if (magnitudes[index] > windowMax) {
            windowMax = magnitudes[i];
            indexWindowMax = i;
        }
    }

    //find min
    windowMin = magnitudes[0];
    indexWindowMin = 0;
    for (int i = 1; i < windowSize; i++) {
        if (magnitudes[index] < windowMin) {
            windowMin = magnitudes[i];
            indexWindowMin = i;
        }
    }

    if (maxFlag == 0) { 
        // Now, we try to know if the Window max is in the middle of the window,
        //  in this case I will mark it as a max
        if (indexWindowMax == ((index + (windowSize/2)) % windowSize)) { 
            maxFlag = 1;
            lastMax = windowMax;
            minMaxSampleCounter = 0;
        }
    } else {
        if (indexWindowMin == ((index + (windowSize/2)) % windowSize)) {
            lastMin = windowMin;  // Updates the MIN
            difference = lastMax - lastMin;  // Updates the Difference
            maxFlag = 0;
            minMaxSampleCounter = 0;  // Reset the max-min sample counter

            /* The algorithm detects if the SENSITIVITYs are in of the Threshold level */

            if ((lastMax > (oldThreshold+(sensitivity/2))) && (lastMin < (oldThreshold-(sensitivity/2)))) {
                thresholdFlag = 1;  // Possible step (it will be analize later)
                thresholdCounterFlag = 0;
            } else {
                thresholdCounterFlag++;  // Number of times the SENSITIVITYs are out of the Threshold Level consecutively
            }

            /* SENSITIVITY detection */
            /* Each time the difference is higher than the fixed SENSITIVITY the algorithm updates the Threshold */

            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //+++++++++++++++++++++++ THRESHOLD LEVEL UPDATE ++++++++++++++++++++++++++++
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            if (difference > sensitivity) {
                /* Threshold level is calculated with the 4 previous good differences */
                // The same method as the Filtering method
                newThreshold = (lastMax + lastMin)/2;  // Calculates the New Threshold
                bufferDynamicThreshold = bufferDynamicThreshold - buffer_dynamic_threshold[indexThreshold] + newThreshold;
                oldThreshold = bufferDynamicThreshold / thresholdOrder; //BufferDinamicThreshold<<2
                buffer_dynamic_threshold[indexThreshold] = newThreshold;
                indexThreshold++;
                if (indexThreshold > thresholdOrder - 1) {
                    indexThreshold = 0;
                }

                // Once the Threshold Level is calculated and if the SENSITIVITYs was in the
                // Threshold Level we certificate the step.

                //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                //++++++++++++++++++++++++ STEP CERTIFICATION   +++++++++++++++++++++++++
                //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

                if (thresholdFlag) {
                    // DETECTED STEP
                    thresholdFlag = 0;
                    stepToStepSamples = 0; // Reset the time.
                    minMaxSampleCounter = 0;

                    if (regulationMode) {
                        //=====Regulation mode======
                        // the step is counted
                        stepsCount++;
                    } else {
                        //=====Non Regulation mode======
                        possibleStepsCount++;

                        // if the possible steps number is 8,
                        // the algorithm concludes that the person is walking.
                        if (possibleStepsCount == 8) {
                            stepsCount = stepsCount + possibleStepsCount; // Possible steps are added to counted steps
                            possibleStepsCount = 0;
                            regulationMode = 1;  // The pedometer enters the Regulation mode
                        }
                    }                 
                }
            }
        
            windowMin = 0;
            // If the SENSITIVITYs are out of the Threshold Level 2 times consecutively with a good difference,
            // Is very probably that the acceleration profile isn't a step sequence
            // (Reset the possible steps to discard false steps)
            if (thresholdCounterFlag > 1) {
                thresholdCounterFlag = 0;
                minMaxSampleCounter = 0;
                windowMax = 0;  // maybe it is not necessary  reset them.
                windowMin = 0;
                regulationMode = 0;
                possibleStepsCount = 0;
                thresholdCounterFlag = 0;
            }
        } else {
            // While the pedometer is searching a min if between MAX and MIN
            //  takes more than 25 samples I reset the step.
            minMaxSampleCounter++;
            if (minMaxSampleCounter == oneSecond) {
                // Reset Step
                minMaxSampleCounter = 0;
                windowMax = 0;
                windowMin = 0;
                maxFlag = 0;
                possibleStepsCount = 0;
            }
        }
    }
    // IndexBuffer is used as a circular buffer index
    index++;
    if (index > windowSize-1) {
        index = 0;
    }

    stepToStepSamples++;
    if (stepToStepSamples >= 2 * oneSecond) {
        // If the pedometer takes 2 seconds without counting a step it
        //  returns to the Non Regulation mode.
        stepToStepSamples = 0;
        possibleStepsCount = 0;
        regulationMode = 0;
        minMaxSampleCounter = 0;
        if (regulationMode == 1) {
            regulationMode = 0;
            oldThreshold = 60;
            bufferDynamicThreshold = 60 * thresholdOrder;//(INIT_OFFSET_VALUE)<<2
            indexThreshold = 0;
            thresholdCounterFlag = 0;
            for (char i = 0; i < thresholdOrder; i++) {
                buffer_dynamic_threshold[i] = 60;
            }
        }
    }   
    // Return the updated step count
    return _stepsCount;
}