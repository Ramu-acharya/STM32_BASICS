/**
  ******************************************************************************
  * @file           : nepali_date.c
  * @brief          : Nepali (Bikram Sambat) Date Converter Implementation
  * @author         : EVON Electric
  ******************************************************************************
  * @note           : Based on standard Nepali calendar data
  *                   Reference: 1 Baishakh 2000 BS = 13 April 1943 AD
  ******************************************************************************
  */

#include "nepali_date.h"
#include <stdio.h>
#include <string.h>

/* Nepali Month Names */
const char* NepaliMonthNames[12] = {
    "Baishakh", "Jestha", "Ashadh", "Shrawan",
    "Bhadra", "Ashwin", "Kartik", "Mangsir",
    "Poush", "Magh", "Falgun", "Chaitra"
};

const char* NepaliMonthNamesShort[12] = {
    "Bai", "Jes", "Ash", "Shr",
    "Bhd", "Asw", "Kar", "Man",
    "Pou", "Mag", "Fal", "Cha"
};

const char* NepaliWeekdayNames[7] = {
    "Aaitabar", "Sombar", "Mangalbar", "Budhabar",
    "Bihibar", "Sukrabar", "Sanibar"
};

const char* NepaliWeekdayNamesShort[7] = {
    "Aai", "Som", "Man", "Bud", "Bih", "Suk", "San"
};

/*
 * Nepali calendar days per month data
 * Index 0 = year 2000 BS, each row has 12 months
 * Data covers 2000 BS to 2090 BS (1943 AD to 2033 AD)
 */
