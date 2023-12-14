#include "main.h"
#include "hal.h"
#include <stdint.h>
#include <stdio.h>

#define TEST_ADC_API 	(0)
#define TEST_SPI_API 	(1)

#if (TEST_SPI_API == 1)
uint8_t flash_info[20] = {0};
volatile bool g_test_flash = false;
#include "MX25Series.h"

MX25Series_t flash_test = {0};
uint8_t buff_read[512] = {0};
uint8_t buff_write[10] = {1, 2, 3};

#define FLASH_RESET_PORT GPIOA
#define FLASH_RESET_PIN GPIO_PIN_11
#define FLASH_WP_PORT GPIOA
#define FLASH_WP_PIN GPIO_PIN_12
#define SPI1_NSS_PIN_NUMBER 10
#define FLASH_RESET_PIN_NUMBER 11
#define FLASH_WP_PIN_NUMBER 12
#endif /* End of (TEST_SPI_API == 1) */

extern UART_HandleTypeDef __uart_handle[2];
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&__uart_handle[0], (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

#include <errno.h>
#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO
int _write(int file, char *data, int len) {
  if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
      errno = EBADF;
      return -1;
  }
  HAL_UART_Transmit(&__uart_handle[0], (uint8_t *)data, (uint16_t) len, 0xFFFF); 
  return len;
}

int main(void)
{
    hal__init();

    while (1)
    {

#if (TEST_SPI_API != 0)

		uint8_t spi_port = 1;
		
		if (MX25Series_status_ok == MX25Series_init(&flash_test, &MX25R6435F_Chip_Def_Low_Power, SPI1_NSS_PIN_NUMBER,
													FLASH_RESET_PIN_NUMBER, FLASH_WP_PIN_NUMBER, 0, &spi_port))
		{
			if (MX25Series_status_ok == MX25Series_read_identification(&flash_test, &flash_info[0], &flash_info[1], &flash_info[2]))
			{
				printf("MX25Series_init ok\r\n");
                int elec_id = 0;
                MX25Series_read_electronic_signature(&flash_test, &elec_id);
                if(elec_id == 0x17)
                    printf("MX25Series_read_electronic_signature ok\r\n");
                else
                    printf("MX25Series_read_electronic_signature fail\r\n");

//				MX25Series_read_stored_data(&flash_test, true, 0x8000, 512, buff_read);
				//			MX25Series_set_write_enable(&flash_test, 1);
				//			MX25Series_write_stored_data(&flash_test, 0x1000, 10, buff_write);
				//			MX25Series_read_stored_data(&flash_test, true, 0x1000, 10, buff_read);
	            continue;
			}
			else
				printf("MX25Series_read_manufacture_and_device_id fail\r\n");
		}
		else
			printf("MX25Series_init fail\r\n");
#endif /* (TEST_SPI_API != 0) */


    }
}

