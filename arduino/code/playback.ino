const long unsigned _buzzerTone = 2000;

enum PlaybackState {
  PLAYBACK_IDLE,
  PLAYBACK_PLAYING,
};

PlaybackState _playbackState = PLAYBACK_IDLE;

Rhythm _playbackRhythm;

int _playbackIndex;
int _playbackLedPin;
unsigned long _playbackStartTime;

void playRhythm(int slot) {
  if (slot < 0 || slot >= maxRhythms) {
    Serial.println("Invalid slot");
    return;
  }

  if (rhythms[slot].intervalCount < 1) {
    Serial.println("No Rhythm stored");
    return;
  }

  Serial.println("Rhythm playing start");

  _playbackRhythm = rhythms[slot];
  _playbackLedPin = getLEDPin(slot);

  playFirstClap();

  _playbackIndex = 1;
  _playbackState = PLAYBACK_PLAYING;
  _playbackStartTime = millis();
}

void playFirstClap() {
  tone(BUZZER_PIN, _buzzerTone, clapDuration);
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
    tone(BUZZER_PIN, _buzzerTone, clapDuration);
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