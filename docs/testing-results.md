# Testing Results

Use this page to record what the real prototype does. Do not add estimated values as test results.

## Test setup

- Container used:
- Sensor position:
- Date of test:
- Power source:
- Measuring tool:
- Notes about water movement:

## Distance test

| Test | Ruler gap (cm) | LCD gap (cm) | LCD status | Expected buzzer | Actual result |
|---|---:|---:|---|---|---|
| Safe level |  |  | SAFE | Silent | Not recorded |
| Warning level |  |  | WARNING | 1 short beep every 5 seconds while rising | Not recorded |
| Critical level |  |  | CRITICAL | Continuous while rising | Not recorded |
| Stationary level |  |  | Current level remains on screen | Silent after about 3 seconds | Not recorded |
| Sensor reading lost | N/A |  | Error message | Silent | Not recorded |

## What to check

1. Compare the LCD distance with a ruler.
2. Check the warning near 10 cm.
3. Check the critical state below 5 cm.
4. Stop pouring and check whether the buzzer becomes silent after about 3 seconds.
5. Start pouring again and check whether the warning returns.
6. Check the LCD when the sensor cannot get a valid reading.

## Measurement error

For each test, calculate:

Measurement difference = LCD reading − ruler reading

Record the result in centimetres. Test more than once before making a claim about accuracy.

## Test notes

Add the problems you saw, what you changed and the result after the change.
