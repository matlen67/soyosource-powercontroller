/*  Get the uptime in char array.
    Relies on the Uptime.h library.
    Output's:
    getUptime1(); = xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds        < Shows full uptime string.
    getUptime2(); = xx.Y, xx.M, xx.D, xx.h, xx.m, xx.s                                    < Only shows elapsed uptime, with short Text.
    getUptime3(); = xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds        < Only shows elapsed uptime, with long Text.
    getUptime4(); = xx.Years, xx.Months, xx.Days, xx.hours, xx.min, xx.sec, xxxx.Millis   < Shows full uptime string with millis.
    
    The string can be made with SRAM or Flash memory. Just define it in "UptimeString.h" > "Line 30" (Default is Flash)
	
    //    FILE: UptimeString.cpp
    //  AUTHOR: XbergCode
    // VERSION: 1.0.0
    // PURPOSE: Calculates the elapsed uptime of the device using millis().
    //     URL: https://github.com/XbergCode/Uptime
	
    Version: -  Date:       -  Changes:
    1.0.0    -  15/08/2020  -  Initial.
*/


#include "UptimeString.h"
#include "Uptime.h"                                  // Get access to Uptime.h

UptimeString::UptimeString() {}                      // Class Call Name


// C String Class
const uint8_t UptimeString::_c_bufferSize = 68;      // Set the buffer size
char UptimeString::_c_buffer[_c_bufferSize];         // Buffer for the uptime C string


