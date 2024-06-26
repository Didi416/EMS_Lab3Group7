#include "stpin.h"

Stpin::Stpin(){
    // State control of the button
    buttonCounter = 0;

    xPin_ = A0;
    yPin_ = A1;
    zPin_ = A2;
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
        
        // 10% from 3V to 3.3V and the filter gain added
        double expectedX = -325 * 1.585 * 1.1;
        double expectedY = 325 * 1.585 * 1.1;
        double expectedZ = 550 * 1.585 * 1.1;

        // 10% -+ tolerance since the data sheet shows the 3V values but we need 3.3V values which is 10% more
        if(x > expectedX*0.9 && x < expectedX*1.1){
            // X printing
            lcd.setCursor(0, 0);
            lcd.print("X: PASS");
        } else {
            lcd.setCursor(0, 0);
            lcd.print("X: FAIL");}

        if(y > expectedY*0.9 && y < expectedY*1.1){
            // X printing
            lcd.setCursor(0, 1);
            lcd.print("Y: PASS");
        } else {
            lcd.setCursor(0, 1);
            lcd.print("Y: FAIL");}

        if(z > expectedZ*0.9 && z < expectedZ*1.1){
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