#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Sensor and buzzer pins
const byte TRIG_PIN = 9;
const byte ECHO_PIN = 10;
const byte BUZZER_PIN = 8;

// Distance levels, measured from the sensor face to the water
const float WARNING_DISTANCE_CM = 10.0;
const float CRITICAL_DISTANCE_CM = 5.0;

// Hysteresis helps stop fast switching near a level boundary
const float WARNING_CLEAR_CM = 10.5;
const float CRITICAL_CLEAR_CM = 5.5;

// Timing
const unsigned long WARNING_BEEP_INTERVAL_MS = 5000UL;
const unsigned long STATIONARY_TIME_MS = 3000UL;
const unsigned long SENSOR_TIMEOUT_US = 25000UL;

// Movement detection
const float RISE_STEP_CM = 0.5;
const float FALL_RESET_CM = 1.0;

// Alert states: 0 = SAFE, 1 = WARNING, 2 = CRITICAL
byte alertZone = 0;
byte invalidReadCount = 0;

float movementReferenceCm = -1.0;
unsigned long lastRiseTimeMs = 0;
unsigned long lastWarningBeepMs = 0;

bool haveRiseEvent = false;
bool continuousBuzzerOn = false;


// Read one distance value from the HC-SR04
float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoTimeUs =
      pulseIn(ECHO_PIN, HIGH, SENSOR_TIMEOUT_US);

  if (echoTimeUs == 0) {
    return -1.0;
  }

  const float distanceCm =
      (echoTimeUs * 0.0343) / 2.0;

  if (distanceCm < 2.0 || distanceCm > 400.0) {
    return -1.0;
  }

  return distanceCm;
}


// Take three readings and return the median of valid values
float readFilteredDistanceCm() {
  float readings[3];
  byte validCount = 0;

  for (byte i = 0; i < 3; i++) {
    const float reading = readDistanceCm();

    if (reading >= 2.0) {
      readings[validCount] = reading;
      validCount++;
    }

    delay(35);
  }

  if (validCount < 2) {
    return -1.0;
  }

  // Sort the valid readings
  for (byte i = 0; i < validCount - 1; i++) {
    for (byte j = i + 1; j < validCount; j++) {
      if (readings[j] < readings[i]) {
        const float temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }

  if (validCount == 3) {
    return readings[1];
  }

  return (readings[0] + readings[1]) / 2.0;
}


// Decide which level the system should show
byte getAlertZone(float distanceCm) {
  if (alertZone == 0) {
    if (distanceCm < CRITICAL_DISTANCE_CM) {
      return 2;
    }

    if (distanceCm <= WARNING_DISTANCE_CM) {
      return 1;
    }

    return 0;
  }

  if (alertZone == 1) {
    if (distanceCm < CRITICAL_DISTANCE_CM) {
      return 2;
    }

    if (distanceCm >= WARNING_CLEAR_CM) {
      return 0;
    }

    return 1;
  }

  // Current zone is CRITICAL
  if (distanceCm >= WARNING_CLEAR_CM) {
    return 0;
  }

  if (distanceCm >= CRITICAL_CLEAR_CM) {
    return 1;
  }

  return 2;
}


// A falling distance means the water is getting closer to the sensor
bool detectWaterRising(float distanceCm, unsigned long nowMs) {
  if (movementReferenceCm < 0.0) {
    movementReferenceCm = distanceCm;
    return false;
  }

  if (distanceCm <= movementReferenceCm - RISE_STEP_CM) {
    movementReferenceCm = distanceCm;
    lastRiseTimeMs = nowMs;
    haveRiseEvent = true;
  } else if (distanceCm >= movementReferenceCm + FALL_RESET_CM) {
    movementReferenceCm = distanceCm;
    haveRiseEvent = false;
  }

  return haveRiseEvent &&
         (nowMs - lastRiseTimeMs < STATIONARY_TIME_MS);
}


// Stop a continuous sound if it is currently on
void stopContinuousBuzzer() {
  if (continuousBuzzerOn) {
    noTone(BUZZER_PIN);
    continuousBuzzerOn = false;
  }
}


// Update the buzzer for the current level
void updateBuzzer(byte zone, bool waterRising, unsigned long nowMs) {
  if (zone == 0) {
    stopContinuousBuzzer();
    return;
  }

  if (zone == 1) {
    stopContinuousBuzzer();

    if (waterRising &&
        nowMs - lastWarningBeepMs >= WARNING_BEEP_INTERVAL_MS) {
      tone(BUZZER_PIN, 2000, 130);
      lastWarningBeepMs = nowMs;
    }

    return;
  }

  // CRITICAL: sound only while recent upward movement is detected
  if (waterRising) {
    if (!continuousBuzzerOn) {
      tone(BUZZER_PIN, 2000);
      continuousBuzzerOn = true;
    }
  } else {
    stopContinuousBuzzer();
  }
}


// Print a line and clear any characters left from the last message
void printLcdLine(byte row, const char *message) {
  lcd.setCursor(0, row);

  byte position = 0;
  while (message[position] != '\0' && position < 16) {
    lcd.print(message[position]);
    position++;
  }

  while (position < 16) {
    lcd.print(' ');
    position++;
  }
}


// Show the distance and one simple status line
void updateLcd(float distanceCm, byte zone) {
  char distanceLine[17];
  snprintf(distanceLine, sizeof(distanceLine),
           "Gap: %5.1f cm", distanceCm);

  printLcdLine(0, distanceLine);

  if (zone == 0) {
    printLcdLine(1, "Status: SAFE");
  } else if (zone == 1) {
    printLcdLine(1, "Status: WARNING");
  } else {
    printLcdLine(1, "Status: CRITICAL");
  }
}


// Show a clear message when the sensor has no valid reading
void showSensorStatus(bool sensorError) {
  printLcdLine(0, "Gap: NO READING");

  if (sensorError) {
    printLcdLine(1, "Status: ERROR");
  } else {
    printLcdLine(1, "Status: CHECKING");
  }
}


void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);

  lcd.begin(16, 2);
  printLcdLine(0, "FILL SENSE");
  printLcdLine(1, "Starting...");
  delay(1200);
}


void loop() {
  const unsigned long nowMs = millis();
  const float distanceCm = readFilteredDistanceCm();

  // Do not use a failed reading as a real distance
  if (distanceCm < 0.0) {
    if (invalidReadCount < 255) {
      invalidReadCount++;
    }

    stopContinuousBuzzer();
    showSensorStatus(invalidReadCount >= 3);
    delay(50);
    return;
  }

  invalidReadCount = 0;

  const bool waterRising =
      detectWaterRising(distanceCm, nowMs);

  const byte newZone = getAlertZone(distanceCm);

  if (newZone != alertZone) {
    alertZone = newZone;
    lastWarningBeepMs = nowMs;

    if (alertZone != 2) {
      stopContinuousBuzzer();
    }
  }

  updateLcd(distanceCm, alertZone);
  updateBuzzer(alertZone, waterRising, nowMs);

  delay(50);
}
