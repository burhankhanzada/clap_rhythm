const long unsigned rhythmTimeout = 2000;

enum RhythmState {
  RHYTHM_IDLE,
  RHYTHM_RECORDING,
  RHYTHM_DETECTING,
  RHYTHM_RECOGNIZING,
};

RhythmState _currentRhythmState = RHYTHM_IDLE;

int _slot;
int _rhythmLedPin;
Rhythm _recordingRhythm;

unsigned long _startTime;
unsigned long _lastClapTime;
bool _firstClapDetected = false;

void rhythmLoop() {

  switch (_currentRhythmState) {
    case RHYTHM_RECORDING:
    case RHYTHM_DETECTING:
      recordRhythm();
      break;
    case RHYTHM_RECOGNIZING:
      recognizeRhythm();
      break;
  }
}

void startRecording(int slot) {
  _slot = slot;
  _startTime = millis();
  _rhythmLedPin = getLEDPin(slot);
  _recordingRhythm.intervalCount = 0;
  _firstClapDetected = false;
  Serial.println("Rhythm recording start");
  _currentRhythmState = RHYTHM_RECORDING;
}

void stopRecording() {

  if (_currentRhythmState == RHYTHM_DETECTING) {
    _currentRhythmState = RHYTHM_RECOGNIZING;
    return;
  }

  _currentRhythmState = RHYTHM_IDLE;
  saveRhythm();
}

void startDetection() {

  startLedCycle();
  _startTime = millis();
  _recordingRhythm.intervalCount = 0;
  _firstClapDetected = false;
  Serial.println("Rhythm detection start");
  _currentRhythmState = RHYTHM_DETECTING;
}

void stopDetection() {
  _currentRhythmState = RHYTHM_IDLE;
  setLedOff();
}

void saveRhythm() {

  rhythms[_slot] = _recordingRhythm;
  saveRhythmToEEPROM(_slot, _recordingRhythm);
}

void recordRhythm() {

  unsigned long currentTime = millis();

  digitalWrite(_rhythmLedPin, HIGH);

  if (!_firstClapDetected && (currentTime - _startTime > rhythmTimeout)) {
    Serial.println("Rhythm timeout reached (no claps detected), nothing saved");
    stopRecording();
    return;
  }

  if (_firstClapDetected) {

    if (_recordingRhythm.intervalCount >= maxClaps - 1) {
      Serial.println("Max claps reached, rhythm saved");
      stopRecording();
      return;
    }

    else if (currentTime - _lastClapTime > rhythmTimeout) {
      Serial.println("Rhythm timeout reached (after claps), rhythm saved");
      stopRecording();
      return;
    }
  }

  bool clapDetected = digitalRead(SOUND_PIN) == HIGH;

  if (clapDetected) {

    if (!_firstClapDetected) {
      _firstClapDetected = true;
      _lastClapTime = currentTime;
      _recordingRhythm.intervalCount++;
      _recordingRhythm.intervals[0] = 0;
      Serial.println("Clap 0 at interval 0");
    }

    else if (currentTime - _lastClapTime > clapDuration) {

      int count = _recordingRhythm.intervalCount;

      int interval = currentTime - _lastClapTime;

      _lastClapTime = currentTime;

      _recordingRhythm.intervalCount++;
      _recordingRhythm.intervals[count] = interval;

      String str = "Clap ";
      str += count;
      str += " interval ";
      str += interval;

      Serial.println(str);
    }
  }
}

void recognizeRhythm() {

  unsigned long currentTime = millis();

  int count = _recordingRhythm.intervalCount;

  if (currentTime - _startTime > rhythmTimeout && count > 0) {

    int matchedSlot = matchRhythm();

    if (matchedSlot != -1) {

      String str = "Matched rhythm: ";
      str += matchedSlot;

      Serial.println("Rhythm detection stop");
      Serial.println(str);

      stopDetection();
      playRhythm(matchedSlot);
      return;
    }
  }

  Serial.println("Rhythm detection stop, no match found");

  stopDetection();
  startLedBlink();
}

int matchRhythm() {

  if (_recordingRhythm.intervalCount < 1) return -1;

  for (int i = 0; i < maxRhythms; i++) {

    Rhythm rhythm = rhythms[i];

    if (rhythm.intervalCount < 1 || rhythm.intervalCount != _recordingRhythm.intervalCount) continue;

    bool match = true;

    for (int j = 0; j < _recordingRhythm.intervalCount; j++) {

      long x = rhythm.intervals[j];
      long y = _recordingRhythm.intervals[j];

      if (y == 0) {  // Avoid division by zero
        if (x != 0) {
          match = false;
          break;
        }
        continue;
      }

      float errorRatio = abs(x - y) / (float)y;

      String str = "ErrorRatio ";
      str += errorRatio;
      str += " at ";
      str += j;

      Serial.println(str);

      if (errorRatio > 0.2) {  // 20% tolerance
        match = false;
        break;
      }
    }

    if (match) return i;
  }

  return -1;
}