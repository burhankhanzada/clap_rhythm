#define BUTTON_0_PIN 7
#define BUTTON_1_PIN 6
#define BUTTON_2_PIN 5

const int _buttonCount = 3;

const int _buttonPins[_buttonCount] = { BUTTON_0_PIN, BUTTON_1_PIN, BUTTON_2_PIN };

Button* _buttons[_buttonCount];

void buttonSetup() {
  for (int i = 0; i < _buttonCount; i++) {

    int pin = _buttonPins[i];

    Button* button = new Button(pin);

    button->singlePress(singlePress, i);
    button->doublePress(doublePress, i);
    button->longPress(longPress, i);

    _buttons[i] = button;
  }
}


void buttonLoop() {
  for (int i = 0; i < _buttonCount; i++) {
    _buttons[i]->loop();
  }
}

void singlePress(int button) {

  String str = "Button pressed: ";
  str += button;
  Serial.println(str);

  playRhythm(button);
}

void doublePress(int button) {

  String str = "Button double pressed: ";
  str += button;
  Serial.println(str);

  startDetection();
}

void longPress(int button) {

  String str = "Button long pressed: ";
  str += button;
  Serial.println(str);

  startRecording(button);
}