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

  pinMode(button, INPUT_PULLUP);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  if (EEPROM.read(20) == 0xA5) { // check if EEPROM is already calibrated
    for (int i = 0; i < 6; i++) {
      mid[i] = EEPROM.read(i) * 4;
      maximum[i] = EEPROM.read(i + 6) * 4;
      minimum[i] = EEPROM.read(i + 12) * 4;
      Serial.println(String(maximum[i]) + " " + String(mid[i]) + " " +
                     String(minimum[i]));
    }
  }
}

void loop() {
  int r = button_read();
  if (r == 1) {
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
    digitalWrite(red, HIGH);

    line_follow(); // mode-1
  } else if (r == 2) {
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    digitalWrite(green, HIGH);

    cal(); // mode-2
  } else if (r == 3) {
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);

    bluetooth(); // mode-3
  }

  /*
  else if (r == 4)
    motor(10 * lsp, 10 * rsp); // to test out if the robot is moving perfectly
                               // straight forward
  else if (r == 5)
    analog_reading(); // first connect robot with pc and open serial monitor in
                      // arduino. and then press the button accordingly to see
                      // analog reading of all sensors
  else if (r == 6)
    digital_reading(); // first connect robot with pc and open serial monitor in
                       // arduino. and then press the button accordingly to see
                       // digital reading of all sensors
  */
}