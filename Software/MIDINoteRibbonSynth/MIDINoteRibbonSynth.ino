//MIDI NOTE RIBBON SYNTH 
//Version 0.1
//This code reads a ribbon of VHS tape and turns it into midi notes over USB
//It also produces a tone on Pin 3
//
//Joseph Kramer 2024

//Code borrowed from https://docs.arduino.cc/built-in-examples/analog/Smoothing/

#include "MIDIpitches.h"
#include "MIDIfrequencies.h"
#include "MIDIUSB.h"

// pin connections
const int inputPin = A0;  // analog pin connected to ribbon's metal bar
const int knobPin = A1;
const int tonePin = 3;  //digital pin for tone output

// value to determine the size of the readings array.
const int numReadings = 35;  //bigger number provide more stable pitch, but longer glide time

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average
int octave[] = { 0, 12, 24, 36, 48, 60, 74, 86 };
int rootOffset[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
int knobVal, octaveNum;

bool ribbonPressed = false;  //keep track of ribbon press state
int prevNoteNum = 0;         //keep track of previous note number playing for note off



int lowThreshold = 51;  // minimum reading of the sensor (found using ribbonCheck code)
int highThreshold = 810; // update these if you change the ribbon

int previousRead = 0;
int currentRead = 0;

void setup() {
  Serial.begin(115200);  //create serial connection at midi data rate

  //intialize all readings to 0
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {

//  touchCheck();
  knobVal = analogRead(knobPin);
  octaveNum = map(knobVal, 0, 1023, 0, 7);

  if (analogRead(inputPin) > lowThreshold) {

    sensorAveraging();  //run a function that averages sensor readings

    // if the sensor is pressed:
    int rescale = constrain(average, lowThreshold, highThreshold);  //make sure that any readings outside range are truncated
    rescale = map(rescale, lowThreshold, highThreshold, 0, 1023);   //map actual range to new range
   

    /*
    //this is code for a half-working linerization of the ribbon
    int linear = log(rescale) / log(1000) * 1023;  //make the sensor more linear
    linear = constrain(linear, 355, 1000); //these are the boundaries of the "linearization"
    Serial.println(linear);
    int noteNum = map(linear, 355, 1000, 0, 12);  //map sensor reading to select notes from arrays
    */

    //comment out the line below if you uncomment out the linearization code above
    int noteNum = map(rescale, 0, 1023, 0, 14);  //map sensor reading to select notes from arrays change last number to decide how many notes the ribbon should represent
    
    //noteNum = constrain(noteNum, 0, 120); //constrain the output to eliminate weirdness (comment out to allow weirdness)
    noteNum = noteNum + octave[octaveNum] + rootOffset[0];  //select octave with knob and set base note with rootOffset
    float freq = pitchFrequency[noteNum];                              //select a frequency for the built-in tone generator

    /////////////////
    tone(3, freq);  //generate a square wave on the tone pin
    int MidiNote = notePitch[noteNum];

    if (!ribbonPressed) {  //c|| MidiNote != prevNoteNum heck to see if ribbbon was already being held down
      //create midi note from array to match the generated tone
      Serial.println("Ribbon Pressed: ");

      Serial.print("Sending note on: ");
      Serial.println(MidiNote);
      noteOn(0, MidiNote, 100);  // Channel 0, noteNumber, normal velocity
      MidiUSB.flush();
      ribbonPressed = true;
      prevNoteNum = MidiNote;
    }

    if (MidiNote != prevNoteNum) {
      Serial.print("Sending note off: ");
      Serial.println(prevNoteNum);
      noteOff(0, prevNoteNum, 0);  // Channel 0, noteNumber, off velocity
      Serial.print("Sending note on: ");
      Serial.println(MidiNote);
      noteOn(0, MidiNote, 100);  // Channel 0, noteNumber, normal velocity
      MidiUSB.flush();
      prevNoteNum = MidiNote;
    }
   
delay(1);
  }

  else {
    noTone(3);  //turn sound off if nothing is pressed
    //send MIDI off
    if (ribbonPressed) {
      Serial.println("Ribbon Released");
      Serial.print("Sending final note off: ");
      Serial.println(prevNoteNum);
      delay(2);
      noteOff(0, prevNoteNum, 0);  // Channel 0, noteNumber, off velocity
      MidiUSB.flush();
      ribbonPressed = false;
    }
  }
}


void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}

void sensorAveraging() {
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
}

