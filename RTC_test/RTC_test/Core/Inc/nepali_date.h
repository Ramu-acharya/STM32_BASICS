/**
  ******************************************************************************
  * @file           : nepali_date.h
  * @brief          : Nepali (Bikram Sambat) Date Converter Header
  * @author         : EVON Electric
  ******************************************************************************
  * @note           : Converts Gregorian (AD) dates to Nepali (BS) dates
  *                   Valid range: 1944 AD - 2033 AD (2000 BS - 2090 BS)
  ******************************************************************************
  */

#ifndef NEPALI_DATE_H
#define NEPALI_DATE_H

#include <stdint.h>
#include <stdbool.h>

/* Nepali Date Structure */
typedef struct {
    uint16_t year;      // BS Year (e.g., 2081)
    uint8_t month;      // 1-12 (Baishakh to Chaitra)
    uint8_t day;        // 1-32
    uint8_t weekday;    // 0=Sunday, 1=Monday, ... 6=Saturday
} NepaliDate_t;

/* Gregorian Date Structure */
typedef struct {
    uint16_t year;      // AD Year (e.g., 2025)
    uint8_t month;      // 1-12
    uint8_t day;        // 1-31
} GregorianDate_t;

/* Nepali Month Names */
extern const char* NepaliMonthNames[12];
extern const char* NepaliMonthNamesShort[12];
extern const char* NepaliWeekdayNames[7];
extern const char* NepaliWeekdayNamesShort[7];

/**
  * @brief  Convert Gregorian date to Nepali date
  * @param  gDate: Pointer to Gregorian date structure
  * @param  nDate: Pointer to Nepali date structure to store result
  * @retval true if conversion successful, false otherwise
  */
bool GregorianToNepali(GregorianDate_t *gDate, NepaliDate_t *nDate);

/**
  * @brief  Convert Nepali date to Gregorian date
  * @param  nDate: Pointer to Nepali date structure
  * @param  gDate: Pointer to Gregorian date structure to store result
  * @retval true if conversion successful, false otherwise
  */
bool NepaliToGregorian(NepaliDate_t *nDate, GregorianDate_t *gDate);

/**
  * @brief  Check if Gregorian year is a leap year
  * @param  year: Gregorian year
  * @retval true if leap year, false otherwise
  */
bool IsLeapYear(uint16_t year);

/**
  * @brief  Get number of days in a Nepali month
  * @param  year: Nepali year (BS)
  * @param  month: Month (1-12)
  * @retval Number of days in the month (29-32), 0 if invalid
  */
uint8_t GetNepaliMonthDays(uint16_t year, uint8_t month);

/**
  * @brief  Format Nepali date as string (DD/MM/YYYY BS)
  * @param  nDate: Pointer to Nepali date structure
  * @param  buffer: Output buffer (minimum 20 bytes)
  */
void FormatNepaliDate(NepaliDate_t *nDate, char *buffer);

/**
  * @brief  Format Nepali date with month name
  * @param  nDate: Pointer to Nepali date structure
  * @param  buffer: Output buffer (minimum 30 bytes)
  */
void FormatNepaliDateFull(NepaliDate_t *nDate, char *buffer);

/**
  * @brief  Get day of week (0=Sunday) for a Gregorian date
  * @param  year: Gregorian year
  * @param  month: Month (1-12)
  * @param  day: Day (1-31)
  * @retval Day of week (0-6)
  */
uint8_t GetDayOfWeek(uint16_t year, uint8_t month, uint8_t day);

#endif /* NEPALI_DATE_H */
