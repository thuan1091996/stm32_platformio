
/*------------------------------------------------------------------------------*/
/*							 Includes and dependencies						    */
/*------------------------------------------------------------------------------*/
#include "hal.h"

/*------------------------------------------------------------------------------*/
/*					  		   Preprocessor defines			    			    */
/*------------------------------------------------------------------------------*/
#define I2C_POLL_TIMEOUT		    0x1000U

/*------------------------------------------------------------------------------*/
/*					  		   Variables Declare			    			    */
/*------------------------------------------------------------------------------*/
I2C_HandleTypeDef __i2c_handle[2];

/*------------------------------------------------------------------------------*/
/*					  	  Function Private Implement		    			    */
/*------------------------------------------------------------------------------*/
int __initI2C1()
{
    int ret = SUCCESS;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/* I2C clock Configuration */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    ret |= ( HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) == HAL_OK ) ? SUCCESS : FAILURE;
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C Configuration */
    __i2c_handle[0].Instance = I2C1;
    __i2c_handle[0].Init.Timing = 0x10707DBC;
    __i2c_handle[0].Init.OwnAddress1 = 0;
    __i2c_handle[0].Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    __i2c_handle[0].Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    __i2c_handle[0].Init.OwnAddress2 = 0;
    __i2c_handle[0].Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    __i2c_handle[0].Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    __i2c_handle[0].Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    ret |= ( HAL_I2C_Init(&__i2c_handle[0]) == HAL_OK ) ? SUCCESS : FAILURE;

    /* GPIO Configuration
        PA9      ------> I2C1_SCL
        PA10     ------> I2C1_SDA
    */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_I2C1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure Analogue and Digital filter */
    ret |= ( HAL_I2CEx_ConfigAnalogFilter(&__i2c_handle[0], I2C_ANALOGFILTER_ENABLE) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_I2CEx_ConfigDigitalFilter(&__i2c_handle[0], 0) == HAL_OK ) ? SUCCESS : FAILURE;

    return ret;
}

int __initI2C2()
{
    int ret = SUCCESS;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* I2C clock Configuration */
    __HAL_RCC_I2C2_CLK_ENABLE();

    /* I2C Configuration */
    __i2c_handle[1].Instance = I2C2;
    __i2c_handle[1].Init.Timing = 0x10707DBC;
    __i2c_handle[1].Init.OwnAddress1 = 0;
    __i2c_handle[1].Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    __i2c_handle[1].Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    __i2c_handle[1].Init.OwnAddress2 = 0;
    __i2c_handle[1].Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    __i2c_handle[1].Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    __i2c_handle[1].Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    ret |= ( HAL_I2C_Init(&__i2c_handle[1]) == HAL_OK ) ? SUCCESS : FAILURE;

    /* GPIO Configuration
        PB10     ------> I2C2_SCL
        PB11     ------> I2C2_SDA
    */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configure Analogue and Digital filter */
    ret |= ( HAL_I2CEx_ConfigAnalogFilter(&__i2c_handle[1], I2C_ANALOGFILTER_ENABLE) == HAL_OK ) ? SUCCESS : FAILURE;
    ret |= ( HAL_I2CEx_ConfigDigitalFilter(&__i2c_handle[1], 0) == HAL_OK ) ? SUCCESS : FAILURE;

    return ret;
}

//Initialize I2C. Returns 0 on success, -1 on failure.
int __InitI2C()
{
    int ret = SUCCESS;

    ret |= __initI2C1();
    ret |= __initI2C2();

    return ret;
}

/*------------------------------------------------------------------------------*/
/*					  	 Function prototypes Implement	    				    */
/*------------------------------------------------------------------------------*/
bool hal__I2CEXISTS(uint8_t i2c_num, uint8_t ADDR)
{
    bool ret = false;

    if ( (1 <= i2c_num) && (i2c_num <= 2) )
    {
        ret = ( HAL_I2C_IsDeviceReady(&__i2c_handle[ i2c_num - 1], ADDR, 1, I2C_POLL_TIMEOUT) == HAL_OK );
    }

    return ret;
}

int hal__I2CREAD_uint8(uint8_t i2c_num, uint8_t ADDR, uint8_t REG, uint8_t *data)
{
    param_check( (1 <= i2c_num) && (i2c_num <= 2) );
    param_check( data );

    int ret = ( HAL_I2C_Mem_Read(&__i2c_handle[ i2c_num - 1], ADDR, REG, I2C_MEMADD_SIZE_8BIT, data, 1, I2C_POLL_TIMEOUT) == HAL_OK ) ? SUCCESS : FAILURE;

    return ret;
}

int hal__I2CREAD(uint8_t i2c_num, uint8_t ADDR, uint8_t REG, uint8_t *data, uint16_t len)
{
    param_check( (1 <= i2c_num) && (i2c_num <= 2) );
    param_check( data );

    int ret = ( HAL_I2C_Mem_Read(&__i2c_handle[ i2c_num - 1], ADDR, REG, I2C_MEMADD_SIZE_8BIT, data, len, I2C_POLL_TIMEOUT) == HAL_OK ) ? SUCCESS : FAILURE;

    return ret;
}

int hal__I2CWRITE_uint8(uint8_t i2c_num, uint8_t ADDR, uint8_t REG, uint8_t data)
{
    param_check( (1 <= i2c_num) && (i2c_num <= 2) );

    int ret = ( HAL_I2C_Mem_Write(&__i2c_handle[ i2c_num - 1], ADDR, REG, I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_POLL_TIMEOUT) == HAL_OK ) ? SUCCESS : FAILURE;

    return ret;
}

int hal__I2CWRITE(uint8_t i2c_num, uint8_t ADDR, uint8_t REG, uint8_t *data, uint16_t len)
{
    param_check( (1 <= i2c_num) && (i2c_num <= 2) );
    param_check( data );

    int ret = ( HAL_I2C_Mem_Write(&__i2c_handle[ i2c_num - 1], ADDR, REG, I2C_MEMADD_SIZE_8BIT, data, len, I2C_POLL_TIMEOUT) == HAL_OK ) ? SUCCESS : FAILURE;

    return ret;
}

