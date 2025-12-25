/**
  ******************************************************************************
  * @file           : ds3231.h
  * @brief          : DS3231 RTC Driver Header
  * @author         : EVON Electric
  ******************************************************************************
  */

#ifndef DS3231_H
#define DS3231_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/* DS3231 I2C Address (7-bit) */
#define DS3231_ADDRESS          0x68
#define DS3231_ADDRESS_WRITE    (DS3231_ADDRESS << 1)
#define DS3231_ADDRESS_READ     ((DS3231_ADDRESS << 1) | 0x01)

/* DS3231 Register Addresses */
#define DS3231_REG_SECONDS      0x00
#define DS3231_REG_MINUTES      0x01
#define DS3231_REG_HOURS        0x02
#define DS3231_REG_DAY          0x03
#define DS3231_REG_DATE         0x04
#define DS3231_REG_MONTH        0x05
#define DS3231_REG_YEAR         0x06
#define DS3231_REG_ALARM1_SEC   0x07
#define DS3231_REG_ALARM1_MIN   0x08
#define DS3231_REG_ALARM1_HOUR  0x09
#define DS3231_REG_ALARM1_DAY   0x0A
#define DS3231_REG_ALARM2_MIN   0x0B
#define DS3231_REG_ALARM2_HOUR  0x0C
#define DS3231_REG_ALARM2_DAY   0x0D
#define DS3231_REG_CONTROL      0x0E
#define DS3231_REG_STATUS       0x0F
#define DS3231_REG_AGING        0x10
#define DS3231_REG_TEMP_MSB     0x11
#define DS3231_REG_TEMP_LSB     0x12

/* Time structure */
typedef struct {
    uint8_t seconds;    // 0-59
    uint8_t minutes;    // 0-59
    uint8_t hours;      // 0-23 (24-hour format)
    uint8_t day;        // 1-7 (day of week, 1=Sunday)
    uint8_t date;       // 1-31
    uint8_t month;      // 1-12
    uint8_t year;       // 0-99 (represents 2000-2099)
} DS3231_Time_t;

/* Function Prototypes */

/**
  * @brief  Initialize DS3231 RTC
  * @param  hi2c: Pointer to I2C handle
  * @retval HAL_StatusTypeDef
  */
HAL_StatusTypeDef DS3231_Init(I2C_HandleTypeDef *hi2c);

/**
  * @brief  Check if DS3231 is connected
  * @param  hi2c: Pointer to I2C handle
  * @retval true if connected, false otherwise
  */
bool DS3231_IsConnected(I2C_HandleTypeDef *hi2c);

/**
  * @brief  Set time on DS3231
  * @param  hi2c: Pointer to I2C handle
  * @param  time: Pointer to time structure
  * @retval HAL_StatusTypeDef
  */
HAL_StatusTypeDef DS3231_SetTime(I2C_HandleTypeDef *hi2c, DS3231_Time_t *time);

/**
  * @brief  Get time from DS3231
  * @param  hi2c: Pointer to I2C handle
  * @param  time: Pointer to time structure to store result
  * @retval HAL_StatusTypeDef
  */
HAL_StatusTypeDef DS3231_GetTime(I2C_HandleTypeDef *hi2c, DS3231_Time_t *time);

/**
  * @brief  Get temperature from DS3231
  * @param  hi2c: Pointer to I2C handle
  * @param  temperature: Pointer to store temperature (in 0.25°C resolution)
  * @retval HAL_StatusTypeDef
  */
HAL_StatusTypeDef DS3231_GetTemperature(I2C_HandleTypeDef *hi2c, float *temperature);

/**
  * @brief  Format time as string (HH:MM:SS)
  * @param  time: Pointer to time structure
  * @param  buffer: Output buffer (minimum 9 bytes)
  */
void DS3231_FormatTime(DS3231_Time_t *time, char *buffer);

/**
  * @brief  Format date as string (DD/MM/YYYY)
  * @param  time: Pointer to time structure
  * @param  buffer: Output buffer (minimum 11 bytes)
  */
void DS3231_FormatDate(DS3231_Time_t *time, char *buffer);

/**
  * @brief  Format full datetime as string
  * @param  time: Pointer to time structure
  * @param  buffer: Output buffer (minimum 25 bytes)
  */
void DS3231_FormatDateTime(DS3231_Time_t *time, char *buffer);

#endif /* DS3231_H */
