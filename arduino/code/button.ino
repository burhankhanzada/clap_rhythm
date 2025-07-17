typedef void (*Callback)(void *);

class Button {
public:

  Button(int pin)
    : _pin(pin) {
    pinMode(pin, INPUT_PULLUP);
  }

  void singlePress(Callback callBack, void *parameter) {
    _singlePressParam = parameter;
    _singlePressCallback = callBack;
  }

  void doublePress(Callback callBack, void *parameter) {
    _doublePressParam = parameter;
    _doublePressCallback = callBack;
  }

  void longPress(Callback callBack, void *parameter) {
    _longPressParam = parameter;
    _longPressCallback = callBack;
  }

  void loop() {
    
    unsigned long currentTime = millis();

    bool isPressed = digitalRead(_pin) == LOW;

    switch (_currentState) {
      case IDLE:

        if (isPressed) {
          _pressCount = 0;
          _pressStartTime = currentTime;
          _currentState = PRESSED;
        }
        break;

      case PRESSED:

        if (!isPressed) {
          _pressStartTime = currentTime;
          _currentState = RELEASED;
        }

        else if (currentTime - _pressStartTime > _longPressDuration && _longPressCallback) {
          _longPressCallback(_longPressParam);
          _currentState = WAIT_RELEASE;
        }
        break;

      case RELEASED:

        _pressCount++;
        _pressStartTime = currentTime;
        _currentState = WAIT_DOUBLE;
        break;

      case WAIT_DOUBLE:

        if (isPressed) {
          _pressStartTime = currentTime;
          _currentState = PRESSED;
        }

        if (currentTime - _pressStartTime > _doublePressWindow) {

          if (_pressCount == 1 && _singlePressCallback) {
            _singlePressCallback(_singlePressParam);
          }

          else if (_pressCount == 2 && _doublePressCallback) {
            _doublePressCallback(_doublePressParam);
          }

          _currentState = IDLE;
        }

        break;

      case WAIT_RELEASE:

        if (!isPressed) {
          _currentState = IDLE;
        }
        break;
    }
  }

private:
  const int _pin;

  enum _State {
    IDLE,
    PRESSED,
    RELEASED,
    WAIT_DOUBLE,
    WAIT_RELEASE
  };

  _State _currentState = IDLE;

  const unsigned long _doublePressWindow = 400;
  const unsigned long _longPressDuration = 800;

  int _pressCount = 0;
  unsigned long _lastDebounceTime = 0;
  unsigned long _pressStartTime = 0;

  void *_singlePressParam = nullptr;
  void *_doublePressParam = nullptr;
  void *_longPressParam = nullptr;

  Callback _singlePressCallback = nullptr;
  Callback _doublePressCallback = nullptr;
  Callback _longPressCallback = nullptr;
};