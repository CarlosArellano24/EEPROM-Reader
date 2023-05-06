#include <Arduino.h>

#define btnAdvance A2
#define btnReset A3
#define addressBus (const uint8_t[15]){ A0, A1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, A4 }

void readEEPROM(int address);
void printAddress(int address);

void setup() {
  Serial.begin(115200);

  pinMode(btnReset, INPUT);
  pinMode(btnAdvance, INPUT);

  for (auto& pin : addressBus)
    pinMode(pin, OUTPUT);

  readEEPROM(0x000);
}

void loop() {
  static unsigned short address = 0x000;
  static bool prevStateBtnAdvance = 0;

  if (digitalRead(btnReset)) {
    address = 0x000;
    readEEPROM(address);
  }
  if (digitalRead(btnAdvance) && prevStateBtnAdvance == 0) {
    address++;
    readEEPROM(address);
  }
  
  prevStateBtnAdvance = digitalRead(btnAdvance);
  delay(100);
}

void readEEPROM(int address) {
  for (int i = 0; i < 15; i++){
    const uint8_t& pin = addressBus[i];
    digitalWrite(pin, address & (0x1 << i));
  }
  printAddress(address);
  return;
}

void printAddress(int address) {
  char debug[20] = {};
  sprintf(debug, "Current address: 0x%X (%d)", address, address);
  Serial.println(debug);
}