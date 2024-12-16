#include "main.h"
#include "hal.h"

#define APP_TEST_PWM            	  (1)
#define APP_TEST_WAKEUP_PC13        (1)
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Static Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

#if (APP_TEST_WAKEUP_PC13 != 0)
/**
  * @brief PB13 interrupt configured as wakeup source
  */
void pc13_wakeup_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin PC13 to wake up the device from stop mode */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
 /* USER CODE BEGIN EXTI4_15_IRQn 0 */

 /* USER CODE END EXTI4_15_IRQn 0 */
 HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
 /* USER CODE BEGIN EXTI4_15_IRQn 1 */

 /* USER CODE END EXTI4_15_IRQn 1 */
}

#endif /* End of (APP_TEST_WAKEUP_PC13 != 0)) */

#if (APP_TEST_PWM != 0)
int __InitPWM();
int hal__setDutyCycle(uint8_t channel_num, uint16_t dutyCycle_tenth);

void pwm_custom_task(void *pvParameters)
{
    // Test PWM APIS with max duty = 1000%
    // Increase the duty cycle of each pin 50% every 300ms
        hal__setDutyCycle(0, 250);
        hal__setDutyCycle(1, 500);
        hal__setDutyCycle(2, 750);
        hal__setDutyCycle(3, 999);
        HAL_Delay(50);
}
#endif /* End of (APP_TEST_PWM != 0) */

int main(void)
{
	if (hal__init() != 0)
	{
		printf("HAL init failed");
		return -1;
	}
#if (APP_TEST_WAKEUP_PC13 != 0)
    pc13_wakeup_init();
#endif /* End of (APP_TEST_WAKEUP_PC13 != 0) */

    while(1)
    {

#if (APP_TEST_PWM != 0)
        pwm_custom_task(NULL);
#endif /* End of (APP_TEST_PWM != 0) */

        hal_lowpowermode_enter();
    }
}
