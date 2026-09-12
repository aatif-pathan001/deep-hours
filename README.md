# Arduino Study Timer

A small study-session timer built with a classic Arduino Nano, two single-digit seven-segment displays, and one or two buttons. It counts elapsed time internally and switches between hours and minutes on the two-digit display.

**Status:** prototype. Host-side logic tests pass. Physical hardware operation and the revised sketch's Tinkercad compilation have not yet been confirmed. GitHub Actions is included to compile both sketches for Nano and Uno after upload.

![Study timer wiring diagram](docs/ConnectionDiagram.png)

## Features

- Start, pause, resume and reset a study session.
- Switch between whole hours and minutes within the current hour.
- Left digit's decimal point indicates hours; right digit's decimal point indicates minutes.
- Keep seconds and fractional seconds internally across pauses.
- Debounced button input and non-blocking display multiplexing.
- Support one-button controls or a dedicated second view button.
- Automatically pause at 99 hours, 59 minutes, 59 seconds.
- No third-party Arduino libraries required.

## Choose your sketch

| Environment | File |
| --- | --- |
| Arduino IDE: classic Nano or Uno | [StudyTimer/StudyTimer.ino](StudyTimer/StudyTimer.ino) |
| Tinkercad: Arduino Uno | [StudyTimer_Tinkercad/StudyTimer_Tinkercad.ino](StudyTimer_Tinkercad/StudyTimer_Tinkercad.ino) |

Both sketches use the same pin assignments and intended behaviour. The Tinkercad version uses built-in integer types and plain arrays, and omits an explicit Arduino header include to avoid the declaration errors encountered with the original sketch in Tinkercad. Maintain equivalent changes in both versions.

## Parts

| Part | Quantity |
| --- | ---: |
| Classic 5 V Arduino Nano with ATmega328P (Uno for Tinkercad) | 1 |
| Small common-cathode single-digit LED display with decimal point | 2 |
| 680 ohm resistor | 8 |
| 1 kilo-ohm resistor | 2 |
| 10 kilo-ohm resistor | 2 |
| NPN transistor, such as PN2222A or BC337 | 2 |
| Normally-open momentary push button | 1, or 2 |
| Breadboard, jumper wires and USB cable | As needed |

This design is for bare common-cathode displays, not common-anode displays or TM1637 modules. Identify the physical display and transistor legs using their exact component datasheets.

## Wiring

See the [zoomable diagram](docs/ConnectionDiagram.svg) and [detailed build guide](docs/BuildGuide.md). Disconnect USB while changing wires. Power through the board's USB connector.

| Arduino pin | Connection |
| --- | --- |
| D2 | 680 ohm resistor, then segment a on both displays |
| D3 | 680 ohm resistor, then segment b on both displays |
| D4 | 680 ohm resistor, then segment c on both displays |
| D5 | 680 ohm resistor, then segment d on both displays |
| D6 | 680 ohm resistor, then segment e on both displays |
| D7 | 680 ohm resistor, then segment f on both displays |
| D8 | 680 ohm resistor, then segment g on both displays |
| D9 | 680 ohm resistor, then decimal point on both displays |
| D10 | 1 kilo-ohm resistor to left digit transistor base |
| D11 | 1 kilo-ohm resistor to right digit transistor base |
| D12 | Control button to GND |
| A0 | Optional view button to GND |

Each transistor's collector connects to its own display's common-cathode legs. Each emitter connects to GND. Fit a 10 kilo-ohm resistor between each base and emitter. Keep the two collectors separate. All grounds join Arduino GND. Buttons use the internal pull-up resistors.

## Controls

Default: `ONE_BUTTON_MODE = true`.

| Gesture | Action |
| --- | --- |
| Short press and release, under 1 second | Start / pause / resume |
| Hold 1 to less than 5 seconds, then release | Switch hours/minutes |
| Hold at least 5 seconds | Reset to zero and pause |

Reset happens while held; releasing afterwards does not restart the timer. Time continues during a hold until paused or reset. The built-in LED is on while running.

For two buttons, set `ONE_BUTTON_MODE = false`. D12 starts/pauses on release and resets on a 5-second hold. A0 switches the view on press.

## Reading the time

The two digits do not show HH:MM simultaneously. For **4,000 seconds** of study time:

- Hours view: **1**, with the left digit's decimal point lit.
- Minutes view: **06**, with the right digit's decimal point lit.
- The remaining **40 seconds** are retained internally.

Minutes range from 00 to 59. Hours below 10 have a blank leading digit. The decimal points identify units, not decimal fractions.

## Upload to hardware

1. Open `StudyTimer/StudyTimer.ino` in Arduino IDE.
2. Install Arduino AVR Boards if necessary.
3. Choose Arduino Nano, processor ATmega328P, and your board's port. Older Nano clones may require ATmega328P (Old Bootloader). For an Uno, select Arduino Uno.
4. Choose the button mode, verify, then upload.

Keep each `.ino` inside the folder with the same name. Do not combine the two sketches into one folder.

## Simulate in Tinkercad

1. Create an Arduino Uno circuit with the components and wiring above.
2. Select Code → Text and replace all existing code with the Tinkercad sketch.
3. Set the display type to common cathode and select your button mode in the code.
4. Start the simulation. It should begin paused at 00 minutes.
5. Press/release D12's button to start. The first minute increment requires 60 seconds of simulated running time.

The revised Tinkercad sketch has passed local logic tests, but a successful Tinkercad run has not yet been reported. See [troubleshooting](docs/TROUBLESHOOTING.md).

## Verification

Run local tests with Python 3 and g++:

```sh
python3 tests/run_tests.py
```

Tests cover both sketches in both button modes: debounce, start/pause/resume, retained fractions, hour/minute conversion, clock rollover, reset release suppression and the maximum duration. Arduino APIs are stubbed; the runner models AVR's 32-bit `unsigned long` on hosts where it is 64-bit. These tests do not emulate display electronics or an AVR processor.

The [GitHub Actions workflow](.github/workflows/compile.yml) compiles both sketches for Nano and Uno, in both button modes, on pushes and pull requests. It has not been run on GitHub yet. A successful compile is not proof of correct wiring or physical operation.

## Limitations

- Elapsed time is lost on power-off, board reset or upload; no EEPROM history is implemented.
- No calendar, daily total, real-time clock or saved session log.
- Timing depends on the board oscillator and can drift.
- At 99:59:59 the timer stops; reset before another session.
- Wokwi supports Nano, but its transistor-driver simulation needs a separate adaptation. This repository does not contain a ready-to-run Wokwi project.

## Repository files

| Path | Purpose |
| --- | --- |
| `StudyTimer/` | Arduino IDE sketch |
| `StudyTimer_Tinkercad/` | Tinkercad-compatible source variant |
| `docs/` | Wiring images, build guide and troubleshooting |
| `tests/` | Reproducible host-side logic checks |
| `.github/workflows/compile.yml` | Nano/Uno compilation checks |
| `CONTRIBUTING.md` | Change and bug-report guidance |
| `PUBLISHING.md` | Upload this folder to GitHub |

## Licensing

No open-source license has been selected. Public availability on GitHub does not itself grant general permission to reuse or redistribute this project. The repository owner should choose a license and add a `LICENSE` file if they want to grant those permissions.

## References

- [Arduino Nano documentation](https://docs.arduino.cc/hardware/nano)
- [Arduino millis reference](https://docs.arduino.cc/language-reference/en/functions/time/millis/)
- [Arduino compile-sketches action](https://github.com/arduino/compile-sketches)
