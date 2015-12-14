#include <MaxMatrix.h>
#include <avr/pgmspace.h>
#include "pitches.h"
#include "alphabet.h"

int matrixDataPin = 8;          // DIN pin of MAX7219 module
int matrixLoadPin = 9;          // CS pin of MAX7219 module
int matrixClockPin = 10;        // CLK pin of MAX7219 module
int maxInUse = 1;               // how many MAX7219 are connected

MaxMatrix m(matrixDataPin, matrixLoadPin, matrixClockPin, maxInUse); // define Library

const int speakerPin = 7;       // Piezo buzzer pin
const int celsiusLedPin = 13;   // Piezo buzzer pin
const int sensorPin = A0;       // Analog input pin that the temperature sensor is attached to
const int buttonPin = 2;        // Push button pin

int sensorValue = 0;
boolean currentState = LOW;
boolean lastState = LOW;
boolean ledState = LOW;
boolean isCelsius = true;

byte buffer[10];

//"Do you want to build a snowman?" - melody
int snowman[] = {
        NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_AS3, NOTE_DS4, NOTE_G4, NOTE_F4, NOTE_G4, 0, 0,
        0, 0, NOTE_DS4, NOTE_DS4, NOTE_AS3, NOTE_DS4, NOTE_G4, NOTE_F4, 0, 0, 0, 0, 0,
        NOTE_DS4, NOTE_DS4, NOTE_AS3, NOTE_DS4, NOTE_G4, NOTE_GS4, NOTE_G4, NOTE_DS4, 0, NOTE_C4, NOTE_GS4, NOTE_G4,
        NOTE_DS4, 0, NOTE_DS4, NOTE_DS4, NOTE_AS3, NOTE_DS4, NOTE_G4, NOTE_AS4, 0
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int snowmanDurations[] = {
        8, 8, 8, 8, 8, 8, 4, 2, 4, 8,
        4, 8, 8, 8, 8, 8, 8, 2, 4, 4, 8, 4, 8,
        8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 8,
        4, 8, 8, 8, 8, 8, 8, 1, 4
};


void setup() {
    m.init(); // module MAX7219
    m.setIntensity(8); // LED Intensity 0-15
    pinMode(celsiusLedPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    digitalWrite(celsiusLedPin, HIGH);
}

void loop() {

    currentState = digitalRead(buttonPin);

    if (currentState == HIGH && lastState == LOW) {//if button has just been pressed

        delay(1);//crude form of button debouncing

        isCelsius = !isCelsius;

        digitalWrite(celsiusLedPin, isCelsius ? HIGH : LOW);


    }

    lastState = currentState;

    if (currentState == LOW) {

        sensorValue = analogRead(sensorPin);

        float temperatureC = (4.9 * sensorValue * 100.0) / 1024.0;
        float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
        char stringTemp[6];

        if (isCelsius) {
            dtostrf(temperatureC, 4, 2, stringTemp);
            printStringWithShift(stringTemp, 100);  // Shows temperature value from analog input
            printStringWithShift("~C   ", 100);  // Shows temperature sign
        } else {
            dtostrf(temperatureF, 4, 2, stringTemp);
            printStringWithShift(stringTemp, 100);  // Shows temperature value from analog input
            printStringWithShift("~F   ", 100);  // Shows temperature sign
        }

        if (temperatureC < 20.0) {
            playSnowmanMelody();
        }
    }
}

// Put extracted character on Display
void printCharWithShift(char c, int shift_speed) {
    if (c < 32) return;
    c -= 32;
    memcpy_P(buffer, CH + 7 * c, 7);
    m.writeSprite(maxInUse * 8, 0, buffer);
    m.setColumn(maxInUse * 8 + buffer[0], 0);

    for (int i = 0; i < buffer[0] + 1; i++) {
        delay(shift_speed);
        m.shiftLeft(false, false);
    }
}

// Extract characters from Scrolling text
void printStringWithShift(char *s, int shift_speed) {
    while (*s != 0) {
        printCharWithShift(*s, shift_speed);
        s++;
    }
}

void playSnowmanMelody() {
    for (int thisNote = 0; thisNote < (sizeof(snowmanDurations) / 2); thisNote++) {

        int snowmanDuration = 1500 / snowmanDurations[thisNote];
        tone(speakerPin, snowman[thisNote], snowmanDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = snowmanDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(speakerPin);
    }
}
