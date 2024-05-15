#include <LiquidCrystal_I2C.h> // the LCD display over I2C
#include <Arduino.h>

class Stpin {

public:
    Stpin(); // Default constructor
    void stControl(LiquidCrystal_I2C lcd);
    void getAxisData(double &x, double &y, double &z);

  private:
    int buttonCounter;
    int xPin_;
    int yPin_;
    int zPin_;
};