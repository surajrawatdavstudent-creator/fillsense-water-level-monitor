# Components

This is the parts list for the current FillSense prototype.

| Part | Quantity | Use |
|---|---:|---|
| Arduino Uno | 1 | Reads the sensor and controls the LCD and buzzer |
| HC-SR04 ultrasonic sensor | 1 | Measures the gap to the water surface |
| 16×2 parallel LCD | 1 | Shows distance and status |
| Piezo buzzer | 1 | Gives audible warnings |
| Breadboard | 1 | Holds the circuit during testing |
| Jumper wires | As needed | Connect the parts |
| USB cable | 1 | Powers the Arduino during setup and tests |
| Water container | 1 | Used for practical testing |

## Notes

- The code uses the standard Arduino LiquidCrystal library.
- The buzzer code uses tone(), so a passive piezo buzzer is expected. Check the type of your buzzer before connecting it.
- The HC-SR04 is not waterproof. Keep it above the water and protect it from splashes.
- No motor, relay or pump is used in this version.
