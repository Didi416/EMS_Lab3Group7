#include <LiquidCrystal_I2C.h> // the LCD display over I2C
#include <Arduino.h>

class Stpin {

public:
    Stpin(); // Default constructor
    void stControl();

  private:
    int buttonCounter;
    
};