
/*------------------------------------------------------------------------------*/
/*							 Includes and dependencies						    */
/*------------------------------------------------------------------------------*/
#include "hal.h"

extern int __InitGPIO();
extern int __InitPWM();
extern int __InitUART();
extern int __InitI2C();
extern int __InitADC();
/*------------------------------------------------------------------------------*/
/*					  	   Function prototypes Implement					    */
/*------------------------------------------------------------------------------*/

/***************************** INIT_HELPER_FUNCTIONS ****************************/
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

    ret |= __InitGPIO();
    ret |= __InitPWM();
    ret |= __InitUART();
    ret |= __InitI2C();
    ret |= __InitADC();
    ret |= __InitSPI();

    return ret;
}

/*------------------------------------------------------------------------------*/
/*					  STM32G0xx Peripheral Interrupt Handlers  				    */
/*------------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
    HAL_IncTick();
}

