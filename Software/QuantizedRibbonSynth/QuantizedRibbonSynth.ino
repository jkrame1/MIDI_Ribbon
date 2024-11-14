//Code borrowed from https://docs.arduino.cc/built-in-examples/analog/Smoothing/

// value to determine the size of the readings array.
const int numReadings = 3;  //bigger number provide more stable pitch, but longer glide time

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average
int octave[] = {0,12,24,36,48,60,74};
int rootOffset[] = {0,1,2,3,4,5,6,7,8,9,10,11};

const int inputPin = A0;  // analog pin connected to ribbon's metal bar
const int tonePin = 3;    //digital pin for tone output

int lowThreshold = 51;  // minimum reading of the sensor (found using ribbonCheck code)
int highThreshold = 810; // update these if you change the ribbon

// Conversion from MIDI pitch codes to frequencies in Hz
static const float pitchFrequency[] = {
	   8.176,     8.662,     9.177,     9.723,    10.301,    10.913,    11.562,    12.250,    12.978,    13.750,    14.568,    15.434,
	  16.352,    17.324,    18.354,    19.445,    20.602,    21.827,    23.125,    24.500,    25.957,    27.500,    29.135,    30.868,
	  32.703,    34.648,    36.708,    38.891,    41.203,    43.654,    46.249,    48.999,    51.913,    55.000,    58.270,    61.735,
	  65.406,    69.296,    73.416,    77.782,    82.407,    87.307,    92.499,    97.999,   103.826,   110.000,   116.541,   123.471,
	 130.813,   138.591,   146.832,   155.563,   164.814,   174.614,   184.997,   195.998,   207.652,   220.000,   233.082,   246.942,
	 261.626,   277.183,   293.665,   311.127,   329.628,   349.228,   369.994,   391.995,   415.305,   440.000,   466.164,   493.883,
	 523.251,   554.365,   587.330,   622.254,   659.255,   698.456,   739.989,   783.991,   830.609,   880.000,   932.328,   987.767,
	1046.502,  1108.731,  1174.659,  1244.508,  1318.510,  1396.913,  1479.978,  1567.982,  1661.219,  1760.000,  1864.655,  1975.533,
	2093.005,  2217.461,  2349.318,  2489.016,  2637.020,  2793.826,  2959.955,  3135.963,  3322.438,  3520.000,  3729.310,  3951.066,
	4186.009,  4434.922,  4698.636,  4978.032,  5274.041,  5587.652,  5919.911,  6271.927,  6644.875,  7040.000,  7458.620,  7902.133,
	8372.018,  8869.844,  9397.273,  9956.063, 10548.082, 11175.303, 11839.822, 12543.854,
};


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
    int noteNum = map(average, lowThreshold, highThreshold, 0, 13);  //map reading value to desired frequency range
    noteNum = constrain(noteNum, 0, 120) + octave[2] + rootOffset[9];              //constrain the output to eliminate weirdness (comment out to allow weirdness)
    float freq = pitchFrequency[noteNum];
    tone(3, freq);                                 //generate a square wave on the tone pin

  //  /*
    //A bunch of printing to get info
    Serial.print(2000);  // print the counter
    Serial.print(" ");   // print a space
    Serial.print(10);    // print the counter
    Serial.print(" ");
    // int sensorReading = analogRead(A0);
    // Serial.print(sensorReading);
    // Serial.print(" ");
    Serial.println(freq);
    delay(1);           //tiny delay to stabalize serial printing
   // */

  }

  else {
    noTone(3);  //turn sound off if nothing is pressed
  }
}
