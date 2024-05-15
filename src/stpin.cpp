#include "stpin.h"

Stpin::Stpin(){
    // State control of the button
    buttonCounter = 0;
    int xPin_ = 0;
    int yPin_ = 1;
    int zPin_ = 2;
}

void Stpin::stControl(LiquidCrystal_I2C lcd){
    // Need to account for the inversion that takes place
    if(digitalRead(7) == 0){
        buttonCounter++;
    }
       
    if (buttonCounter == 0) {
        digitalWrite(2, HIGH); //st off
    }
    if (buttonCounter == 1) {

        double x,y,z;
        getAxisData(x,y,z);

        digitalWrite(2, LOW); // If the button is pressed provide 0V to gate, causing ST pin to activate
        // Print the state to the arduino
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ADXL355 SelfTest");

        // X printing
        lcd.setCursor(0, 1);
        lcd.print("X: ");
        lcd.setCursor(3, 1);
        lcd.print(x);

        // Y printing
        lcd.setCursor(7, 1);
        lcd.print("Y: ");
        lcd.setCursor(9,1);
        lcd.print(y);

        // Z printing
        lcd.setCursor(11, 1);
        lcd.print("Z: ");
        lcd.setCursor(13, 1);
        lcd.print(z);

    }
        // If the button is pressed for a second time exit ST routine
    if(buttonCounter == 2){
        buttonCounter = 0;     // Reset the button counter for states
    }
}


void Stpin::getAxisData(double &x, double &y, double &z) {
  x = analogRead(xPin_); // Read sensor data
  y = analogRead(yPin_);  
  z = analogRead(zPin_);
}