// Full string
// xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds
char * UptimeString::getUptime1() {
  Uptime::calculateUptime();                         // Calculate the uptime
#if STRING_MEMORY_USE == SRAM
  // Make the string - Use SRAM memory.
  sprintf (_c_buffer, "%i.Years, %i.Months, %i.Days, %i.hours, %i.minutes, %i.seconds", 
  Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
#else
  // Make the string - Use flash memory.
  snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Years, %i.Months, %i.Days, %i.hours, %i.minutes, %i.seconds"), 
  Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
#endif
  return _c_buffer;                                  // Retrun the C string
}

// Short text string - Shows only elapsed uptime
// xx.Y, xx.M, xx.D, xx.h, xx.m, xx.s
char * UptimeString::getUptime2() {
  Uptime::calculateUptime();                         // Calculate the uptime
  
#if STRING_MEMORY_USE == SRAM
  // Make The String - Use SRAM memory.
  if (Uptime::getYears() == 0U) {                    // Uptime Is Less Than One Year
    // First 60 Seconds
    if (Uptime::getMinutes() == 0U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.s", Uptime::getSeconds());
    // Second, Minute And More But Less Than Hours, Days, Months
    else if (Uptime::getMinutes() >= 1U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
	  sprintf(_c_buffer, "%i.m, %i.s", Uptime::getMinutes(), Uptime::getSeconds());
    // First Hour And More But Less Than Days, Months
    else if (Uptime::getHours() >= 1U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.h, %i.m, %i.s", Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // First Day And More But Less Than Month
    else if (Uptime::getDays() >= 1U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.D, %i.h, %i.m, %i.s", Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // First Month And More But Less Than One Year
    else if (Uptime::getMonths() >= 1U)
      sprintf(_c_buffer, "%i.M, %i.D, %i.h, %i.m, %i.s", Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // If There Is Any Error In This If Loop Then Make Full String.
    else 
	  sprintf(_c_buffer, "%i.Y, %i.M, %i.D, %i.h, %i.m, %i.s", Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
  } 
  else                                               // Uptime Is More Than One Year
    sprintf(_c_buffer, "%i.Y, %i.M, %i.D, %i.h, %i.m, %i.s", Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
#else
  // Make The String - Use flash memory.
  if (Uptime::getYears() == 0U) {                    // Uptime Is Less Than One Year
    // First 60 Seconds
    if (Uptime::getMinutes() == 0U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.s"), Uptime::getSeconds());
    // Second, Minute And More But Less Than Hours, Days, Months
    else if (Uptime::getMinutes() >= 1U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.m, %i.s"), Uptime::getMinutes(), Uptime::getSeconds());
    // First Hour And More But Less Than Days, Months
    else if (Uptime::getHours() >= 1U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.h, %i.m, %i.s"), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // First Day And More But Less Than Month
    else if (Uptime::getDays() >= 1U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.D, %i.h, %i.m, %i.s"), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // First Month And More But Less Than One Year
    else if (Uptime::getMonths() >= 1U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.M, %i.D, %i.h, %i.m, %i.s"), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // If There Is Any Error In This If Loop Then Make Full String.
    else 
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Y, %i.M, %i.D, %i.h, %i.m, %i.s"), Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
  } 
  else                                               // Uptime Is More Than One Year
    snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Y, %i.M, %i.D, %i.h, %i.m, %i.s"), Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
#endif
  return _c_buffer;                                  // Retrun the C string
}

// Long text string - Shows only elapsed uptime
// xx.Years, xx.Months, xx.Days, xx.hours, xx.minutes, xx.seconds
char * UptimeString::getUptime3() {
  Uptime::calculateUptime();                         // Calculate the uptime
  
#if STRING_MEMORY_USE == SRAM
  // Make the string - Use SRAM memory.
  if (Uptime::getYears() == 0U) {                    // Uptime Is Less Than One Year
    // First 60 Seconds
    if (Uptime::getMinutes() == 0U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.seconds", Uptime::getSeconds());
    // First Minute
    else if (Uptime::getMinutes() == 1U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.minute, %i.seconds", Uptime::getMinutes(), Uptime::getSeconds());
    // Second Minute And More But Less Than Hours, Days, Months
    else if (Uptime::getMinutes() >= 2U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.minutes, %i.seconds", Uptime::getMinutes(), Uptime::getSeconds());
    // First Hour And More But Less Than Days, Months
    else if (Uptime::getHours() >= 1U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.hours, %i.minutes, %i.seconds", Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // First Day And Less Than Month
    else if (Uptime::getDays() == 1U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.Day, %i.hours, %i.minutes, %i.seconds", Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // Second Day And More But Less Than Month
    else if (Uptime::getDays() >= 2U && Uptime::getMonths() == 0U)
      sprintf(_c_buffer, "%i.Days, %i.hours, %i.minutes, %i.seconds", Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // First Month And More But Less Than One Year
    else if (Uptime::getMonths() >= 1U)
      sprintf(_c_buffer, "%i.Months, %i.Days, %i.hours, %i.minutes, %i.seconds", Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // If There Is Any Error In This If Loop Then Make Full String.
    else 
      sprintf(_c_buffer, "%i.Years, %i.Months, %i.Days, %i.hours, %i.minutes, %i.seconds", Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
  }
  else                                               // Uptime Is More Than One Year
    sprintf(_c_buffer, "%i.Years, %i.Months, %i.Days, %i.hours, %i.minutes, %i.seconds", Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
#else
  // Make the string - Use flash memory.
  if (Uptime::getYears() == 0U) {                    // Uptime Is Less Than One Year
    // First 60 Seconds
    if (Uptime::getMinutes() == 0U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.seconds"), Uptime::getSeconds());
    // First Minute
    else if (Uptime::getMinutes() == 1U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.minute, %i.seconds"), Uptime::getMinutes(), Uptime::getSeconds());
    // Second Minute And More But Less Than Hours, Days, Months
    else if (Uptime::getMinutes() >= 2U && Uptime::getHours() == 0U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.minutes, %i.seconds"), Uptime::getMinutes(), Uptime::getSeconds());
    // First Hour And More But Less Than Days, Months
    else if (Uptime::getHours() >= 1U && Uptime::getDays() == 0U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.hours, %i.minutes, %i.seconds"), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // First Day And Less Than Month
    else if (Uptime::getDays() == 1U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Day, %i.hours, %i.minutes, %i.seconds"), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // Second Day And More But Less Than Month
    else if (Uptime::getDays() >= 2U && Uptime::getMonths() == 0U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Days, %i.hours, %i.minutes, %i.seconds"), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // First Month And More But Less Than One Year
    else if (Uptime::getMonths() >= 1U)
      snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Months, %i.Days, %i.hours, %i.minutes, %i.seconds"), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
    // If There Is Any Error In This If Loop Then Make Full String.
    else 
	  snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Years, %i.Months, %i.Days, %i.hours, %i.minutes, %i.seconds"), Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
  } 
  else                                               // Uptime Is More Than One Year
    snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Years, %i.Months, %i.Days, %i.hours, %i.minutes, %i.seconds"), Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds());
#endif
  return _c_buffer;                                  // Retrun the C string
}

// Full string - With millis 
// xx.Years, xx.Months, xx.Days, xx.hours, xx.min, xx.sec, xxxx.millis
char * UptimeString::getUptime4() {
  Uptime::calculateUptime();                         // Calculate the uptime
  
#if STRING_MEMORY_USE == SRAM
  // Make the string - Use SRAM memory.
  sprintf (_c_buffer, "%i.Years, %i.Months, %i.Days, %i.hours, %i.min, %i.sec, %lu.millis", 
  Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds(), Uptime::getMilliseconds());
#else
  // Make the string - Use flash memory.
  snprintf_P(_c_buffer, _c_bufferSize, PSTR("%i.Years, %i.Months, %i.Days, %i.hours, %i.min, %i.sec, %lu.millis"), 
  Uptime::getYears(), Uptime::getMonths(), Uptime::getDays(), Uptime::getHours(), Uptime::getMinutes(), Uptime::getSeconds(), Uptime::getMilliseconds());
#endif
  return _c_buffer;                                  // Retrun the C string
}

// End Of File.
