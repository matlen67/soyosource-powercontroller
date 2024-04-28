/*  Uptime library for Arduino.
    Calculates the elapsed uptime of the device using millis(), even after the millis() overflow after 49 days.
    Shows: Years, Months, Days, Hours, Minutes, Seconds, Milliseconds, Total days and Total seconds.
    No millis overflow, no String class, max runtime is "limited" to 136 to 179.years. See Note for more info.*
    
    Based on 365-day calendar, no leap day or second are added.
      1 2 3 4 5 6 7 8 9 10 11 12 = Months
      S L S L S L S L S L  S  S  = 5*31 + 7*30 = 365 /  L = Long month and S = Short month.
  	
    //---------------------------------------------------------------------------------------------------------------------                      
    * Note:
      Total seconds is uint32_t and has a max runtime of 136 years.
      Total days is uint16_t = 65.535 days. So max runtime for it is about 179 years.
      If you need 11.759.230,7.years uptime then change "_years" & "_totaldays" to uint32_t,
      and change getTotalSeconds() to uint64_t if you use that too. < 64 bits can be problematic.
      
      In a perfect world this code should be able to run for 136 to 179 years without problem.
    //---------------------------------------------------------------------------------------------------------------------
    
    Keep in mind that the oscillator is not 100.% accurate, and is sensitive to temperature and supply voltage fluctuation.
    A crystal oscillator is about 0.005% accurate. = 86400*0,005=432/100 = ±4.32.sec per day
    A ceramic oscillator is about 0.5% accurate.   = 86400*0,5=43200/100=432/60 = ±7.min & 2.sec per day.
    
    //    FILE: Uptime.cpp
    //  AUTHOR: XbergCode
    // VERSION: 1.0.0
    // PURPOSE: Calculates the elapsed uptime of the device using millis().
    //     URL: https://github.com/XbergCode/Uptime
 	
    Version: -  Date:       -  Changes:
    1.0.0    -  15/08/2020  -  Initial.
*/

// Headers
#include <Arduino.h>
#include "Uptime.h"

// Private variables to hold the elapsed uptime.
uint8_t  Uptime::_years                 = 0;     // Uptime - Years
uint8_t  Uptime::_months                = 0;     // Uptime - Months
uint8_t  Uptime::_days                  = 0;     // Uptime - Days
uint8_t  Uptime::_hours                 = 0;     // Uptime - Hours
uint8_t  Uptime::_minutes               = 0;     // Uptime - Minutes
uint8_t  Uptime::_seconds               = 0;     // Uptime - Seconds
uint32_t Uptime::_milliseconds          = 0UL;   // Uptime - Milliseconds
uint16_t Uptime::_totaldays             = 0U;    // Uptime - Total days

// Private variabes to keep track of the time, in case of millis() overflow.
uint32_t Uptime::_last_milliseconds     = 0UL;   // Keep track of the time - Milliseconds
uint32_t Uptime::_remaining_seconds     = 0UL;   // Keep track of the time - Seconds
uint32_t Uptime::_remaining_minutes     = 0UL;   // Keep track of the time - Minutes
uint32_t Uptime::_remaining_hours       = 0UL;   // Keep track of the time - Hours 
uint16_t Uptime::_remaining_totaldays   = 0U;    // Keep track of the time - Total days

// Private variabes to convert milliseconds to seconds, minutes and hours.
uint32_t Uptime::_c_milliseconds        = 0UL;   // Convert - Milliseconds
uint32_t Uptime::_c_seconds             = 0UL;   // Convert - Seconds
uint32_t Uptime::_c_minutes             = 0UL;   // Convert - Minutes
uint32_t Uptime::_c_hours               = 0UL;   // Convert - Hours

// Private Bool so we only change once each time.
bool Uptime::_b_newDay                  = false; // If its okay to change the day
bool Uptime::_b_shortMonth              = true;  // Short or Long months - Start with short month

//--------------------------------------------------------------------------------//

Uptime::Uptime() {}                                                         // Set the class call name	

