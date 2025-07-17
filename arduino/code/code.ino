#define SOUND_PIN 2
#define BUZZER_PIN 3

const int maxRhythms = 3;
const int maxClaps = 10;
const long unsigned clapDuration = 100;

struct Rhythm {
  int intervalCount;
  int intervals[maxClaps - 1];
};

Rhythm rhythms[maxRhythms];

void setup() {

  Serial.begin(9600);

  pinMode(SOUND_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  ledSetup();
  buttonSetup();

  loadRhythmsFromEEPROM();
}

void loop() {
  ledLoop();
  buttonLoop();
  rhythmLoop();
  playbackLoop();
}