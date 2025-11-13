/* ----- LED Blink / Digital Write ----- */
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);   // Set pin high
HAL_Delay(1000);                                       // Wait 1 second
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET); // Set pin low
HAL_Delay(1000);                                       // Wait 1 second
/*********************************************************************************/

/* ----- Toggle LED ----- */
HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_13);               // Toggle the pin state
/*********************************************************************************/


/* ----- Digital Read ----- */
uint8_t value = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13); // Read pin state (0 or 1)
/*********************************************************************************/


/* ----- Analog Read (ADC) ----- */
HAL_ADC_Start(&hadc1);                               // Start ADC conversion (&hadc1 -> ADC1)
HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);    // Wait until conversion completes (no timeout)
uint16_t adc_value = HAL_ADC_GetValue(&hadc1);       // Read the converted value (0-4095 for 12-bit ADC)

// Optional: using custom timeout (e.g., 100 ms)
HAL_ADC_PollForConversion(&hadc1, 100);             // Wait max 100 ms
/*********************************************************************************/


/* ----- Analog Write (DAC) ----- */
uint32_t Value1 = 2048;                              // Example: 50% of 12-bit DAC
HAL_DAC_Start(&hdac, DAC_CHANNEL_1);                // Start DAC channel
HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Value1); // Set DAC output
/*********************************************************************************/


/* ----- Analog Write with PWM ----- */
uint32_t dutyValue = __HAL_TIM_GET_AUTORELOAD(&htim3) / 2; // 50% duty cycle
HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);                  // Start PWM
__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, dutyValue);  // Set duty cycle
/*********************************************************************************/

/*-------Print basic message through USB_CDC_COM PORT*/
char msg[] = "Hello STM32 World!\r\n";//decaration of message inside int main() function

while(1)
{
    CDC_Transmit_FS((uint8_t *)msg, strlen(msg));
    HAL_Delay(1000); // wait 1 second
}
/*********************************************************************************/


/*--------Use printf function to print through the usb_cdc_com port*/
#include "stdio.h"
int _write(int file, char *ptr, int len)
{
    CDC_Transmit_FS((uint8_t*)ptr, len);
    return len;
}

while (1)
    {
        printf("Hello there!!!!!\r\n");
        HAL_Delay(1000);
    }
/*********************************************************************************/