// Get the actual elapsed time since the device booted.
const uint8_t  Uptime::getYears()        { return _years;        }          // Get Years
const uint8_t  Uptime::getMonths()       { return _months;       }          // Get Months
const uint8_t  Uptime::getDays()         { return _days;         }          // Get Days
const uint8_t  Uptime::getHours()        { return _hours;        }          // Get Hours
const uint8_t  Uptime::getMinutes()      { return _minutes;      }          // Get Minutes
const uint8_t  Uptime::getSeconds()      { return _seconds;      }          // Get Seconds
const uint32_t Uptime::getMilliseconds() { return _milliseconds; }          // Get Milliseconds
const uint16_t Uptime::getTotalDays()    { return _totaldays;    }          // Get Total Days
const uint32_t Uptime::getTotalSeconds() { return (_totaldays * 86400UL) + (_hours * 3600UL) + (_minutes * 60UL) + _seconds; }	// Get Total Seconds

//--------------------------------------------------------------------------------//

// Calculate milliseconds, seconds, minutes, hours, days, months, years and total days.
void Uptime::calculateUptime() {                                            // Calculate The Uptime
  _c_milliseconds = millis();                                               // Get The Time
  
  // If the millis overflows then store the last elapsed seconds, minutes, hours and total days.
  if (_last_milliseconds > _c_milliseconds) {                               // Millis overflow.
    _remaining_seconds    = _seconds;                                       // Seconds
    _remaining_minutes    = _minutes;                                       // Minutes
    _remaining_hours      = _hours;                                         // Hours
    _remaining_totaldays  = _totaldays;                                     // Total Days
  }
  // Store the last millis, for millis overflow tracking.
  _last_milliseconds = _c_milliseconds;                                     // Keep track of the time - Milliseconds

  // Convert elapsed millis to seconds, minutes and hours.
  _c_seconds     = (_c_milliseconds / 1000) + _remaining_seconds;           // Seconds
  _c_minutes     = (_c_seconds      / 60)   + _remaining_minutes;           // Minutes
  _c_hours       = (_c_minutes      / 60)   + _remaining_hours;             // Hours
  
  // Calculate total days.
  _totaldays     = (_c_hours        / 24)   + _remaining_totaldays;         // Total Days

  // Calculate the actual time elapsed, using modulus, for milliseconds, seconds, minutes and hours.
  _milliseconds  = _c_milliseconds  % 1000;                                 // Milliseconds
  _seconds       = _c_seconds       % 60;                                   // Seconds
  _minutes       = _c_minutes       % 60;                                   // Minutes
  _hours         = _c_hours         % 24;                                   // Hours
    
  // Count days, months and years.
  
  // Get ready for a new day. (From 13 to 23 hours) Gives Arduino lot of opportunities to change the day.
  if (_hours >= 13 && _hours <= 23 && _b_newDay == false) _b_newDay = true; // Set the state - Start running
  
  // One day is up. Add to the day count. (From 0 to 12 hours) Gives Arduino lot of opportunities to change the day.
  if (_hours >= 0 && _hours <= 12 && _b_newDay == true) {                   // Days
    _days = _days + 1;                                                      // Add to days
    _b_newDay = false;                                                      // Set the state - Stop running
  }
  
  // One year is up. Reset months & days, add to years count.
  if (_months >= 11 && _days >= 30) {                                       // Years
    _b_shortMonth = true;                                                   // Next month is short
    _days = 0;                                                              // Reset days
    _months = 0;                                                            // Reset months
    _years = _years + 1;                                                    // Add to years count
  }
  
  // One short month is up. Reset days and add to months count.
  if (_days >= 30 && _b_shortMonth == true) {                               // Months - Short
    _b_shortMonth = false;                                                  // Next month is long
    _days = 0;                                                              // Reset days
    _months = _months + 1;                                                  // Add to months count
  }

  // One long month is up. Reset days and add to months count.
  if (_days >= 31 && _b_shortMonth == false) {                              // Months - Long
    _b_shortMonth = true;                                                   // Next month is short
    _days = 0;                                                              // Reset days
    _months = _months + 1;                                                  // Add to months count
  }
}

// End Of File.
