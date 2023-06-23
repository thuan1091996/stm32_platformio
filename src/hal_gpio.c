
/*------------------------------------------------------------------------------*/
/*							 Includes and dependencies						    */
/*------------------------------------------------------------------------------*/
#include "hal.h"

/*------------------------------------------------------------------------------*/
/*					  		   Preprocessor defines			    			    */
/*------------------------------------------------------------------------------*/
#define GPIO_PIN_NUMBER_ALL     64

#define mPORT(num)              GPIO[ num - 1 ].Port
#define mPIN(num)               GPIO[ num - 1 ].Pin

/*------------------------------------------------------------------------------*/
/*					 		Typedefs enums & structs					  	    */
/*------------------------------------------------------------------------------*/
typedef struct 
{
    GPIO_TypeDef *Port;
    uint16_t      Pin;
} tGpioDef;

/*------------------------------------------------------------------------------*/
/*					  		   Variables Declare			    			    */
/*------------------------------------------------------------------------------*/
const tGpioDef GPIO[GPIO_PIN_NUMBER_ALL] = {

    { GPIOC, GPIO_PIN_11 /*1*/},     { GPIOC, GPIO_PIN_12 /*2*/},      { GPIOC, GPIO_PIN_13 /*3*/},       { GPIOC, GPIO_PIN_14 /*4*/},
    { GPIOC, GPIO_PIN_15 /*5*/},     { NULL,  0           /*6-VBAT*/}, { NULL,  0           /*7-VREF+*/}, { NULL,  0           /*8-VDD*/},
    { NULL,  0           /*9-VSS*/}, { GPIOF, GPIO_PIN_0  /*10*/},     { GPIOF, GPIO_PIN_1  /*11*/},      { NULL,  0           /*12-NRST*/},
    { GPIOC, GPIO_PIN_0  /*13*/},    { GPIOC, GPIO_PIN_1  /*14*/},     { GPIOC, GPIO_PIN_2  /*15*/},      { GPIOC, GPIO_PIN_3  /*16*/},
    { GPIOA, GPIO_PIN_0  /*17*/},    { GPIOA, GPIO_PIN_1  /*18*/},     { GPIOA, GPIO_PIN_2  /*19*/},      { GPIOA, GPIO_PIN_3  /*20*/},
    { GPIOA, GPIO_PIN_4  /*21*/},    { GPIOA, GPIO_PIN_5  /*22*/},     { GPIOA, GPIO_PIN_6  /*23*/},      { GPIOA, GPIO_PIN_7  /*24*/},
    { GPIOC, GPIO_PIN_4  /*25*/},    { GPIOC, GPIO_PIN_5  /*26*/},     { GPIOB, GPIO_PIN_0  /*27*/},      { GPIOB, GPIO_PIN_1  /*28*/},
    { GPIOB, GPIO_PIN_2  /*29*/},    { GPIOB, GPIO_PIN_10 /*30*/},     { GPIOB, GPIO_PIN_11 /*31*/},      { GPIOB, GPIO_PIN_12 /*32*/},
    { GPIOB, GPIO_PIN_13 /*33*/},    { GPIOB, GPIO_PIN_14 /*34*/},     { GPIOB, GPIO_PIN_15 /*35*/},      { GPIOA, GPIO_PIN_8  /*36*/},
    { GPIOA, GPIO_PIN_9  /*37*/},    { GPIOC, GPIO_PIN_6  /*38*/},     { GPIOC, GPIO_PIN_7  /*39*/},      { GPIOD, GPIO_PIN_8  /*40*/},
    { GPIOD, GPIO_PIN_9  /*41*/},    { GPIOA, GPIO_PIN_10 /*42*/},     { GPIOA, GPIO_PIN_11 /*43*/},      { GPIOA, GPIO_PIN_12 /*44*/},
    { GPIOA, GPIO_PIN_13 /*45*/},    { GPIOA, GPIO_PIN_14 /*46*/},     { GPIOA, GPIO_PIN_15 /*47*/},      { GPIOC, GPIO_PIN_8  /*48*/},
    { GPIOC, GPIO_PIN_9  /*49*/},    { GPIOD, GPIO_PIN_0  /*50*/},     { GPIOD, GPIO_PIN_1  /*51*/},      { GPIOD, GPIO_PIN_2  /*52*/},
    { GPIOD, GPIO_PIN_3  /*53*/},    { GPIOD, GPIO_PIN_4  /*54*/},     { GPIOD, GPIO_PIN_5  /*55*/},      { GPIOD, GPIO_PIN_6  /*56*/},
    { GPIOB, GPIO_PIN_3  /*57*/},    { GPIOB, GPIO_PIN_4  /*58*/},     { GPIOB, GPIO_PIN_5  /*59*/},      { GPIOB, GPIO_PIN_6  /*60*/},
    { GPIOB, GPIO_PIN_7  /*61*/},    { GPIOB, GPIO_PIN_8  /*62*/},     { GPIOB, GPIO_PIN_9  /*63*/},      { GPIOC, GPIO_PIN_10 /*64*/}
};

/*------------------------------------------------------------------------------*/
/*					  	  Function Private Implement		    			    */
/*------------------------------------------------------------------------------*/
//Initialize GPIO. Returns 0 on success, -1 on failure.
int __InitGPIO()
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    return SUCCESS;
}

/*------------------------------------------------------------------------------*/
/*					  	 Function prototypes Implement	    				    */
/*------------------------------------------------------------------------------*/
int hal__setHigh(uint8_t pinNum)
{
    param_check( (1 <= pinNum) && (pinNum <= GPIO_PIN_NUMBER_ALL) );
    param_check( (mPORT(pinNum)) && (mPIN(pinNum)) );

    HAL_GPIO_WritePin(mPORT(pinNum), mPIN(pinNum), GPIO_PIN_SET);
    return SUCCESS;
}

int hal__setLow(uint8_t pinNum)
{
    param_check( (1 <= pinNum) && (pinNum <= GPIO_PIN_NUMBER_ALL) );
    param_check( (mPORT(pinNum)) && (mPIN(pinNum)) );

    HAL_GPIO_WritePin(mPORT(pinNum), mPIN(pinNum), GPIO_PIN_RESET);
    return SUCCESS;
}

int hal__read(uint8_t pinNum)
{
    param_check( (1 <= pinNum) && (pinNum <= GPIO_PIN_NUMBER_ALL) );
    param_check( (mPORT(pinNum)) && (mPIN(pinNum)) );

    return HAL_GPIO_ReadPin(mPORT(pinNum), mPIN(pinNum));
}

int hal__setState(uint8_t pinNum, uint8_t state)
{
    param_check( (1 <= pinNum) && (pinNum <= GPIO_PIN_NUMBER_ALL) );
    param_check( (0 <= state) && (state <= 2) );
    param_check( (mPORT(pinNum)) && (mPIN(pinNum)) );

    GPIO_InitTypeDef GPIO_Init = {0};

    GPIO_Init.Pin = mPIN(pinNum);

    if (state == 0) //  Set pin as input
    {
        GPIO_Init.Mode = GPIO_MODE_INPUT;
        GPIO_Init.Pull = GPIO_NOPULL;
    }
    else if (state == 1) // Set pin as output
    {
        GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_Init.Pull = GPIO_NOPULL;
    }
    else if (state == 2) // Set pin as high impedance
    {
        GPIO_Init.Mode = GPIO_MODE_ANALOG;
        GPIO_Init.Pull = GPIO_NOPULL;
    }

    HAL_GPIO_Init(mPORT(pinNum), &GPIO_Init);

    return SUCCESS;
}

