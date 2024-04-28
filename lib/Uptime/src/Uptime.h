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
    
    //    FILE: Uptime.h
    //  AUTHOR: XbergCode
    // VERSION: 1.0.0
    // PURPOSE: Calculates the elapsed uptime of the device using millis().
    //     URL: https://github.com/XbergCode/Uptime
 	
    Version: -  Date:       -  Changes:
    1.0.0    -  15/08/2020  -  Initial.
*/

#pragma once
#define UPTIME_VERSION       (F("1.0.0"))         // Version

class Uptime {
  public:                                         // Public
    
    Uptime();                                     // Set the class call name

    static void calculateUptime();                // Call the calculate uptime loop
    
    // Get the uptime.
    static const uint8_t  getYears();             // Get - Years
    static const uint8_t  getMonths();            // Get - Months
    static const uint8_t  getDays();              // Get - Days
    static const uint8_t  getHours();             // Get - Hours
    static const uint8_t  getMinutes();           // Get - Minutes
    static const uint8_t  getSeconds();           // Get - Seconds
    static const uint32_t getMilliseconds();      // Get - Milliseconds
    static const uint16_t getTotalDays();         // Get - Total Days
    static const uint32_t getTotalSeconds();      // Get - Total Seconds
      
  private:                                        // Private
  	
    // Uptime.
    static uint8_t  _years;                       // Uptime - Years
    static uint8_t  _months;                      // Uptime - Months
    static uint8_t  _days;                        // Uptime - Days
    static uint8_t  _hours;                       // Uptime - Hours
    static uint8_t  _minutes;                     // Uptime - Minutes
    static uint8_t  _seconds;                     // Uptime - Seconds
    static uint32_t _milliseconds;                // Uptime - Milliseconds
    static uint16_t _totaldays;                   // Uptime - Total Days
    
    // Keep track of the time.
    static uint32_t _last_milliseconds;           // Keep track of the time - Milliseconds
    static uint32_t _remaining_seconds;           // Keep track of the time - Seconds
    static uint32_t _remaining_minutes;           // Keep track of the time - Minutes
    static uint32_t _remaining_hours;             // Keep track of the time - Hours
    static uint16_t _remaining_totaldays;         // Keep track of the time - Total days
	
    // Convert.
    static uint32_t _c_hours;                     // Convert - Hours
    static uint32_t _c_minutes;                   // Convert - Minutes
    static uint32_t _c_seconds;                   // Convert - Seconds
    static uint32_t _c_milliseconds;              // Convert - Milliseconds
    
    // Bools to change day and month.
    static bool _b_newDay;                        // If its okay to change the day
    static bool _b_shortMonth;                    // Short or Long month
};

// End Of File.
