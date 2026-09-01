#include "bluetooth.h"
#include "config.h"
#include "motor.h"
#include "pins.h"
#include <Arduino.h>

void bluetooth() {
  int s = 10; // for adjustable speed system
  char x;     // for storing receiving incoming data

  while (1) { // infinite loop
    if (Serial.available() > 0) {
      x = Serial.read();
      Serial.println(x);

      if (x == 'F')
        motor(s * lsp, s * rsp);
      else if (x == 'B')
        motor(-s * lsp, -s * rsp);
      else if (x == 'L')
        motor(-s * lsp, s * rsp);
      else if (x == 'R')
        motor(s * lsp, -s * rsp);
      else if (x == 'G')
        motor(0, s * rsp);
      else if (x == 'I')
        motor(s * lsp, 0);
      else if (x == 'H')
        motor(0, -s * rsp);
      else if (x == 'J')
        motor(-s * lsp, 0);
      else if (x == 'S')
        motor(0, 0);
      else if (x == 'U')
        digitalWrite(red, HIGH);
      else if (x == 'u')
        digitalWrite(red, LOW);
      else if (x == '0')
        s = 0;
      else if (x == '1')
        s = 1;
      else if (x == '2')
        s = 2;
      else if (x == '3')
        s = 3;
      else if (x == '4')
        s = 4;
      else if (x == '5')
        s = 5;
      else if (x == '6')
        s = 6;
      else if (x == '7')
        s = 7;
      else if (x == '8')
        s = 8;
      else if (x == '9')
        s = 9;
      else if (x == 'q')
        s = 10;
    }
  }
}
