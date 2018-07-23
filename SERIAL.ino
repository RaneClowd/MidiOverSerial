/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/

// constants won't change. They're used here to set pin numbers:
const int button1Pin = 2;     // the number of the pushbutton pin
const int button2Pin = 3;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int button1State = 0;         // variable for reading the pushbutton status
int lastButton1State = 0;
int button2State = 0;         // variable for reading the pushbutton status
int lastButton2State = 0;

#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
  button1State = digitalRead(button1Pin);

  if (button1State != lastButton1State) {
    lastButton1State = button1State;
    
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (button1State == LOW) {
      // turn LED on:
      digitalWrite(ledPin, HIGH);
      noteOn(40);
    } else {
      // turn LED off:
      digitalWrite(ledPin, LOW);
      noteOff(40);
    }
  }

  
  // read the state of the pushbutton value:
  button2State = digitalRead(button2Pin);

  if (button2State != lastButton2State) {
    lastButton2State = button2State;
    
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (button2State == LOW) {
      // turn LED on:
      digitalWrite(ledPin, HIGH);
      noteOn(50);
    } else {
      // turn LED off:
      digitalWrite(ledPin, LOW);
      noteOff(50);
    }
  }
}

void noteOn(int note) {
  Serial.write(NOTE_ON_CMD);
  Serial.write(note);
  Serial.write(NOTE_VELOCITY);
}

void noteOff(int note) {
  Serial.write(NOTE_OFF_CMD);
  Serial.write(note);
  Serial.write(NOTE_VELOCITY);
}


