# Arduino DeepHours Timer [![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/) [![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](StudyTimer/StudyTimer.ino) [![Tinkercad](https://img.shields.io/badge/Tinkercad-FF6F00?style=for-the-badge&logo=tinkercad&logoColor=white)](StudyTimer_Tinkercad/StudyTimer_Tinkercad.ino)

DeepHours is an open-source, physical desk timer engineered to track long-form, distraction-free work sessions. Built around an Arduino Nano and a multiplexed dual 7-segment display, it provides a tactile, zero-distraction alternative to digital timer apps. \
Unlike simple countdown timers, DeepHours continuously measures accumulated seconds, minutes, and hours in the background. Using non-blocking millis() architecture and gesture-based single-button controls, it lets you seamlessly switch between viewing current minutes and total focus hours with visual LED status indicators.

## Key Features
* **Background Timekeeping:** Accurately tracks session durations up to 99 hours in real time without halting main execution loops.
* **Single-Button Gesture Control:**
  * **Short Press:** Start / Pause time tracking.
  * **2-Second Hold:** Toggle display between **Minutes View** (`MM`) and **Hours View** (`HH`).
  * **5-Second Hold:** Reset total recorded time to zero.
* **Visual Status Indicators:** Uses decimal point LEDs to denote active views (Left DP for Hours, Right DP for Minutes). When paused, both decimal points remain OFF; upon resuming, the corresponding decimal point illuminates based on the active view.
* **Multiplexed Hardware Design:** Efficient pin allocation driving two common-cathode displays via NPN transistor switching.
* **Zero Digital Distractions:** Dedicated hardware setup designed to build flow state and measure dedicated deep work blocks.
* Keep seconds and fractional seconds internally across pauses.
* Debounced button input and non-blocking display multiplexing.
* No third-party Arduino libraries required.

## Choose your sketch

| Environment | File |
| --- | --- |
| Arduino IDE: classic Nano or Uno | [DeepHours/DeepHours.ino](DeepHours/DeepHours.ino) |

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

| Gesture | Action |
| --- | --- |
| Short press | Start / pause / resume |
| Hold 2 to 4 seconds, then release | Switch hours/minutes |
| Hold at least 5 seconds | Reset to zero |

## Reading the time

The two digits do not show HH:MM simultaneously. For **4,000 seconds** of study time:

- Hours view: **1**, with the left digit's decimal point lit.
- Minutes view: **06**, with the right digit's decimal point lit.
- The remaining **40 seconds** are retained internally.

Minutes range from 00 to 59. Hours below 10 have a blank leading digit. The decimal points identify units, not decimal fractions.


## Limitations

- Elapsed time is lost on power-off, board reset or upload; no EEPROM history is implemented.
- No calendar, daily total, real-time clock or saved session log.
- Timing depends on the board oscillator and can drift.
- At 99:59:59 the timer stops; reset before another session.
- 
## Repository files

| Path | Purpose |
| --- | --- |
| `DeepHours/` | Arduino IDE sketch |
| `docs/` | Wiring images, build guide and concept sketch |

## References

- [Arduino Nano documentation](https://docs.arduino.cc/hardware/nano)
- [Arduino millis reference](https://docs.arduino.cc/language-reference/en/functions/time/millis/)
- [Arduino compile-sketches action](https://github.com/arduino/compile-sketches)
