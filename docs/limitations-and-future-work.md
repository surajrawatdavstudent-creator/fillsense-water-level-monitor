# Limitations and Future Work

## Current limits

- The HC-SR04 is not waterproof.
- Water ripples can change the distance reading.
- The sensor needs a clear view of the water surface.
- The program detects rising water from changes in distance. It does not measure water flow directly.
- Very slow changes may not reach the movement threshold at once.
- The system gives a warning only. It does not turn off a pump or stop the water supply.
- The prototype has not been tested for long-term outdoor use.

## Future work

These are ideas for later versions:

1. Test the system with different container sizes.
2. Record more distance readings and compare them with a ruler.
3. Improve the sensor mount to reduce movement.
4. Try a waterproof ultrasonic sensor.
5. Save user settings for the warning level.
6. Study a safe pump-control design for a later version.

Any future pump control would need proper electrical isolation and safety checks. It is not part of the current project.
