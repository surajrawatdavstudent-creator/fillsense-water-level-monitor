# System Design

## 1. Main idea

The HC-SR04 is placed above the container and points towards the water surface. It measures the gap between the sensor and the water.

The sensor does not touch the water.

## 2. Distance calculation

The sensor sends a sound pulse and measures the echo time.

The Arduino uses this formula:

Distance in cm = (echo time in microseconds × 0.0343) ÷ 2

The value is divided by 2 because the sound travels to the water and then returns to the sensor.

## 3. Reading filter

The program takes three readings. It sorts the valid readings and uses the middle value when all three are valid. This is a simple median filter. It can reduce the effect of one unusual reading.

At least two valid readings are needed. If there are not enough valid readings, the program shows a sensor message instead of using a false distance.

## 4. Alert levels

| Condition | System state |
|---|---|
| Gap above 10 cm | SAFE |
| Gap from 5 cm to 10 cm | WARNING |
| Gap below 5 cm | CRITICAL |

The warning buzzer gives one short beep every 5 seconds while a rise in the water level is detected.

At the critical level, the buzzer sounds continuously while a rise is detected.

## 5. Stationary-level check

The program compares new distance readings with an earlier reading.

When the gap becomes smaller by at least 0.5 cm, the program treats it as a new rise event. If no new rise event is detected for about 3 seconds, the buzzer becomes silent.

This is only an estimate based on distance readings. It is not a direct water-flow sensor.

## 6. Hysteresis

The program uses separate entry and clear values:

- WARNING starts at 10 cm or less and clears at 10.5 cm or more.
- CRITICAL starts below 5 cm and clears at 5.5 cm or more.

This small gap between the entry and clear values helps stop the status from changing too quickly when readings are close to a limit.
