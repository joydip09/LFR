#include "bluetooth.h"
#include "button.h"
#include "config.h"
#include "line_follow.h"
#include "motor.h"
#include "pins.h"
#include "sensor.h"
#include <Arduino.h>
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);
  pinMode(left_motor_forward, OUTPUT);
  pinMode(left_motor_backward, OUTPUT);
  pinMode(left_motor_speed, OUTPUT);
  pinMode(right_motor_forward, OUTPUT);
  pinMode(right_motor_backward, OUTPUT);
  pinMode(right_motor_speed, OUTPUT);

  for (int i = 0; i < 6; i++) {
    mid[i] = EEPROM.read(i) * 4;
    maximum[i] = EEPROM.read(i + 6) * 4;
    minimum[i] = EEPROM.read(i + 12) * 4;
    Serial.println(String(maximum[i]) + " " + String(mid[i]) + " " +
                   String(minimum[i]));
  }
}

void loop() {
  int r = button_read();
  if (r == 1)
    line_follow();
  else if (r == 2)
    cal();
  else if (r == 3)
    bluetooth();
}