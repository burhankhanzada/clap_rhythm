#include <EEPROM.h>

void loadRhythmsFromEEPROM() {
  int address = 0;
  for (int i = 0; i < maxRhythms; i++) {
    EEPROM.get(address, rhythms[i]);
    address += sizeof(Rhythm);
  }
}

void saveRhythmToEEPROM(int slot, Rhythm rhythm) {
  if (slot >= 0 && slot < maxRhythms) {
    int address = slot * sizeof(Rhythm);
    EEPROM.put(address, rhythm);
  }
}