#define RED_LED_PIN 12
#define YELLOW_LED_PIN 11
#define GREEN_LED_PIN 10

const int _ledCount = 3;
const int _ledPins[_ledCount] = { RED_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN };

const unsigned long _cycleDuration = 250;

enum _LedState {
  LED_OFF,
  LED_CYCLE,
  LED_BLINK,
};

_LedState _currentLedState;

int _blinkCount = 0;
int _currentLedIndex = 0;
unsigned long _lastLedTime = 0;

void ledSetup() {
  for (int i = 0; i < _ledCount; i++) {
    int pin = _ledPins[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  setLedOff();
}

void ledLoop() {

  switch (_currentLedState) {
    case LED_OFF:
      off();
      break;
    case LED_CYCLE:
      cycle();
      break;
    case LED_BLINK:
      blink();
      break;
  }
}

int getLEDPin(int index) {

  if (index < 0 || index >= _ledCount) {
    Serial.println("Invalid LED index");
    return -1;
  }

  return _ledPins[index];
}


void setLedOff() {
  _currentLedState = LED_OFF;
}

void startLedBlink() {
  _blinkCount = 0;
  _currentLedState = LED_BLINK;
}

void startLedCycle() {
  _currentLedIndex = 0;
  _currentLedState = LED_CYCLE;
}

int setAllLedsState(int state) {
  for (int i = 0; i < _ledCount; i++) {
    int pin = _ledPins[i];
    digitalWrite(pin, state);
  }
}

int off() {
  setAllLedsState(LOW);
}

void blink() {

  unsigned long currentTime = millis();

  if (_blinkCount < 4) {

    if (currentTime - _lastLedTime >= _cycleDuration) {

      setAllLedsState(_blinkCount % 2 == 0 ? HIGH : LOW);

      _blinkCount++;
      _lastLedTime = currentTime;
    }
  }

  else setLedOff();
}

void cycle() {

  unsigned long currentTime = millis();

  if (currentTime - _lastLedTime >= _cycleDuration) {

    off();

    int pin = getLEDPin(_currentLedIndex);

    digitalWrite(pin, HIGH);

    _lastLedTime = currentTime;

    _currentLedIndex = (_currentLedIndex + 1) % _ledCount;
  }
}