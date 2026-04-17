#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define LED_PIN 23
#define IR_PIN 36   // ADC pin

void setup() {
  Serial.begin(115200);
  SerialBT.begin("BlewToT");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(IR_PIN, INPUT);
}

void loop() {

  // ===== BLUETOOTH LED CONTROL =====
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();

    if (cmd == "ON") {
      digitalWrite(LED_PIN, HIGH);
      SerialBT.println("LED ON");
    }
    else if (cmd == "OFF") {
      digitalWrite(LED_PIN, LOW);
      SerialBT.println("LED OFF");
    }
  }

  // ===== IR SENSOR READ ONLY =====
  int irValue = analogRead(IR_PIN);

  if (irValue < 2000) {
    SerialBT.println("IR: OBJECT DETECTED");
  } else {
    SerialBT.println("IR: CLEAR");
  }

  delay(500);
}