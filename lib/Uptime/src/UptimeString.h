/*  Get the uptime in char array.
  Relies on the Uptime.h library.
  Output's:
  getUptime1(); = xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds        < Shows full uptime string.
  getUptime2(); = xx.Y, xx.M, xx.D, xx.h, xx.m, xx.s                                    < Only shows elapsed uptime, with short Text.
  getUptime3(); = xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds        < Only shows elapsed uptime, with long Text.
  getUptime4(); = xx.Years, xx.Months, xx.Days, xx.hours, xx.min, xx.sec, xxxx.Millis	< Shows full uptime string with millis.
	
  The string can be made with SRAM or Flash memory. Just define it below. "Line 30" (Default is Flash)
	
  //    FILE: UptimeString.h
  //  AUTHOR: XbergCode
  // VERSION: 1.0.0
  // PURPOSE: Calculates the elapsed uptime of the device using millis().
  //     URL: https://github.com/XbergCode/Uptime
  
  Version: -  Date:       -  Changes:
  1.0.0    -  15/08/2020  -  Initial.
*/


#pragma once
#include <Arduino.h>                              // For C string class

// SRAM or FLASH memory.
#define SRAM  0                                   // String are made with "sprintf"
#define FLASH 1                                   // String are made with "snprintf_P"

// Define the memory to use.
#define STRING_MEMORY_USE    FLASH                // Use this memory


#define UPTIME_VERSION       (F("1.0.0"))         // Version


class UptimeString {
  public:                                         // Public
  	
    UptimeString();                               // Set class call name
    
    // C Strings functions.
    static char * getUptime1();                   // = xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds
    static char * getUptime2();                   // = xx.Y, xx.M, xx.D, xx.h, xx.m, xx.s 
    static char * getUptime3();                   // = xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds
    static char * getUptime4();                   // = xx.Years, xx.Months, xx.Days, xx.hours, xx.min, xx.sec, xxxx.millis
    
  private:                                        // Private
    static char _c_buffer[];                      // Buffer for the uptime C string
    static const uint8_t _c_bufferSize;           // Buffer size
};

// End Of File.
