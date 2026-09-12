# Arduino Nano study timer

## Hardware assumed
Classic 5 V Arduino Nano with ATmega328P; two small, bare single-digit **common-cathode** LED displays with decimal points. This is not for TM1637 modules, common-anode displays, or large high-voltage digits. Match connections by segment names from your display datasheet; physical leg numbers cannot be established from these photos.

## Behaviour
- Starts paused at 00 minutes. Press and release starts/pauses/resumes.
- Default one-button mode: hold 1 to less than 5 seconds, then release to switch hours/minutes. At 5 seconds the timer resets immediately and remains paused.
- Left digit's decimal point = hours; right digit's decimal point = minutes. Points are unit indicators, not a decimal fraction. Their physical location depends on the display.
- Hours display has no leading zero below 10. Minutes display is always 00–59, the remainder within the current hour, not cumulative minutes.
- For 4,000 elapsed seconds: hours view is 1 and minutes view is 06; the remaining 40 seconds are retained internally. (The handwritten 10-second remainder is an arithmetic slip.)
- Either of the decimal point is on when running, both off when paused. Time continues during a button hold until paused or reset.
- Maximum is 99:59:59, then automatic pause. Reset to start another session.
- Power-off, board reset and uploading clear the time. No daily history or EEPROM persistence is implemented. This is a millis-based timer.

## Parts
- 1 classic Nano and USB cable/power source
- 2 common-cathode single-digit displays with DP
- 8 × 680 ohm resistors (one per shared segment line, including DP)
- 2 × 1 kilo-ohm base resistors
- 2 × 10 kilo-ohm base-to-emitter pulldown resistors
- 2 NPN transistors, e.g. PN2222A or BC337; check the exact manufacturer's E/B/C pinout
- 1 normally-open momentary button; optional second button
- Breadboard and jumpers

## Connections
Unplug USB while wiring. Power the completed build through the Nano USB connector. All ground connections below join Nano GND.

| Nano | Series part | Destination |
|---|---|---|
| D2 | 680 ohm | segment a on BOTH displays |
| D3 | 680 ohm | segment b on BOTH displays |
| D4 | 680 ohm | segment c on BOTH displays |
| D5 | 680 ohm | segment d on BOTH displays |
| D6 | 680 ohm | segment e on BOTH displays |
| D7 | 680 ohm | segment f on BOTH displays |
| D8 | 680 ohm | segment g on BOTH displays |
| D9 | 680 ohm | dp on BOTH displays |
| D10 | 1 kilo-ohm | Q1 base, left digit driver |
| D11 | 1 kilo-ohm | Q2 base, right digit driver |
| D12 | control switch | GND when pressed |
| A0 | optional view switch | GND when pressed |

For Q1: collector to every common-cathode leg of the LEFT display, emitter to GND, 10 kilo-ohm resistor from base to emitter. For Q2: same connections for the RIGHT display. Do not connect the two collectors together. Do not connect display common cathodes directly to a Nano GPIO. Leave A0 unconnected in one-button mode.

Both displays share a,b,c,d,e,f,g,dp through the eight resistors; the code enables only one digit at a time. With a typical red LED drop around 2 V, 680 ohms gives approximately 4 mA peak per lit segment and about half that average at 50% duty. Transistors carry the combined digit current. Brightness depends on your displays.

On a four-leg tactile switch, use contacts that are disconnected when released and connected when pressed; the two legs on a given internal contact are already joined. INPUT_PULLUP supplies the button bias: no external button pull-up is needed.

## Upload
1. Extract the ZIP. Keep DeepHours.ino inside its DeepHours folder.
2. Open DeepHours/DeepHours.ino in Arduino IDE.
3. Install/select Arduino AVR Boards and choose Arduino Nano, ATmega328P, and the connected port. Some older clones require ATmega328P (Old Bootloader).
4. Verify, then upload. No third-party libraries are required.

## Check the assembled build
- At power-on: 00 with right digit DP.
- Press: LED on. After one minute the display reads 01.
- Pause, wait, resume: only running time should accumulate.
- Switch view without stopping: hours show 0 with left DP.
- Hold 5 seconds: 00 minutes, paused.
- Wrong segments usually mean a segment/leg mapping mismatch. Blank digits can indicate wrong display polarity or transistor E/B/C wiring. Recheck with USB disconnected.

## Validation and references

Official references:
- Nano hardware: https://docs.arduino.cc/hardware/nano
- millis timing and rollover: https://docs.arduino.cc/language-reference/en/functions/time/millis/
- Internal pull-up example: https://docs.arduino.cc/tutorials/generic/digital-input-pullup/
