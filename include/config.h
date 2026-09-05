#pragma once

#include <Arduino.h>

int s[6];
int threshold = 300; // tune this value for sensor sensitivity or use
                     // calibration mode to set it automatically
int mid[6] = {threshold, threshold, threshold, threshold, threshold, threshold};

int maximum[6], minimum[6];

long base[6] = {1, 2, 4, 8, 16, 32}; // this is for binary to decimal conversion
// long base[6] = {1, 10, 100, 1000, 10000, 100000};
long sensor; // this is to store final value after binary conversion
int sum;     // this is to store the count of detected sensor

int lsp = 10, rsp = 10; // left and right motor speed coefficient
float line_prop = 1;
int pos;
char flag = 's';         // s = straight, l = left, r = right
int tsp = 100;           // turn speed
int node_delay = 30;     // tune this value for node delay
int line_end_delay = 30; // tune this value for line end delay

uint32_t m1, m2, m3;
uint16_t uturn_timeout = 1500;
uint16_t stop_timer = 1500;