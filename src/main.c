#include "main.h"
#include "hal.h"

void LED_Init();

int main(void)
{
    hal__init();

    while (1)
    {
        ;
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

