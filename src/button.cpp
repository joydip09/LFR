#include "button.h"
#include "config.h"
#include "pins.h"
#include <Arduino.h>

int button_read() {
  int cl = 0;
p:
  int t = 0;
  if (digitalRead(button) == 0) {
    while (digitalRead(button) == 0) {
      delay(1);
      t++;
    }
    if (t > 15) {
      t = 0;
      cl++;
      while (digitalRead(button) == HIGH) {
        delay(1);
        t++;
        if (t > 1000)
          return cl;
      }
      goto p;
    }
  }
  return cl;
}
