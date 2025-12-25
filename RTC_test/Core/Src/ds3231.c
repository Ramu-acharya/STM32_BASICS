/**
  ******************************************************************************
  * @file           : ds3231.c
  * @brief          : DS3231 RTC Driver Implementation
  * @author         : EVON Electric
  ******************************************************************************
  */

#include "ds3231.h"
#include <stdio.h>

/* Private function prototypes */
static uint8_t BCD_To_Decimal(uint8_t bcd);
static uint8_t Decimal_To_BCD(uint8_t decimal);

/**
  * @brief  Convert BCD to Decimal
  */
static uint8_t BCD_To_Decimal(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/**
  * @brief  Convert Decimal to BCD
  */
static uint8_t Decimal_To_BCD(uint8_t decimal)
{
    return ((decimal / 10) << 4) | (decimal % 10);
}

/**
  * @brief  Initialize DS3231 RTC
  */
HAL_StatusTypeDef DS3231_Init(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef status;
    uint8_t control_reg;

    /* Check if device is connected */
    if (!DS3231_IsConnected(hi2c)) {
        return HAL_ERROR;
    }

    /* Read control register */
    status = HAL_I2C_Mem_Read(hi2c, DS3231_ADDRESS_WRITE, DS3231_REG_CONTROL,
                              I2C_MEMADD_SIZE_8BIT, &control_reg, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status;
    }

    /* Enable oscillator (clear EOSC bit), disable interrupts */
    control_reg &= ~(1 << 7);  // Clear EOSC (enable oscillator)
    control_reg &= ~(1 << 2);  // Disable interrupt output

    status = HAL_I2C_Mem_Write(hi2c, DS3231_ADDRESS_WRITE, DS3231_REG_CONTROL,
                               I2C_MEMADD_SIZE_8BIT, &control_reg, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status;
    }

    /* Clear status register flags */
    uint8_t status_reg = 0x00;
    status = HAL_I2C_Mem_Write(hi2c, DS3231_ADDRESS_WRITE, DS3231_REG_STATUS,
                               I2C_MEMADD_SIZE_8BIT, &status_reg, 1, HAL_MAX_DELAY);

    return status;
}

/**
  * @brief  Check if DS3231 is connected
  */
bool DS3231_IsConnected(I2C_HandleTypeDef *hi2c)
{
    return (HAL_I2C_IsDeviceReady(hi2c, DS3231_ADDRESS_WRITE, 3, 100) == HAL_OK);
}

/**
  * @brief  Set time on DS3231
  */
HAL_StatusTypeDef DS3231_SetTime(I2C_HandleTypeDef *hi2c, DS3231_Time_t *time)
{
    uint8_t buffer[7];

    buffer[0] = Decimal_To_BCD(time->seconds);
    buffer[1] = Decimal_To_BCD(time->minutes);
    buffer[2] = Decimal_To_BCD(time->hours);       // 24-hour format
    buffer[3] = Decimal_To_BCD(time->day);
    buffer[4] = Decimal_To_BCD(time->date);
    buffer[5] = Decimal_To_BCD(time->month);
    buffer[6] = Decimal_To_BCD(time->year);

    return HAL_I2C_Mem_Write(hi2c, DS3231_ADDRESS_WRITE, DS3231_REG_SECONDS,
                             I2C_MEMADD_SIZE_8BIT, buffer, 7, HAL_MAX_DELAY);
}

/**
  * @brief  Get time from DS3231
  */
HAL_StatusTypeDef DS3231_GetTime(I2C_HandleTypeDef *hi2c, DS3231_Time_t *time)
{
    HAL_StatusTypeDef status;
    uint8_t buffer[7];

    status = HAL_I2C_Mem_Read(hi2c, DS3231_ADDRESS_WRITE, DS3231_REG_SECONDS,
                              I2C_MEMADD_SIZE_8BIT, buffer, 7, HAL_MAX_DELAY);

    if (status == HAL_OK) {
        time->seconds = BCD_To_Decimal(buffer[0] & 0x7F);
        time->minutes = BCD_To_Decimal(buffer[1] & 0x7F);
        time->hours   = BCD_To_Decimal(buffer[2] & 0x3F);  // 24-hour format mask
        time->day     = BCD_To_Decimal(buffer[3] & 0x07);
        time->date    = BCD_To_Decimal(buffer[4] & 0x3F);
        time->month   = BCD_To_Decimal(buffer[5] & 0x1F);
        time->year    = BCD_To_Decimal(buffer[6]);
    }

    return status;
}

/**
  * @brief  Get temperature from DS3231
  */
HAL_StatusTypeDef DS3231_GetTemperature(I2C_HandleTypeDef *hi2c, float *temperature)
{
    HAL_StatusTypeDef status;
    uint8_t buffer[2];

    status = HAL_I2C_Mem_Read(hi2c, DS3231_ADDRESS_WRITE, DS3231_REG_TEMP_MSB,
                              I2C_MEMADD_SIZE_8BIT, buffer, 2, HAL_MAX_DELAY);

    if (status == HAL_OK) {
        int8_t temp_msb = (int8_t)buffer[0];
        uint8_t temp_lsb = buffer[1] >> 6;  // Only upper 2 bits are used

        *temperature = (float)temp_msb + (temp_lsb * 0.25f);
    }

    return status;
}

/**
  * @brief  Format time as string (HH:MM:SS)
  */
void DS3231_FormatTime(DS3231_Time_t *time, char *buffer)
{
    sprintf(buffer, "%02d:%02d:%02d", time->hours, time->minutes, time->seconds);
}

/**
  * @brief  Format date as string (DD/MM/YYYY)
  */
void DS3231_FormatDate(DS3231_Time_t *time, char *buffer)
{
    sprintf(buffer, "%02d/%02d/20%02d", time->date, time->month, time->year);
}

/**
  * @brief  Format full datetime as string
  */
void DS3231_FormatDateTime(DS3231_Time_t *time, char *buffer)
{
    sprintf(buffer, "%02d/%02d/20%02d %02d:%02d:%02d",
            time->date, time->month, time->year,
            time->hours, time->minutes, time->seconds);
}
