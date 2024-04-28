[![Arduino CI](https://github.com/XbergCode/Uptime/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/XbergCode/Uptime/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/XbergCode/Uptime/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/XbergCode/Uptime/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/XbergCode/Uptime/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/github/license/XbergCode/Uptime)](https://github.com/XbergCode/Uptime/blob/master/LICENSE)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/XbergCode/Uptime)](https://github.com/XbergCode/Uptime/releases)
![code size:](https://img.shields.io/github/languages/code-size/XbergCode/Uptime)


# Uptime library.
Arduino library to get the device uptime.

Max runtime is "limited" to 136 to 179.years. See Note* in "Uptime.h" for more info. Lines 11 to 15.


## Description.

Uptime library for Arduino. 

Shows device uptime in: years, months, days, hours, minutes, seconds, milliseconds, total days and total seconds.

Based on 365-day calendar year.

Keep in mind that the oscillator is not 100.% accurate, and is sensitive to temperature and supply voltage fluctuation.

A crystal oscillator is about 0.005% accurate.

A ceramic oscillator is about 0.5% accurate.


## Interface.

The following functions are implemented:

#### Uptime.h:

* void calculateUptime();  <-- First call this to calculate the uptime.

* const uint8_t  getYears();

* const uint8_t  getMonths();

* const uint8_t  getDays();

* const uint8_t  getHours();

* const uint8_t  getMinutes();

* const uint8_t  getSeconds();

* const uint32_t getMilliseconds();

* const uint16_t getTotalDays();

* const uint32_t getTotalSeconds();

#### Uptime_array.h:

> char arrays are made with snprintf_P = flash memory. It can be change to SRAM in the "UptimeString.h" library @ line 30.

* char * getUptime1();  = xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds      < Shows full uptime string.

* char * getUptime2();  = xx.Y, xx.M, xx.D, xx.h, xx.m, xx.s              		                 < Shows elapsed uptime, with short text.

* char * getUptime3();  = xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds      < Shows elapsed uptime, with long text.

* char * getUptime4();  = xx.Years, xx.Months, xx.Days, xx.hours, xx.min, xx.sec, xxxx.millis < Shows full uptime string with millis.


## Outputs.

### Outputs from: Uptime.h

Years: 0

Months: 0

Days: 0

Hours: 3

Minutes: 1

Seconds: 21

Milliseconds: 0

Total Days: 0

Total Seconds: 10881

------------------------------------------------------------------------

### Outputs from: UptimeString.h

#### Output from: getUptime1();

*... @ Startup.*

0.Years, 0.Months, 0.Days, 0.hours, 0.minutes, 1.seconds

0.Years, 0.Months, 0.Days, 0.hours, 0.minutes, 2.seconds

0.Years, 0.Months, 0.Days, 0.hours, 0.minutes, 3.seconds

0.Years, 0.Months, 0.Days, 0.hours, 0.minutes, 4.seconds

*... @ First day.*

0.Years, 0.Months, 0.Days, 23.hours, 59.minutes, 58.seconds

0.Years, 0.Months, 0.Days, 23.hours, 59.minutes, 59.seconds

0.Years, 0.Months, 1.Days, 0.hours, 0.minutes, 0.seconds

0.Years, 0.Months, 1.Days, 0.hours, 0.minutes, 1.seconds

*... @ First month.*

0.Years, 0.Months, 29.Days, 23.hours, 59.minutes, 58.seconds

0.Years, 0.Months, 29.Days, 23.hours, 59.minutes, 59.seconds

0.Years, 1.Months, 0.Days, 0.hours, 0.minutes, 0.seconds

0.Years, 1.Months, 0.Days, 0.hours, 0.minutes, 1.seconds

*... @ Second month.*

0.Years, 1.Months, 30.Days, 23.hours, 59.minutes, 58.seconds

0.Years, 1.Months, 30.Days, 23.hours, 59.minutes, 59.seconds

0.Years, 2.Months, 0.Days, 0.hours, 0.minutes, 0.seconds

0.Years, 2.Months, 0.Days, 0.hours, 0.minutes, 1.seconds

*... @ Third month.*

0.Years, 2.Months, 29.Days, 23.hours, 59.minutes, 58.seconds

0.Years, 2.Months, 29.Days, 23.hours, 59.minutes, 59.seconds

0.Years, 3.Months, 0.Days, 0.hours, 0.minutes, 0.seconds

0.Years, 3.Months, 0.Days, 0.hours, 0.minutes, 1.seconds

*... @ One year.*

0.Years, 11.Months, 29.Days, 23.hours, 59.minutes, 58.seconds

0.Years, 11.Months, 29.Days, 23.hours, 59.minutes, 59.seconds

1.Years, 0.Months, 0.Days, 0.hours, 0.minutes, 0.seconds

1.Years, 0.Months, 0.Days, 0.hours, 0.minutes, 1.seconds

------------------------------------------------------------------------

#### Output from: getUptime2();

*... @ Startup.*

1.s

*... @ One year.*

1.Y, 0.M, 0.D, 0.h, 0.m, 0.s

------------------------------------------------------------------------

#### Output from: getUptime3();

*... @ Startup.*

1.seconds

*... @ One year.*

1.Years, 0.Months, 0.Days, 0.hours, 0.minutes, 0.seconds

------------------------------------------------------------------------

#### Output from: getUptime4();

*... @ Startup.*

0.Years, 0.Months, 0.Days, 0.hours, 0.min, 1.sec, 11.millis

*... @ One year.*

1.Years, 0.Months, 0.Days, 0.hours, 0.min, 0.sec, 10.millis

------------------------------------------------------------------------


## Operation.

See examples.


## Performance.

Most time is spend converting millis to seconds, minutes, hours and total days.
