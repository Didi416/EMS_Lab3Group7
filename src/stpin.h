#include <Arduino.h>
#include <Wire.h>              // for I2C
#include <LiquidCrystal_I2C.h> // the LCD display over I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

class Stpin {

public:
    Stpin(); // Default constructor
    void stControl();

  private:
    int buttonCounter;
    
};