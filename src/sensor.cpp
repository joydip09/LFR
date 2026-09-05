#include "sensor.h"
#include "config.h"
#include "motor.h"
#include "pins.h"
#include <Arduino.h>
#include <EEPROM.h>

void show_the_value() {
  reading();
  for (int i = 5; i >= 0; i--)
    Serial.print(String(s[i], 10) + " ");
  Serial.println(sensor);
}

void cal() {
  for (int i = 0; i < 6; i++) {
    maximum[i] = 0;
    minimum[i] = 1023;
  }
  motor(100, -100);
  for (int i = 0; i < 5000; i++) {
    for (int i = 0; i < 6; i++) {
      s[i] = analogRead(i);
      maximum[i] = max(maximum[i], s[i]);
      minimum[i] = min(minimum[i], s[i]);
    }
  }
  motor(0, 0);
  for (int i = 0; i < 6; i++) {
    mid[i] = (maximum[i] + minimum[i]) / 2;
    EEPROM.write(i, mid[i] / 4);
    delay(10);
    EEPROM.write(i + 6, maximum[i] / 4);
    delay(10);
    EEPROM.write(i + 12, minimum[i] / 4);
    delay(10);
  }
  EEPROM.write(20, 0xA5);
}

void reading() {
  sensor = 0; // this is to refresh initial value
  sum = 0;
  for (int i = 0; i < 6; i++) {
    s[i] = analogRead(i);
    (s[i] > mid[i])
        ? s[i] = 1
        : s[i] = 0; // conditional statement. this is to convert analog value to
                    // digital. if you want to see real analog value, then
                    // comment it. but for line follow, you must uncomment it.
    sensor +=
        s[i] *
        base[i]; // this is to merge all 6 values and imagine them in a single
                 // binary number. then i converted it into decimal number to
                 // use as final value. better search about base convertion
    sum += s[i]; // finds out total count of sensor
  }
}

void analog_reading() {
  while (1) {
    for (int i = 0; i < 6; i++) {
      s[i] = analogRead(i);
      Serial.print(String(s[i]) + " ");
    }
    Serial.println();
  }
}

void digital_reading() {
  while (1) {
    reading();
    for (int i = 5; i >= 0; i--)
      Serial.print(String(s[i]) + " ");
    Serial.println(sensor);
  }
}