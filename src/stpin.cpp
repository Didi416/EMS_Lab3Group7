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
        delay(500);
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
        lcd.print("SelfTest X,Y,Z:");

        lcd.setCursor(0, 1);
        lcd.print("Orient as on LCD");

        delay(1000);

        lcd.setCursor(0, 0);
        lcd.print("Lay flat");

        delay(1000);
        lcd.clear();

        for(int i = 0; i < 2000; i++){
            // X printing
            lcd.setCursor(0, 0);
            lcd.print("X: ");
            lcd.setCursor(3, 0);
            lcd.print(x);
        }

        lcd.setCursor(0, 0);
        lcd.print("Lay on side");

        delay(1000);
        lcd.clear();

        for(int i = 0; i < 2000; i++){

        // Y printing
        lcd.setCursor(0, 1);
        lcd.print("Y: ");
        lcd.setCursor(3, 0);
        lcd.print(y);

        // Z printing
        lcd.setCursor(7, 1);
        lcd.print("Z: ");
        lcd.setCursor(10, 1);
        lcd.print(z);

        }
        

        double expectedX = 0;
        double expectedY = 0;
        double expectedZ = 0;


        if(x == expectedX){
            // X printing
            lcd.setCursor(0, 0);
            lcd.print("X: PASS");
        } else {
            lcd.setCursor(0, 0);
            lcd.print("X: FAIL");}

        if(y == expectedY){
            // X printing
            lcd.setCursor(0, 1);
            lcd.print("Y: PASS");
        } else {
            lcd.setCursor(0, 1);
            lcd.print("Y: FAIL");}

        if(z == expectedZ){
            // X printing
            lcd.setCursor(8, 1);
            lcd.print("Z: PASS");
        } else {
            lcd.setCursor(8, 1);
            lcd.print("Z: FAIL");}
        
        delay(2000);
        lcd.clear();
    }
        // If the button is pressed for a second time exit ST routine
    if(buttonCounter == 2){
        buttonCounter = 0;     // Reset the button counter for states
        lcd.clear();
    }
}


void Stpin::getAxisData(double &x, double &y, double &z) {
  x = analogRead(xPin_); // Read sensor data
  y = analogRead(yPin_);  
  z = analogRead(zPin_);
}