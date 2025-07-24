const long unsigned _buzzerTone = 2000;

enum PlaybackState {
  PLAYBACK_IDLE,
  PLAYBACK_PLAYING,
};

PlaybackState _playbackState = PLAYBACK_IDLE;

Rhythm _playbackRhythm;

int _playbackIndex = -1;
int _playbackLedPin = -1;
unsigned long _playbackStartTime = 0;

void playRhythm(int slot) {
  if (slot < 0 || slot >= MAX_RHYTHMS) {
    Serial.println("Invalid slot");
    return;
  }

  if (rhythms[slot].intervalCount < 1) {
    Serial.println("No Rhythm stored");
    startLedBlink();
    return;
  }

  Serial.println("Rhythm playing start");

  _playbackRhythm = rhythms[slot];
  _playbackLedPin = getLEDPin(slot);

  playFirstClap(); // Play first clap because its interval is always 0

  _playbackState = PLAYBACK_PLAYING;
  _playbackStartTime = millis();
}

void playFirstClap() {
  _playbackIndex = 1; // Start from 1 index because 0 index interval is always 0
  tone(BUZZER_PIN, _buzzerTone, CLAP_DURATION);
  digitalWrite(_playbackLedPin, HIGH);
  Serial.println("Interval 0 at 0");
}

void playbackLoop() {
  if (_playbackState == PLAYBACK_IDLE) return;

  digitalWrite(_playbackLedPin, LOW);

  if (_playbackIndex >= _playbackRhythm.intervalCount) {
    // Playback complete
    _playbackState = PLAYBACK_IDLE;
    Serial.println("Rhythm playing stop");
    return;
  }

  unsigned long currentTime = millis();

  unsigned long interval = _playbackRhythm.intervals[_playbackIndex];

  if (currentTime - _playbackStartTime >= interval) {

    // Play next clap
    tone(BUZZER_PIN, _buzzerTone, CLAP_DURATION);
    digitalWrite(_playbackLedPin, HIGH);

    String str = "Interval ";
    str += interval;
    str += " at ";
    str += _playbackIndex;
    Serial.println(str);

    _playbackIndex++;

    _playbackStartTime = currentTime;
  }
}