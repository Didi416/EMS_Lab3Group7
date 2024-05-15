int xpin = A3;
int ypin = A2;
int zpin = A1;
int xvalue;
int yvalue;
int zvalue;

void setup() {
    Serial.begin(9600); // Initialize serial communication
}

void loop() {
    xvalue = analogRead(xpin);
    int x = map(xvalue, 267, 400, -100, 100); // Map analog values to -100 to 100 (adjust with your calibration values)
    float xg = static_cast<float>(x) / (-100.00); // Convert mapped value to acceleration in terms of "g"
    Serial.print(xg);
    Serial.print("g\t");

    yvalue = analogRead(ypin);
    int y = map(yvalue, 272, 406, -100, 100);
    float yg = static_cast<float>(y) / (-100.00);
    Serial.print(yg);
    Serial.print("g\t");

    zvalue = analogRead(zpin);
    int z = map(zvalue, 277, 410, -100, 100);
    float zg = static_cast<float>(z) / 100.00;
    Serial.print(zg);
    Serial.println("g");

    delay(100);
}
