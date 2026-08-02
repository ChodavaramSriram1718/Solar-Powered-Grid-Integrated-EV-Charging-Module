#include <LiquidCrystal.h>
#include "ACS712.h"

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

int relay = 6;
const int buzzer = A2;
const int voltageSensor = A1;
const int currentSensor = A0;

float vOUT = 0.0, vIN = 0.0;
float R1 = 30000.0, R2 = 7500.0;
int value = 0;
float current = 0.0;

const float CURRENT_HIGH = 0.05;
const float VOLTAGE_LOW = 200.0;
const float VOLTAGE_HIGH = 250.0;

ACS712 ACS(currentSensor, 5.0, 1023, 100);

unsigned long lastUpdate = 0;
int displayStep = 0;

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);

  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LOAD MONITOR");
  lcd.setCursor(0, 1);
  lcd.print("SMART GRID");
  delay(2000);

  Serial.begin(9600);
  ACS.autoMidPoint();
}

float readCurrent() {
  float total = 0.0;
  for (int i = 0; i < 100; i++) {
    total += ACS.mA_AC() / 1000.0;
    delay(5);
  }
  return total / 100.0;
}

float readVoltage() {
  value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 1023.0;
  return vOUT / (R2 / (R1 + R2));
}

void alarmPattern() {
  digitalWrite(buzzer, HIGH); delay(300);
  digitalWrite(buzzer, LOW); delay(200);
  digitalWrite(buzzer, HIGH); delay(300);
  digitalWrite(buzzer, LOW);
}

void loop() {

  current = readCurrent();
  vIN = readVoltage();

  if (current > CURRENT_HIGH || vIN < VOLTAGE_LOW || vIN > VOLTAGE_HIGH) {
    digitalWrite(relay, LOW);
    alarmPattern();
  } else {
    digitalWrite(relay, HIGH);
  }

  if (millis() - lastUpdate > 2000) {

    switch(displayStep) {

      case 0:
        lcd.clear();
        lcd.print("Current:");
        lcd.print(current * 1000, 0);
        lcd.print("mA");
        lcd.setCursor(0,1);
        lcd.print(current > CURRENT_HIGH ? "OVERLOAD" : "NORMAL");
        break;

      case 1:
        lcd.clear();
        lcd.print("Voltage:");
        lcd.print(vIN, 1);
        lcd.print("V");
        lcd.setCursor(0,1);
        if (vIN < VOLTAGE_LOW) lcd.print("LOW VOLTAGE");
        else if (vIN > VOLTAGE_HIGH) lcd.print("HIGH VOLTAGE");
        else lcd.print("STABLE");
        break;

      case 2:
        lcd.clear();
        lcd.print("I:");
        lcd.print(current * 1000, 0);
        lcd.print("mA");
        lcd.setCursor(0,1);
        lcd.print("R:");
        lcd.print(digitalRead(relay) ? "ON" : "OFF");
        break;
    }

    Serial.println("I:" + String(current * 1000,0) + "mA | V:" +
                   String(vIN,1) + "V | Relay:" +
                   String(digitalRead(relay) ? "ON" : "OFF"));

    displayStep = (displayStep + 1) % 3;
    lastUpdate = millis();
  }
}
