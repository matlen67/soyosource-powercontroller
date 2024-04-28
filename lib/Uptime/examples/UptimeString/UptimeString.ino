/*  Uptime library for Arduino.
    Calculates the elapsed uptime of the device using millis(), even after the millis() overflow after 49 days.
    Shows: Years, Months, Days, Hours, Minutes, Seconds, Milliseconds, Total days and Total seconds.
    No millis overflow, no String class, max runtime is "limited" to 136 to 179.years. See Note for more info.*
    
    char arrays are made with snprintf_P = flash memory. It can be change to SRAM in the "UptimeString.h" library @ line 30.
    
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
    
    //    FILE: UptimeString.ino
    //  AUTHOR: XbergCode
    // VERSION: 1.0.0
    // PURPOSE: Calculates the elapsed uptime of the device using millis().
    //     URL: https://github.com/XbergCode/Uptime
 	
    Version: -  Date:       -  Changes:
    1.0.0    -  15/08/2020  -  Initial.
*/

#include <UptimeString.h>                                    // Library To Get Uptime In Char Array's
UptimeString uptimeString;                                   // Set The Call Name < Optional.

void setup() {
  Serial.begin(115200UL);                                    // Start The Serial
  delay(100UL);                                              // Wait For Serial To Wake Up
  Serial.print(F("Uptime String example. Version: "));       // Serial Print Sketch Purpose
  Serial.println(UPTIME_VERSION);                            // Serial Print Library Version
  Serial.print(F("\n"));                                     // New Line At Last
}

void loop() {
  // Uptime String - Char Array
  Serial.print(F("L1: Uptime: "));
  Serial.println(uptimeString.getUptime1());                 // Full String
  Serial.print(F("L2: Uptime: "));
  Serial.println(uptimeString.getUptime2());                 // Short String, Only elapsed uptime
  Serial.print(F("L3: Uptime: "));
  Serial.println(UptimeString::getUptime3());                // Long String, Only elapsed uptime
  Serial.print(F("L4: Uptime: "));
  Serial.println(UptimeString::getUptime4());                // Full String With Milliseconds
  
  // Wait 1.Second Between Readings
  delay(1000UL);                                             // Wait 1.Second Between Readings
}
