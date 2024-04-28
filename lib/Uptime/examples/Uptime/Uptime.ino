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
    
    //    FILE: Uptime.ino
    //  AUTHOR: XbergCode
    // VERSION: 1.0.0
    // PURPOSE: Calculates the elapsed uptime of the device using millis().
    //     URL: https://github.com/XbergCode/Uptime
 	
    Version: -  Date:       -  Changes:
    1.0.0    -  15/08/2020  -  Initial.
*/

// Uptime Library
#include <Uptime.h>                                          // Library To Calculate Uptime
Uptime uptime;                                               // Set The Call Name < Optional.
// If call name is not used then call like: Uptime::calculateUptime(); ...

// Uptime Global Variables
uint8_t Uptime_Years = 0U, Uptime_Months = 0U, Uptime_Days = 0U, Uptime_Hours = 0U, Uptime_Minutes = 0U, Uptime_Seconds = 0U;
uint16_t Uptime_TotalDays = 0U;                              // Total Uptime Days
char Uptime_Str[37];                                         // Custom Uptime C String - Global

void setup() {
  Serial.begin(115200UL);                                    // Start The Serial
  delay(100UL);                                              // Wait For Serial To Wake Up
  Serial.print(F("Uptime example. Version: "));              // Serial Print Sketch Purpose
  Serial.println(UPTIME_VERSION);                            // Serial Print Library Version
  Serial.print(F("\n"));                                     // New Line At Last
}

void loop() {
  uptime.calculateUptime();                                  // Calculate The Uptime In Library

  // Get The Uptime Values To Global Variables
  Uptime_Years      = uptime.getYears();                     // Uptime Years      "uint8_t"
  Uptime_Months     = uptime.getMonths();                    // Uptime Months     "uint8_t"
  Uptime_Days       = uptime.getDays();                      // Uptime Days       "uint8_t"
  Uptime_Hours      = uptime.getHours();                     // Uptime Hours      "uint8_t"
  Uptime_Minutes    = uptime.getMinutes();                   // Uptime Minutes    "uint8_t"
  Uptime_Seconds    = uptime.getSeconds();                   // Uptime Seconds    "uint8_t"
  Uptime_TotalDays  = uptime.getTotalDays();                 // Uptime Total Days "uint16_t"

  /*// Get The Uptime Values To Local Variables
    const uint8_t  Uptime_Years      = uptime.getYears();      // Uptime Years      "uint8_t"
    const uint8_t  Uptime_Months     = uptime.getMonths();     // Uptime Months     "uint8_t"
    const uint8_t  Uptime_Days       = uptime.getDays();       // Uptime Days       "uint8_t"
    const uint8_t  Uptime_Hours      = uptime.getHours();      // Uptime Hours      "uint8_t"
    const uint8_t  Uptime_Minutes    = uptime.getMinutes();    // Uptime Minutes    "uint8_t"
    const uint8_t  Uptime_Seconds    = uptime.getSeconds();    // Uptime Seconds    "uint8_t"
    const uint16_t Uptime_TotalDays  = uptime.getTotalDays();  // Uptime Total Days "uint16_t"
    const uint32_t Uptime_TotalSeconds = uptime.getTotalSeconds(); // Uptime Total Seconds "uint32_t"
  */

  // Serial Print Uptime Variables
  Serial.print(F("Years: "));
  Serial.println(Uptime_Years);                              // Years
  Serial.print(F("Months: "));
  Serial.println(Uptime_Months);                             // Months
  Serial.print(F("Days: "));
  Serial.println(Uptime_Days);                               // Days
  Serial.print(F("Hours: "));
  Serial.println(Uptime_Hours);                              // Hours
  Serial.print(F("Minutes: "));
  Serial.println(Uptime_Minutes);                            // Minutes
  Serial.print(F("Seconds: "));
  Serial.println(Uptime_Seconds);                            // Seconds
  Serial.print(F("Milliseconds: "));
  Serial.println(uptime.getMilliseconds());                  // Milliseconds
  Serial.print(F("Total Days: "));
  Serial.println(uptime.getTotalDays());                     // Total Days
  Serial.print(F("Total Seconds: "));
  Serial.println(uptime.getTotalSeconds());                  // Total Seconds

  // Custom C String
  //char Uptime_Str[37];                                       // Custom Uptime C String - Local
  Serial.print(F("C1: Uptime: "));
  // Make The String
  if (Uptime_Years == 0U) {                                  // Uptime Is Less Than One Year
    // First 60 Seconds
    if (Uptime_Minutes == 0U && Uptime_Hours == 0U && Uptime_Days == 0U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%i.seconds", Uptime_Seconds);
    // First Minute
    else if (Uptime_Minutes == 1U && Uptime_Hours == 0U && Uptime_Days == 0U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%i.minute, %i.seconds", Uptime_Minutes, Uptime_Seconds);
    // Second Minute And More But Less Than Hours, Days, Months
    else if (Uptime_Minutes >= 2U && Uptime_Hours == 0U && Uptime_Days == 0U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%i.minutes, %i.seconds", Uptime_Minutes, Uptime_Seconds);
    // First Hour And More But Less Than Days, Months
    else if (Uptime_Hours >= 1U && Uptime_Days == 0U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%i.hours, %i.minutes, %i.seconds", Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
    // First Day And Less Than Month
    else if (Uptime_Days == 1U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%i.Day, %i.hours, %i.min, %i.sec", Uptime_Days, Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
    // Second Day And More But Less Than Month
    else if (Uptime_Days >= 2U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%i.Days, %i.hours, %i.min, %i.sec", Uptime_Days, Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
    // First Month And More But Less Than One Year
    else if (Uptime_Months >= 1U)
      sprintf(Uptime_Str, "%i.M, %i.D, %i.hours, %i.min, %i.sec", Uptime_Months, Uptime_Days, Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
    // If There Is Any Error In This If Loop Then Make Full String.
    else sprintf(Uptime_Str, "%i.Y, %i.M, %i.D, %i.h, %i.m, %i.sec", Uptime_Years, Uptime_Months, Uptime_Days, Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
  } else                                                     // Uptime Is More Than One Year
    sprintf(Uptime_Str, "%i.Y, %i.M, %i.D, %i.h, %i.m, %i.sec", Uptime_Years, Uptime_Months, Uptime_Days, Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
  Serial.println(Uptime_Str);                                // Serial Print The Custom C String

  Serial.print(F("\n"));                                     // New Line At Last
  
  // Wait 1.Second Between Readings
  delay(1000UL);                                             // Wait 1.Second Between Readings
}

// End Of File.
