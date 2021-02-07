# RTC on the ATMega328P

Use the built-in RTC functionality for clock/alarm functionality. Avoid external RTC chips/modules.

## Background reading

* [ATMega328P datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf)
* [AN 1259](https://www.microchip.com/wwwAppNotes/AppNotes.aspx?appnote=en591232)

## Arduino setup

ATMega328P running with 8 MHz internal RC oscillator with [6pF 32.768Khz watch crystal](http://www.farnell.com/datasheets/1564124.pdf):

![Schematic](https://user-images.githubusercontent.com/534681/107137750-857a3b80-6907-11eb-935c-bca31a8653f8.png)

Flashed using Arduino ISP (bootloader loaded via ICSP and Arduino Nano)
