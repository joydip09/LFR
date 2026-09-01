#include "line_follow.h"
#include "config.h"
#include "motor.h"
#include "pins.h"
#include "sensor.h"
#include <Arduino.h>

void line_follow() {
  while (1) {
    reading();

    if (sum == 0) { // out of line
      if (flag != 's') {
        // (flag == 'r') ? motor(10 * lsp, -10 * rsp)
        //               : motor(-10 * lsp,
        //                       10 * rsp); // turn using the configurable
        //                       left/right speed factors
        (flag == 'r') ? motor(tsp, -tsp) : motor(-tsp, tsp);
        while (s[2] == 0 && s[3] == 0)
          reading();
        flag = 's';
      }
    }

    else if (sum == 1 || sum == 2) { // normal line
      if (sensor == 0b001100) {
        if (pos != 0) {
          (pos > 0) ? motor(-10 * lsp, 10 * rsp) : motor(10 * lsp, -10 * rsp);
          (pos > 0) ? delay(pos * 5) : delay(-pos * 5);
          pos = 0; // briefly correct back to center after high-speed wobble
                   // keep the delay below 30 ms
        }
        motor(10 * lsp, 10 * rsp);
      }

      // for right side turn correction
      else if (sensor == 0b000100)
        motor(10 * lsp, 9 * rsp); // motor(250, 230);

      else if (sensor == 0b000110) {
        if (pos < 1)
          pos = 1;
        motor(10 * lsp, (9 - line_prop) * rsp); // motor(250, 150);
      } else if (sensor == 0b000010) {
        if (pos < 2)
          pos = 2;
        motor(10 * lsp, (9 - 2 * line_prop) * rsp); // motor(250, 100);
      } else if (sensor == 0b000011) {
        if (pos < 3)
          pos = 3;
        motor(10 * lsp, (9 - 3 * line_prop) * rsp); // motor(250, 50);
      } else if (sensor == 0b000001) {
        if (pos < 4)
          pos = 4;
        motor(10 * lsp, (9 - 4 * line_prop) * rsp); // motor(250, 0);
      }

      // for left side turn correction
      else if (sensor == 0b001000)
        motor(9 * lsp, 10 * rsp); // motor(230, 250);

      else if (sensor == 0b011000) {
        if (pos > -1)
          pos = -1;
        motor((9 - line_prop) * lsp, 10 * rsp); // motor(150, 250);
      } else if (sensor == 0b010000) {
        if (pos > -2)
          pos = -2;
        motor((9 - 2 * line_prop) * lsp, 10 * rsp); // motor(100, 250);
      } else if (sensor == 0b110000) {
        if (pos > -3)
          pos = -3;
        motor((9 - 3 * line_prop) * lsp, 10 * rsp); // motor(50, 250);
      } else if (sensor == 0b100000) {
        if (pos > -4)
          pos = -4;
        motor((9 - 4 * line_prop) * lsp, 10 * rsp); // motor(0, 250);
      }
    }

    else if (sum >= 3 && sum <= 5) { // sharp turn detection
      if ((s[2] + s[3]) && s[0] == 1 && s[5] == 0)
        flag = 'r';
      if ((s[2] + s[3]) && s[0] == 0 && s[5] == 1)
        flag = 'l';

      m1 = millis();
    }

    else if (sum == 6) { // T or + intersection or black box

      delay(node_delay); // tune this value
      reading();
      if (sum == 6) { // detection of dead-end
        motor(0, 0);
        while (sum == 6)
          reading(); // breaks stop phase
      } else if (sum == 0)
        flag = 'r'; // detection of T-selection
    }
    m2 = millis();
    if (m2 - m1 > 500)
      flag = 's'; // cancel the sharp-turn recovery direction after 500 ms
  }
}