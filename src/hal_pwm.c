
/*------------------------------------------------------------------------------*/
/*							 Includes and dependencies						    */
/*------------------------------------------------------------------------------*/
#include "hal.h"

/*------------------------------------------------------------------------------*/
/*					  		   Preprocessor defines			    			    */
/*------------------------------------------------------------------------------*/
#define PWM_TIM_ARR_MAX		65536	//16bit counter

/*------------------------------------------------------------------------------*/
/*					  		    Variables Declare			    			    */
/*------------------------------------------------------------------------------*/
TIM_HandleTypeDef __pwm_timer[2];
uint16_t __pwm_duty[8] = {0};

/*------------------------------------------------------------------------------*/
/*					  		Function Private Implement		    			    */
/*------------------------------------------------------------------------------*/
int __initTIM1()
{
    int ret = SUCCESS;
    TIM_OC_InitTypeDef sConfigOC = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Timer clock Configuration */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* Timer Configuration */
    __pwm_timer[0].Instance = TIM1;
    __pwm_timer[0].Init.Prescaler = 0;
    __pwm_timer[0].Init.CounterMode = TIM_COUNTERMODE_UP;
    __pwm_timer[0].Init.Period = 65535;
    __pwm_timer[0].Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    __pwm_timer[0].Init.RepetitionCounter = 0;
    __pwm_timer[0].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    ret |= ( HAL_TIM_PWM_Init(&__pwm_timer[0]) == HAL_OK ) ? SUCCESS : FAILURE;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    ret |= ( HAL_TIM_PWM_ConfigChannel(&__pwm_timer[0], &sConfigOC, TIM_CHANNEL_1) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_ConfigChannel(&__pwm_timer[0], &sConfigOC, TIM_CHANNEL_2) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_ConfigChannel(&__pwm_timer[0], &sConfigOC, TIM_CHANNEL_3) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_ConfigChannel(&__pwm_timer[0], &sConfigOC, TIM_CHANNEL_4) == HAL_OK ) ? SUCCESS : FAILURE;

    /* GPIO Configuration
        PA8     ------> TIM1_CH1
        PC9     ------> TIM1_CH2
        PB6     ------> TIM1_CH3
        PC11    ------> TIM1_CH4
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_11;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Timer interrupt Configuration */
    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);

    return ret;
}

int __initTIM3()
{
    int ret = SUCCESS;
    TIM_OC_InitTypeDef sConfigOC = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Timer clock Configuration */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* Timer Configuration */
    __pwm_timer[1].Instance = TIM3;
    __pwm_timer[1].Init.Prescaler = 0;
    __pwm_timer[1].Init.CounterMode = TIM_COUNTERMODE_UP;
    __pwm_timer[1].Init.Period = 65535;
    __pwm_timer[1].Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    __pwm_timer[1].Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    ret |= ( HAL_TIM_PWM_Init(&__pwm_timer[1]) == HAL_OK ) ? SUCCESS : FAILURE;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    ret |= ( HAL_TIM_PWM_ConfigChannel(&__pwm_timer[1], &sConfigOC, TIM_CHANNEL_1) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_ConfigChannel(&__pwm_timer[1], &sConfigOC, TIM_CHANNEL_2) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_ConfigChannel(&__pwm_timer[1], &sConfigOC, TIM_CHANNEL_3) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_ConfigChannel(&__pwm_timer[1], &sConfigOC, TIM_CHANNEL_4) == HAL_OK ) ? SUCCESS : FAILURE;

    /* GPIO Configuration
        PA6     ------> TIM3_CH1
        PA7     ------> TIM3_CH2
        PB0     ------> TIM3_CH3
        PB1     ------> TIM3_CH4
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
    
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Timer interrupt Configuration */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    return ret;
}

