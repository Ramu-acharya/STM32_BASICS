/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - DS3231 RTC with USB CDC Output
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.

  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "ds3231.h"
#include "nepali_date.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USB_TX_BUFFER_SIZE  256
#define TIME_PRINT_INTERVAL 1000  // Print time every 1 second

// ============================================================================
// SET THIS TO 1 TO PROGRAM THE RTC TIME (ONLY ONCE), THEN SET BACK TO 0
// ============================================================================
#define SET_RTC_TIME        0     // 1 = Set time, 0 = Just read time
// ============================================================================

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* NOTE: hpcd_USB_OTG_FS is defined in USB_DEVICE/Target/usbd_conf.c */

/* USER CODE BEGIN PV */
DS3231_Time_t currentTime;
NepaliDate_t nepaliDate;
GregorianDate_t gregorianDate;
char usbTxBuffer[USB_TX_BUFFER_SIZE];
uint32_t lastPrintTime = 0;
uint8_t rtcInitialized = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* NOTE: USB is initialized via MX_USB_DEVICE_Init() from usb_device.c */
/* USER CODE BEGIN PFP */
void USB_Print(const char *str);
void PrintCurrentTime(void);
void SetInitialTime(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Send string via USB CDC
  * @param  str: String to send
  */
void USB_Print(const char *str)
{
    uint16_t len = strlen(str);
    uint8_t retries = 10;

    while (retries > 0) {
        if (CDC_Transmit_FS((uint8_t*)str, len) == USBD_OK) {
            break;
        }
        HAL_Delay(1);
        retries--;
    }
}

/**
  * @brief  Print formatted time via USB CDC (with Nepali date)
  */
void PrintCurrentTime(void)
{
    float temperature;

    if (DS3231_GetTime(&hi2c1, &currentTime) == HAL_OK) {
        // Convert to Nepali date
        gregorianDate.year = 2000 + currentTime.year;
        gregorianDate.month = currentTime.month;
        gregorianDate.day = currentTime.date;

        // Get weekday
        uint8_t weekday = GetDayOfWeek(gregorianDate.year, gregorianDate.month, gregorianDate.day);

        // English weekday abbreviations
        const char* engWeekdays[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

        int len = 0;

        // Line 1: Time (HH:MM format)
        len += sprintf(usbTxBuffer + len, "Time: %02d:%02d",
                      currentTime.hours, currentTime.minutes);

        // Add temperature
        if (DS3231_GetTemperature(&hi2c1, &temperature) == HAL_OK) {
            len += sprintf(usbTxBuffer + len, " | Temp: %.2f C", temperature);
        }
        len += sprintf(usbTxBuffer + len, "\r\n");

        // Line 2: AD Date (MM/DD/YYYY format with weekday)
        len += sprintf(usbTxBuffer + len, "AD:   %02d/%02d/%04d (%s)\r\n",
                      currentTime.month, currentTime.date, 2000 + currentTime.year,
                      engWeekdays[weekday]);

        // Line 3: BS Date (Nepali)
        if (GregorianToNepali(&gregorianDate, &nepaliDate)) {
            len += sprintf(usbTxBuffer + len, "BS:   %d %s %04d (%s)\r\n",
                          nepaliDate.day, NepaliMonthNames[nepaliDate.month - 1],
                          nepaliDate.year, NepaliWeekdayNames[weekday]);
        } else {
            len += sprintf(usbTxBuffer + len, "BS:   Conversion Error\r\n");
        }

        // Separator line
        len += sprintf(usbTxBuffer + len, "----------------------------------------\r\n");

        USB_Print(usbTxBuffer);
    } else {
        USB_Print("ERROR: Failed to read RTC!\r\n");
    }
}

/**
  * @brief  Set initial time on RTC (modify these values as needed)
  */
void SetInitialTime(void)
{
    DS3231_Time_t initTime;

    // Set your desired initial time here
    // Today: 23 December 2025 = 8 Poush 2082 BS (Tuesday/Mangalbar)
    initTime.year    = 25;    // 2025
    initTime.month   = 12;    // December
    initTime.date    = 24;    // 23rd
    initTime.day     = 4;     // Tuesday (1=Sunday, 2=Monday, 3=Tuesday, etc.)
    initTime.hours   = 17;    // 5:20 PM = 17:20
    initTime.minutes = 47;
    initTime.seconds = 0;

    if (DS3231_SetTime(&hi2c1, &initTime) == HAL_OK) {
        USB_Print("RTC time set successfully!\r\n");
    } else {
        USB_Print("ERROR: Failed to set RTC time!\r\n");
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USB_DEVICE_Init();  // Initialize USB Device (not PCD directly)
  /* USER CODE BEGIN 2 */

  // Wait for USB CDC to enumerate
  HAL_Delay(2000);

  USB_Print("\r\n================================================\r\n");
  USB_Print("    DS3231 RTC - USB CDC Demo\r\n");
  USB_Print("    EVON Electric VCU Project\r\n");
  USB_Print("    AD/BS (Nepali) Date Display\r\n");
  USB_Print("================================================\r\n\r\n");

  // Initialize DS3231 RTC
  if (DS3231_Init(&hi2c1) == HAL_OK) {
      rtcInitialized = 1;
      USB_Print("DS3231 RTC initialized successfully!\r\n\r\n");

#if SET_RTC_TIME == 1
      // Set initial time (only when SET_RTC_TIME is 1)
      SetInitialTime();
      USB_Print("\r\n*** TIME HAS BEEN SET! ***\r\n");
      USB_Print("*** Now set SET_RTC_TIME to 0 and re-flash ***\r\n\r\n");
#endif

  } else {
      USB_Print("ERROR: DS3231 not found! Check I2C connections.\r\n");
      USB_Print("  - SDA: PB9\r\n");
      USB_Print("  - SCL: PB8\r\n\r\n");
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // Print time every second
    if (rtcInitialized && (HAL_GetTick() - lastPrintTime >= TIME_PRINT_INTERVAL)) {
        lastPrintTime = HAL_GetTick();
        PrintCurrentTime();
    }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/* NOTE: USB_OTG_FS is initialized via MX_USB_DEVICE_Init() in usb_device.c
 * The PCD handle and low-level init are in USB_DEVICE/Target/usbd_conf.c
 */

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
