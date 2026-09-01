# Line Following Robot (LFR)

A line following robot built using an Arduino Uno, L293D motor driver, two TT motors, and a 6-sensor IR array.

## Hardware

- Arduino Uno
- L293D motor driver
- 2 × TT DC motors
- 6 × analog IR sensors

## Project Structure

```text
LFR/
├── include/
│   ├── config.h
│   ├── line_follow.h
│   ├── motor.h
│   ├── pins.h
│   └── sensor.h
├── src/
│   ├── main.cpp
│   ├── line_follow.cpp
│   ├── motor.cpp
│   ├── sensor.cpp
│   └── ...
└── platformio.ini
```

## Pin Configuration

| Function             | Arduino Pin |
| -------------------- | ----------: |
| Left motor PWM       |           9 |
| Left motor forward   |           6 |
| Left motor backward  |           7 |
| Right motor forward  |           4 |
| Right motor backward |           5 |
| Right motor PWM      |           3 |

The six IR sensors use analog inputs `A0`–`A5`.

## Configuration

Main parameters are defined in `config.h`.

| Parameter        | Purpose                       |
| ---------------- | ----------------------------- |
| `threshold`      | IR sensor threshold           |
| `lsp`            | Left motor speed coefficient  |
| `rsp`            | Right motor speed coefficient |
| `line_prop`      | Line correction strength      |
| `tsp`            | Turning speed                 |
| `node_delay`     | Node detection delay          |
| `line_end_delay` | Line-end confirmation delay   |
| `uturn_timeout`  | Maximum U-turn recovery time  |

The delay values require tuning according to the robot and track.

## Sensor System

The six IR sensors are read through analog inputs and converted into binary states using a configurable threshold.

- `0` = white
- `1` = black

The sensor states are stored in:

```text
s[0] ... s[5]
```

`sum` stores the number of active sensors:

- `sum == 0` → all sensors see white
- `sum == 6` → all sensors see black

The sensor pattern is also converted into a binary-weighted value using:

```text
1, 2, 4, 8, 16, 32
```

For example:

```text
001100 → 12
```

## Line Following

Normal line following is based on predefined sensor patterns.

The center pattern is:

```text
001100
```

The robot adjusts the left and right motor speeds according to the position of the line.

### Main Parameters

- `lsp` → left motor speed coefficient
- `rsp` → right motor speed coefficient
- `line_prop` → line correction strength
- `pos` → previous line-position correction

## Navigation

The main control loop selects a behavior based on the number of active sensors.

```text
Sensor reading
     │
     ├── sum == 0
     │     ├── flag = 'l'/'r' → directional recovery
     │     └── flag = 's'     → line-end/U-turn recovery
     │
     ├── sum == 1 or 2
     │     └── normal line following
     │
     ├── sum == 3–5
     │     └── left or right turn detection
     │
     └── sum == 6
           └── node/dead-end handling
```

## Direction Flag

`flag` stores the direction of a detected sharp turn.

```text
's' = straight
'l' = left
'r' = right
```

If the robot loses the line while a direction is stored, it rotates in that direction until the center sensors detect the line again.

The direction memory is cleared after 500 ms using `millis()`.

## Sharp-Turn Detection

For three to five active sensors, the outer and center sensors are checked to determine the turn direction.

### Left

```text
s[5] == 1
s[0] == 0
s[2] + s[3] > 0
```

### Right

```text
s[0] == 1
s[5] == 0
s[2] + s[3] > 0
```

The detected direction is stored in `flag`.

## Line-Loss and U-Turn Recovery

When all sensors see white:

```text
sum == 0
```

### Known Direction

If `flag` is `l` or `r`, the robot rotates in the stored direction until the center sensors detect the line.

### No Direction

If:

```text
sum == 0
flag == 's'
```

the robot first waits for `line_end_delay`.

This allows temporary gaps, such as dashed-line gaps, to be crossed.

If the line returns, normal line following continues.

If the line is still missing, the robot rotates until the previous line is detected.

A `uturn_timeout` prevents the robot from rotating indefinitely if the line cannot be recovered.

```text
sum == 0 + flag == 's'
          │
          ▼
   line_end_delay
          │
          ▼
      read sensors
       │       │
    line      no line
    found       │
       │        ▼
       │      rotate
       │        │
       │        ▼
       │   line detected
       │        │
       └────────┘
            │
            ▼
     normal following
```

## Intersection / Dead-End Handling

When all six sensors detect black:

```text
sum == 6
```

the robot waits for `node_delay` and reads the sensors again.

If `sum` is still `6`, the robot stops and waits until the black section clears.

If the reading changes to:

```text
sum == 0
```

the robot interprets it as a T-selection and sets:

```text
flag = 'r';
```

## Motor Control

The `motor()` function controls the two motors through the L293D.

- Positive value → forward
- Negative value → backward
- Zero → stop

Motor speed is limited to the Arduino PWM range:

```text
0–255
```

Hobby Project /n
Free for Educational Purpose
