/*
 * ------------------------------------------------------------
 * Project : Solar Powered Grid Integrated EV Charging Module
 * File    : Smart_Grid_Load_Monitor.ino
 * Author  : Sriram Chodavaram
 *
 * Description:
 * This program monitors AC voltage and current using a voltage
 * sensor and ACS712 current sensor. If the voltage goes out of
 * the safe range or the current exceeds the preset limit, the
 * relay disconnects the load and a buzzer alarm is activated.
 * The measured values are displayed on a 16x2 LCD and sent to
 * the Serial Monitor.
 * ------------------------------------------------------------
 */

#include <LiquidCrystal.h>
#include "ACS712.h"

/*----------------------- LCD Configuration -----------------------*/
// LCD Pins: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

/*---------------------- Pin Configuration ------------------------*/
const int RELAY_PIN = 6;
const int BUZZER_PIN = A2;
const int VOLTAGE_SENSOR_PIN = A1;
const int CURRENT_SENSOR_PIN = A0;

/*-------------------- Voltage Divider Values ---------------------*/
const float R1 = 30000.0;
const float R2 = 7500.0;

/*---------------------- Safety Thresholds -------------------------*/
const float CURRENT_HIGH = 0.05;     // 50 mA
const float VOLTAGE_LOW = 200.0;     // Minimum Safe Voltage
const float VOLTAGE_HIGH = 250.0;    // Maximum Safe Voltage

/*----------------------- Global Variables ------------------------*/
float inputVoltage = 0.0;
float outputVoltage = 0.0;
float loadCurrent = 0.0;
int adcValue = 0;

unsigned long lastUpdate = 0;
int displayStep = 0;

/*--------------------- Current Sensor Object ---------------------*/
ACS712 ACS(CURRENT_SENSOR_PIN, 5.0, 1023, 100);

/*=============================================================
                        SETUP FUNCTION
=============================================================*/
void setup()
{
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    // Startup Beep
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);

    // LCD Initialization
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("LOAD MONITOR");
    lcd.setCursor(0, 1);
    lcd.print("SMART GRID");

    delay(2000);

    Serial.begin(9600);

    // Automatically calibrate ACS712 zero point
    ACS.autoMidPoint();
}

/*=============================================================
                  READ CURRENT FROM ACS712
=============================================================*/
float readCurrent()
{
    float totalCurrent = 0;

    for (int i = 0; i < 100; i++)
    {
        totalCurrent += ACS.mA_AC() / 1000.0;
        delay(5);
    }

    return totalCurrent / 100.0;
}

/*=============================================================
                  READ INPUT VOLTAGE
=============================================================*/
float readVoltage()
{
    adcValue = analogRead(VOLTAGE_SENSOR_PIN);

    outputVoltage = (adcValue * 5.0) / 1023.0;

    inputVoltage = outputVoltage / (R2 / (R1 + R2));

    return inputVoltage;
}

/*=============================================================
                  BUZZER ALARM PATTERN
=============================================================*/
void alarmPattern()
{
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);

    digitalWrite(BUZZER_PIN, LOW);
    delay(200);

    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);

    digitalWrite(BUZZER_PIN, LOW);
}

/*=============================================================
                        MAIN PROGRAM
=============================================================*/
void loop()
{
    // Read Sensor Values
    loadCurrent = readCurrent();
    inputVoltage = readVoltage();

    // Protection Logic
    if (loadCurrent > CURRENT_HIGH ||
        inputVoltage < VOLTAGE_LOW ||
        inputVoltage > VOLTAGE_HIGH)
    {
        digitalWrite(RELAY_PIN, LOW);
        alarmPattern();
    }
    else
    {
        digitalWrite(RELAY_PIN, HIGH);
    }

    // LCD Update Every 2 Seconds
    if (millis() - lastUpdate > 2000)
    {
        switch (displayStep)
        {
            // Display Current
            case 0:
                lcd.clear();
                lcd.print("Current:");
                lcd.print(loadCurrent * 1000, 0);
                lcd.print("mA");

                lcd.setCursor(0, 1);

                if (loadCurrent > CURRENT_HIGH)
                    lcd.print("OVERLOAD");
                else
                    lcd.print("NORMAL");

                break;

            // Display Voltage
            case 1:
                lcd.clear();

                lcd.print("Voltage:");
                lcd.print(inputVoltage, 1);
                lcd.print("V");

                lcd.setCursor(0, 1);

                if (inputVoltage < VOLTAGE_LOW)
                    lcd.print("LOW VOLTAGE");
                else if (inputVoltage > VOLTAGE_HIGH)
                    lcd.print("HIGH VOLTAGE");
                else
                    lcd.print("STABLE");

                break;

            // Display Relay Status
            case 2:
                lcd.clear();

                lcd.print("I:");
                lcd.print(loadCurrent * 1000, 0);
                lcd.print("mA");

                lcd.setCursor(0, 1);

                lcd.print("Relay:");

                if (digitalRead(RELAY_PIN))
                    lcd.print("ON");
                else
                    lcd.print("OFF");

                break;
        }

        // Serial Output
        Serial.print("Current : ");
        Serial.print(loadCurrent * 1000, 0);
        Serial.print(" mA");

        Serial.print(" | Voltage : ");
        Serial.print(inputVoltage, 1);
        Serial.print(" V");

        Serial.print(" | Relay : ");

        if (digitalRead(RELAY_PIN))
            Serial.println("ON");
        else
            Serial.println("OFF");

        displayStep = (displayStep + 1) % 3;
        lastUpdate = millis();
    }
}
