#include "stpin.h"

Stpin::Stpin(){
    
       
    // State control of the button
    buttonCounter = 0;
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
        digitalWrite(2, LOW); // If the button is pressed provide 0V to gate, causing ST pin to activate
        // Print the state to the arduino
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ADXL355 SelfTest");
        lcd.setCursor(0, 1);
        lcd.print("Routine Active!");
    }
        // If the button is pressed for a second time exit ST routine
    if(buttonCounter == 2){
        buttonCounter = 0;     // Reset the button counter for states
    }
}


<<<<<<< HEAD

=======
>>>>>>> 0b40d2eaf00c40870257d75f9b061679daf9c4d3
