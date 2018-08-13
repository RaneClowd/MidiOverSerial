const int pianoReadWires[] = {A5, A4, A3, A2, A1, A0, 13}
const int numReadWires = 7;
const int pianoLiveWires[] = {12, 11, 10, 9, 8, 7};
const int numLiveWires = 6;


const int noteMap[numReadWires][numLiveWires] = {{ 65,  0, 61, 62, 63, 64},
                                                 { 59, 60, 55, 56, 57, 58},
                                                 { 53, 54, 49, 50, 51, 52},
                                                 { 47, 48, 43, 44, 45, 46},
                                                 { 41, 42, 37, 38, 39, 40},
                                                 { 35, 36, 31, 32, 33, 34},
                                                 {  0, 30,  0,  0,  0,  0}}

int activePianoWire = 0;
int wireActivity[numReadWires][numLiveWires];

void setup() {
  for (int i = 0; i < numLiveWires; i++) {
    pinMode(pianoLiveWires[i], OUTPUT);
  }
  for (int i = 0; i < numReadWires; i++) {
    pinMode(pianoReadWires[i], INPUT);
  }

  for (int i = 0; i < numReadWires; i++) {
    for (int j = 0; j < numLiveWires; j++) {
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
    if (wireState != wireActivity[readIndex][activePianoWire]) {
      wireActivity[readIndex][activePianoWire] = wireState;
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
  int note = noteMap[readIndex][liveIndex];
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
  sendCommand(NOTE_ON_CMD, note);
}

void noteOff(int note) {
  sendCommand(NOTE_OFF_CMD, note);
}

void sendCommand(int code, int note) {
  Serial.write(code);
  Serial.write(note);
  Serial.write(NOTE_VELOCITY);
}
