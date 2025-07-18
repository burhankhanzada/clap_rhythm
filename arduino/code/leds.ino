#define RED_LED_PIN 12
#define YELLOW_LED_PIN 11
#define GREEN_LED_PIN 10

const int _LED_COUNT = 3;

const int _LED_PINS[_LED_COUNT] = { RED_LED_PIN, YELLOW_LED_PIN, GREEN_LED_PIN };

const unsigned long _CYCLE_DURATION = 250;

enum _LedState {
  LED_OFF,
  LED_BLINK,
  LED_CYCLE,
};

_LedState _currentLedState;

int _blinkCount = 0;
int _currentLedIndex = 0;
unsigned long _lastLedTime = 0;

void ledInit() {
  for (int i = 0; i < _LED_COUNT; i++) {
    int pin = _LED_PINS[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  setLedOff();
}

void ledUpdate() {

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

  if (index < 0 || index >= _LED_COUNT) {
    Serial.println("Invalid LED index");
    return -1;
  }

  return _LED_PINS[index];
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
  for (int i = 0; i < _LED_COUNT; i++) {
    int pin = _LED_PINS[i];
    digitalWrite(pin, state);
  }
}

int off() {
  setAllLedsState(LOW);
}

void blink() {

  unsigned long currentTime = millis();

  if (_blinkCount < 4) {

    if (currentTime - _lastLedTime >= _CYCLE_DURATION) {

      setAllLedsState(_blinkCount % 2 == 0 ? HIGH : LOW);

      _blinkCount++;
      _lastLedTime = currentTime;
    }
  }

  else setLedOff();
}

void cycle() {

  unsigned long currentTime = millis();

  if (currentTime - _lastLedTime >= _CYCLE_DURATION) {

    off();

    int pin = getLEDPin(_currentLedIndex);

    digitalWrite(pin, HIGH);

    _lastLedTime = currentTime;

    _currentLedIndex = (_currentLedIndex + 1) % _LED_COUNT;
  }
}