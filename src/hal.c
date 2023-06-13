
/*------------------------------------------------------------------------------*/
/*							 Includes and dependencies						    */
/*------------------------------------------------------------------------------*/
#include "hal.h"

extern int __InitUART();

/*------------------------------------------------------------------------------*/
/*					  	   Function prototypes Implement					    */
/*------------------------------------------------------------------------------*/

/***************************** INIT_HELPER_FUNCTIONS ****************************/
//Note: These internalFunctions are used to initialize the various peripherals. They are not intended to be called directly. (The peripherals are abstracted away by hal__init()
//Initialize PWM. Returns 0 on success, -1 on failure.
int __InitPWM()
{
    int ret = SUCCESS;
    return ret;
}

//Initialize I2C. Returns 0 on success, -1 on failure.
int __InitI2C()
{
    int ret = SUCCESS;
    return ret;
}

//Initialize GPIO. Returns 0 on success, -1 on failure.
int __InitGPIO()
{
    int ret = SUCCESS;
    return ret;
}

//Initialize Clocks. Returns 0 on success, -1 on failure. (Note: If this happens automatically, this function is not needed.)
int __InitClocks()
{
    int ret = SUCCESS;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 8;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    ret |= (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK) ? SUCCESS : FAILURE;

    /** Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    ret |= (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) == HAL_OK) ? SUCCESS : FAILURE;

    return ret;
}

int hal__init()
{
    int ret = SUCCESS;

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    ret |= (HAL_Init() == HAL_OK) ? SUCCESS : FAILURE;

    /* Configure the system clock */
    ret |= __InitClocks();

    ret |= __InitUART();

    return ret;
}

/**************************** TIMER_HELPER_FUNCTIONS ****************************/

//Note: These internal functions are not intended to be called directly. (The timer peripherals are abstracted away by hal__setDutyCycle();
//Set Timer Duty Cycle, in tenths of percent. For Example, Passing (1, 1, 50) will set Timer 1 Channel 1 to 5.0%. Returns 0 on success, -1 on failure.
int __hal__setTimerDutyCycle(uint8_t timerNum, uint8_t channelNum, uint16_t dutyCycle_tenth)
{
    int ret = SUCCESS;
    return ret;
}

//Set Timer Frequency, in Hz. For Example, Passing (1, 1000) will set Timer 1 to 1kHz. Returns 0 on success, -1 on failure.
int __hal__setTimerFrequency(uint8_t timerNum, uint32_t freq)
{
    int ret = SUCCESS;
    return ret;
}

int hal__setDutyCycle(uint8_t channelNum, uint16_t dutyCycle_tenth)
{
    int ret = SUCCESS;
    return ret;
}
/***************************** GPIO_HELPER_FUNCTIONS ****************************/
int hal__setHigh(uint8_t pinNum)
{
    int ret = SUCCESS;
    return ret;
}

int hal__setLow(uint8_t pinNum)
{
    int ret = SUCCESS;
    return ret;
}

int hal__setState(uint8_t pinNum, uint8_t state)
{
    int ret = SUCCESS;
    return ret;
}

/***************************** I2C_HELPER_FUNCTIONS *****************************/
bool hal__I2CEXISTS(uint8_t i2c_num, uint8_t ADDR)
{
    return true;
}

int hal__I2CREAD_uint8(uint8_t i2c_num, uint8_t ADDR, uint8_t REG, uint8_t *data)
{
    int ret = SUCCESS;
    return ret;
}

int hal__I2CREAD(uint8_t i2c_num, uint8_t ADDR, uint8_t REG, uint8_t *data, uint16_t len)
{
    int ret = SUCCESS;
    return ret;
}

int hal__I2CWRITE_uint8(uint8_t i2c_num, uint8_t ADDR, uint8_t REG, uint8_t data)
{
    int ret = SUCCESS;
    return ret;
}

int hal__I2CWRITE(uint8_t i2c_num, uint8_t ADDR, uint8_t REG, uint8_t *data, uint16_t len)
{
    int ret = SUCCESS;
    return ret;
}

/*------------------------------------------------------------------------------*/
/*					  STM32G0xx Peripheral Interrupt Handlers  				    */
/*------------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
    HAL_IncTick();
}