//Note: These internal functions are not intended to be called directly. (The timer peripherals are abstracted away by hal__setDutyCycle();
//Set Timer Duty Cycle, in tenths of percent. For Example, Passing (1, 1, 50) will set Timer 1 Channel 1 to 5.0%. Returns 0 on success, -1 on failure.
int __hal__setTimerDutyCycle(uint8_t timerNum, uint8_t channelNum, uint16_t dutyCycle_tenth)
{
	param_check( (1 <= timerNum) && (timerNum <= 2) );
	param_check( (1 <= channelNum) && (channelNum <= 8) );
	param_check( dutyCycle_tenth <= 1000 );

	uint16_t ccr_value = ( (__HAL_TIM_GET_AUTORELOAD(&__pwm_timer[timerNum - 1]) + 1) * (dutyCycle_tenth / 10.0) / 100.0 );
	__pwm_duty[channelNum - 1] = ccr_value;

    return SUCCESS;
}

//Set Timer Frequency, in Hz. For Example, Passing (1, 1000) will set Timer 1 to 1kHz. Returns 0 on success, -1 on failure.
int __hal__setTimerFrequency(uint8_t timerNum, uint32_t freq)
{
    param_check( (1 <= timerNum) && (timerNum <= 2) );

	uint32_t arr_max;
	uint16_t psc, arr;

	arr_max = HAL_RCC_GetPCLK1Freq() / freq;
	psc = arr_max / PWM_TIM_ARR_MAX + 1;
	arr = arr_max / psc;

	__HAL_TIM_SET_PRESCALER(&__pwm_timer[timerNum - 1], psc - 1) ;
	__HAL_TIM_SET_AUTORELOAD(&__pwm_timer[timerNum - 1], arr - 1);

    return SUCCESS;
}

//Initialize PWM. Returns 0 on success, -1 on failure.
int __InitPWM()
{
    int ret = SUCCESS;

    ret |= __initTIM1();
    ret |= __initTIM3();

    ret |= __hal__setTimerFrequency(1, 1000);
    ret |= __hal__setTimerFrequency(2, 2000);

    ret |= ( HAL_TIM_PWM_Start_IT(&__pwm_timer[0], TIM_CHANNEL_1) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_Start_IT(&__pwm_timer[0], TIM_CHANNEL_2) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_Start_IT(&__pwm_timer[0], TIM_CHANNEL_3) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_Start_IT(&__pwm_timer[0], TIM_CHANNEL_4) == HAL_OK ) ? SUCCESS : FAILURE;

    ret |= ( HAL_TIM_PWM_Start_IT(&__pwm_timer[1], TIM_CHANNEL_1) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_Start_IT(&__pwm_timer[1], TIM_CHANNEL_2) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_Start_IT(&__pwm_timer[1], TIM_CHANNEL_3) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_TIM_PWM_Start_IT(&__pwm_timer[1], TIM_CHANNEL_4) == HAL_OK ) ? SUCCESS : FAILURE;

    return ret;
}

/*------------------------------------------------------------------------------*/
/*					  	   Function prototypes Implement					    */
/*------------------------------------------------------------------------------*/
int hal__setDutyCycle(uint8_t channelNum, uint16_t dutyCycle_tenth)
{
	param_check( (1 <= channelNum) && (channelNum <= 8) );
	param_check( dutyCycle_tenth <= 1000 );

	int ret = FAILURE;

	if ( (1 <= channelNum) && (channelNum <= 4) )
	{
		ret = __hal__setTimerDutyCycle(1, channelNum, dutyCycle_tenth);
	}
	else if ( (5 <= channelNum) && (channelNum <= 8) )
	{
		ret = __hal__setTimerDutyCycle(2, channelNum, dutyCycle_tenth);
	}

    return ret;
}

/*------------------------------------------------------------------------------*/
/*					  STM32G0xx Peripheral Interrupt Handlers  				    */
/*------------------------------------------------------------------------------*/
void TIM1_CC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&__pwm_timer[0]);
}

void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&__pwm_timer[1]);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == __pwm_timer[0].Instance)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, __pwm_duty[0]);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, __pwm_duty[1]);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, __pwm_duty[2]);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, __pwm_duty[3]);
	}
	else if (htim->Instance == __pwm_timer[1].Instance)
	{
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, __pwm_duty[4]);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, __pwm_duty[5]);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, __pwm_duty[6]);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, __pwm_duty[7]);
	}
}

