#include "stpin.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

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
        lcd.print("ADXL355 Self Test");
        lcd.setCursor(0, 1);
        lcd.print("Routine Active!!!");
    }
        // If the button is pressed for a second time exit ST routine
    if(buttonCounter == 2){
        buttonCounter = 0;     // Reset the button counter for states
    }
}


