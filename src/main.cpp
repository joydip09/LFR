#include "config.h"
#include "line_follow.h"
#include "motor.h"
#include "pins.h"
#include "sensor.h"
#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  pinMode(left_motor_forward, OUTPUT);
  pinMode(left_motor_backward, OUTPUT);
  pinMode(right_motor_forward, OUTPUT);
  pinMode(right_motor_backward, OUTPUT);

  // motor(150,150);
}

void loop() {
  reading();
  lineFollow();
  // for (byte i = 0; i<6 ; i++) {
  //   Serial.print(s[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
  // delay(200);

  // if (s[0] == 0 && s[1] == 0 && s[2] == 1 && s[3] == 1 && s[4] == 0 && s[5]
  // ==0) motor(120, 120); else { motor(0,0);}

  // if (sensor == 0b001100) motor(lsp, rsp); //12
  // else if (sensor == 0b001000) motor(200, 250); //8
  // else if (sensor == 0b100000) motor(0, 250); //32
}