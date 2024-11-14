//COde borrowed from https://docs.arduino.cc/built-in-examples/analog/Smoothing/

// value to determine the size of the readings array.
const int numReadings = 30;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average
int inputPin = A0;


const int threshold = 80;  // minimum reading of the sensors that generates a note



void setup() {
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  // for (int thisSensor = 0; thisSensor < 3; thisSensor++) {
  //   // get a sensor reading:
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
  // send it to the computer as ASCII digits

  //Serial.print("average Value = ");
  // Serial.print(average);



  // if the sensor is pressed:
  if (average > threshold) {
    // play the note corresponding to this sensor:
    int freq = map(average, 260, 810, 100, 2000);
    freq = constrain(freq, 10, 2000);
    tone(3, freq);
    Serial.print(2000);  // print the counter
    Serial.print(" ");   // print a space
    Serial.print(10);    // print the counter
    Serial.print(" ");
    int sensorReading = analogRead(A0);
    Serial.print(sensorReading);
    Serial.print(" ");
    Serial.println(freq);
  } else
    noTone(3);
  delay(1);
}
