const int pianoLiveWires[] = {A5, A4, A3, A2, A1, A0, 13}
const int numLiveWires = 7;
const int pianoReadWires[] = {12, 11, 10, 9, 8, 7};
const int numReadWires = 6;


const int noteMap[numLiveWires][numReadWires] = {{ 65,  0, 61, 62, 63, 64},
                                                 { 59, 60, 55, 56, 57, 58},
                                                 { 53, 54, 49, 50, 51, 52},
                                                 { 47, 48, 43, 44, 45, 46},
                                                 { 41, 42, 37, 38, 39, 40},
                                                 { 35, 36, 31, 32, 33, 34},
                                                 {  0, 30,  0,  0,  0,  0}}


int activePianoWire = 0;
int wireActivity[numLiveWires][numReadWires];

void setup() {
  for (int i = 0; i < numLiveWires; i++) {
    pinMode(pianoLiveWires[i], OUTPUT);
  }
  for (int i = 0; i < numReadWires; i++) {
    pinMode(pianoReadWires[i], INPUT);
  }

  for (int i = 0; i < numLiveWires; i++) {
    for (int j = 0; j < numReadWires; j++) {
      wireActivity[i][j] = LOW;
    }
  }

  setLiveWire(activePianoWire, HIGH);

  Serial.begin(9600);
}

void loop() {
  // Compare note states (active/inactive) to wire state, when diff found send signal for that note
  int wireState;
  for (int readIndex = 0; readIndex < numReadWires; readIndex++) {
    wireState = digitalRead(pianoReadWires[readIndex]);
    if (wireState != wireActivity[activePianoWire][readIndex]) {
      wireActivity[activePianoWire][readIndex] = wireState;
      sendWireStateChange(activePianoWire, readIndex, wireState);
    }
  }

  activateNextPianoWire();
}

void activateNextPianoWire() {
  setLiveWire(activePianoWire, LOW);
  
  activePianoWire += 1;
  if (activePianoWire == numLiveWires) {
    activePianoWire = 0;
  }

  setLiveWire(activePianoWire, HIGH);
}

void setLiveWire(int wireIndex, int val) {
  digitalWrite(pianoLiveWires[wireIndex], val);
}

void sendWireStateChange(int liveIndex, int readIndex, int state) {
  int note = noteMap[liveIndex][readIndex];
  if (state == HIGH) {
    noteOn(note);
  } else {
    noteOff(note);
  }
}

#define NOTE_ON_CMD 0x90
#define NOTE_OFF_CMD 0x80
#define NOTE_VELOCITY 127

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
