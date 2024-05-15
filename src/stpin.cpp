#include "stpin.h"

Stpin::Stpin(){
    pinMode(2, OUTPUT); // Either 5V to gate or 0V to gate  
    pinMode(7, INPUT);  // Button is read in from this pin
    
    // Lcd declaration for writing to it
    lcd.begin(16, 2);
    lcd.backlight();
    
    // State control of the button
    buttonCounter = 0;
}

void Stpin::stControl(){
    // Need to account for the inversion that takes place
    if(digitalRead(7) == HIGH){
        buttonCounter++;
        digitalWrite(2, LOW); // If the button is pressed provide 0V to gate, causing ST pin to activate

        // Print the state to the arduino
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ADXL355 Self Test");
        lcd.setCursor(0, 1);
        lcd.print("Routine Active!!!");

        // If the button is pressed for a second time exit ST routine
        if(buttonCounter == 2){
            digitalWrite(2, HIGH); // Provide digital high 5V to the gate, causing the ST pin to be low
            buttonCounter = 0;     // Reset the button counter for states
        }
    }
}



