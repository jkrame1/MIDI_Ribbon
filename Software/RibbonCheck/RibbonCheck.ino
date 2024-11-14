/*
This code is to check the high and low values of the ribbon.

INSTRUCTIONS:

1. Turn on the Serial Monitor in the Arduino Tools menu
2. Turn on calibration mode by pressing the ribbon first, and then turning the knob all the way up to the right 
3. Do not remove your finger from the ribbon
4. Slide your finger all the way to the left of the ribbon and all the way to the right of the ribbon
5. Without removing your finger, turn the knob all the way down to the left.
6. The serial monitor should now show the the high and low thresholds that can be used in your other code
7. Run this sketch any time you replace the ribbon

*/



const int ribbonPin = A0;
const int knobPin = A1;

int lowThreshold = 1023;
int highThreshold = 0;
int ribbonVal;

void setup() {
  Serial.begin(9600);
}

void loop() {

  //check whether to enter calibration mode
  if (analogRead(knobPin) > 900) {

    ribbonVal = analogRead(ribbonPin);
    if (ribbonVal < lowThreshold) {
      lowThreshold = ribbonVal;
    }
    if (ribbonVal > highThreshold) {
      highThreshold = ribbonVal;
    }

    Serial.print("Ribbon Value = ");
    Serial.print(ribbonVal);
    Serial.print("\t");
    Serial.print("LOW THRESHOLD = ");
    Serial.print(lowThreshold);
    Serial.print("\t");
    Serial.print("HIGH THRESHOLD = ");
    Serial.println(highThreshold);
  }
}