static const uint8_t NepaliMonthData[91][12] = {
    {30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2000 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2001 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2002 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2003 BS
    {30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2004 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2005 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2006 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2007 BS
    {31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 29, 31},  // 2008 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2009 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2010 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2011 BS
    {31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30},  // 2012 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2013 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2014 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2015 BS
    {31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30},  // 2016 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2017 BS
    {31, 32, 31, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2018 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2019 BS
    {31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30},  // 2020 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2021 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30},  // 2022 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2023 BS
    {31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30},  // 2024 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2025 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2026 BS
    {30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2027 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2028 BS
    {31, 31, 32, 31, 32, 30, 30, 29, 30, 29, 30, 30},  // 2029 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2030 BS
    {30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2031 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2032 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2033 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2034 BS
    {30, 32, 31, 32, 31, 31, 29, 30, 30, 29, 29, 31},  // 2035 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2036 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2037 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2038 BS
    {31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30},  // 2039 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2040 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2041 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2042 BS
    {31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30},  // 2043 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2044 BS
    {31, 32, 31, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2045 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2046 BS
    {31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30},  // 2047 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2048 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30},  // 2049 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2050 BS
    {31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30},  // 2051 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2052 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30},  // 2053 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2054 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2055 BS
    {31, 31, 32, 31, 32, 30, 30, 29, 30, 29, 30, 30},  // 2056 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2057 BS
    {30, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2058 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2059 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2060 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2061 BS
    {30, 32, 31, 32, 31, 31, 29, 30, 29, 30, 29, 31},  // 2062 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2063 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2064 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2065 BS
    {31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 29, 31},  // 2066 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2067 BS
    {31, 31, 32, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2068 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2069 BS
    {31, 31, 31, 32, 31, 31, 29, 30, 30, 29, 30, 30},  // 2070 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2071 BS
    {31, 32, 31, 32, 31, 30, 30, 29, 30, 29, 30, 30},  // 2072 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 31},  // 2073 BS
    {31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30},  // 2074 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2075 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30},  // 2076 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2077 BS
    {31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30},  // 2078 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2079 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30},  // 2080 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2081 BS
    {31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30},  // 2082 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2083 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30},  // 2084 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2085 BS
    {31, 31, 31, 32, 31, 31, 30, 29, 30, 29, 30, 30},  // 2086 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2087 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 29, 30, 30},  // 2088 BS
    {31, 32, 31, 32, 31, 30, 30, 30, 29, 30, 29, 31},  // 2089 BS
    {31, 31, 32, 31, 31, 31, 30, 29, 30, 29, 30, 30},  // 2090 BS
};

/* Reference date: 1 Baishakh 2000 BS = 13 April 1943 AD */
#define REF_BS_YEAR     2000
#define REF_AD_YEAR     1943
#define REF_AD_MONTH    4
#define REF_AD_DAY      13

/* Days in Gregorian months */
static const uint8_t GregorianMonthDays[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/**
  * @brief  Check if Gregorian year is a leap year
  */
bool IsLeapYear(uint16_t year)
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

/**
  * @brief  Get number of days in a Gregorian month
  */
static uint8_t GetGregorianMonthDays(uint16_t year, uint8_t month)
{
    if (month == 2 && IsLeapYear(year)) {
        return 29;
    }
    return GregorianMonthDays[month - 1];
}

/**
  * @brief  Get number of days in a Nepali month
  */
uint8_t GetNepaliMonthDays(uint16_t year, uint8_t month)
{
    if (year < REF_BS_YEAR || year > (REF_BS_YEAR + 90)) {
        return 0;  // Out of range
    }
    if (month < 1 || month > 12) {
        return 0;
    }
    return NepaliMonthData[year - REF_BS_YEAR][month - 1];
}

/**
  * @brief  Calculate total days from reference date for Gregorian date
  */
static int32_t GregorianDaysFromRef(GregorianDate_t *gDate)
{
    int32_t totalDays = 0;
    uint16_t y, m;

    // Add days for complete years
    for (y = REF_AD_YEAR; y < gDate->year; y++) {
        totalDays += IsLeapYear(y) ? 366 : 365;
    }

    // Add days for complete months in current year
    for (m = 1; m < gDate->month; m++) {
        totalDays += GetGregorianMonthDays(gDate->year, m);
    }

    // Add days in current month
    totalDays += gDate->day;

    // Subtract reference date offset (13 April 1943)
    // Days from Jan 1, 1943 to April 13, 1943 = 31 + 28 + 31 + 13 = 103
    totalDays -= 103;

    return totalDays;
}

/**
  * @brief  Get day of week (0=Sunday) for a Gregorian date
  */
uint8_t GetDayOfWeek(uint16_t year, uint8_t month, uint8_t day)
{
    // Using Tomohiko Sakamoto's algorithm (simpler and more reliable)
    static const uint8_t t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    uint16_t y = year;
    if (month < 3) {
        y -= 1;
    }
    return (y + y/4 - y/100 + y/400 + t[month-1] + day) % 7;
}

/**
  * @brief  Convert Gregorian date to Nepali date
  */
bool GregorianToNepali(GregorianDate_t *gDate, NepaliDate_t *nDate)
{
    int32_t totalDays;
    uint16_t bsYear = REF_BS_YEAR;
    uint8_t bsMonth = 1;
    uint8_t bsDay = 1;
    uint8_t daysInMonth;

    // Validate input range
    if (gDate->year < REF_AD_YEAR || gDate->year > 2033) {
        return false;
    }

    // Calculate days from reference
    totalDays = GregorianDaysFromRef(gDate);

    if (totalDays < 0) {
        return false;  // Date before reference
    }

    // Convert to Nepali date
    while (totalDays > 0) {
        daysInMonth = GetNepaliMonthDays(bsYear, bsMonth);
        if (daysInMonth == 0) {
            return false;  // Out of data range
        }

        if (totalDays > daysInMonth) {
            totalDays -= daysInMonth;
            bsMonth++;
            if (bsMonth > 12) {
                bsMonth = 1;
                bsYear++;
            }
        } else {
            bsDay = (uint8_t)totalDays;
            break;
        }
    }

    nDate->year = bsYear;
    nDate->month = bsMonth;
    nDate->day = bsDay;
    nDate->weekday = GetDayOfWeek(gDate->year, gDate->month, gDate->day);

    return true;
}

/**
  * @brief  Convert Nepali date to Gregorian date
  */
bool NepaliToGregorian(NepaliDate_t *nDate, GregorianDate_t *gDate)
{
    int32_t totalDays = 0;
    uint16_t y;
    uint8_t m;

    // Validate input range
    if (nDate->year < REF_BS_YEAR || nDate->year > (REF_BS_YEAR + 90)) {
        return false;
    }

    // Calculate total days from BS reference
    for (y = REF_BS_YEAR; y < nDate->year; y++) {
        for (m = 1; m <= 12; m++) {
            totalDays += GetNepaliMonthDays(y, m);
        }
    }

    for (m = 1; m < nDate->month; m++) {
        totalDays += GetNepaliMonthDays(nDate->year, m);
    }

    totalDays += nDate->day;

    // Convert back to Gregorian starting from reference date
    uint16_t adYear = REF_AD_YEAR;
    uint8_t adMonth = REF_AD_MONTH;
    uint8_t adDay = REF_AD_DAY;

    while (totalDays > 1) {
        totalDays--;
        adDay++;

        uint8_t daysInMonth = GetGregorianMonthDays(adYear, adMonth);
        if (adDay > daysInMonth) {
            adDay = 1;
            adMonth++;
            if (adMonth > 12) {
                adMonth = 1;
                adYear++;
            }
        }
    }

    gDate->year = adYear;
    gDate->month = adMonth;
    gDate->day = adDay;

    return true;
}

/**
  * @brief  Format Nepali date as string (DD/MM/YYYY BS)
  */
void FormatNepaliDate(NepaliDate_t *nDate, char *buffer)
{
    sprintf(buffer, "%02d/%02d/%04d BS", nDate->day, nDate->month, nDate->year);
}

/**
  * @brief  Format Nepali date with month name
  */
void FormatNepaliDateFull(NepaliDate_t *nDate, char *buffer)
{
    sprintf(buffer, "%d %s %04d", nDate->day, NepaliMonthNames[nDate->month - 1], nDate->year);
}
