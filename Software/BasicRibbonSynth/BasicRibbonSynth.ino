//Code borrowed from https://docs.arduino.cc/built-in-examples/analog/Smoothing/

// value to determine the size of the readings array.
const int numReadings = 1;  //bigger number provide more stable pitch, but longer glide time

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average

const int inputPin = A0;  // analog pin connected to ribbon's metal bar
const int tonePin = 3;    //digital pin for tone output

int lowThreshold = 51;  // minimum reading of the sensor (found using ribbonCheck code)
int highThreshold = 810; // update these if you change the ribbon



void setup() {
  Serial.begin(9600);
  //intialize all readings to 0
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {

  // get a sensor reading:
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  // if the sensor is pressed:
  if (analogRead(inputPin) > lowThreshold) {
    // play a frequency relative corresponding to this sensor:
    int freq = map(average, lowThreshold, highThreshold, 100, 2000);  //map reading value to desired frequency range
    freq = constrain(freq, 10, 2000);              //constrain the output to eliminate weirdness (comment out to allow weirdness)
    tone(3, freq);                                 //generate a square wave on the tone pin

  }

  else {
    noTone(3);  //turn sound off if nothing is pressed
  }
}
