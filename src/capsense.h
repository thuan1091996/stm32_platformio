//capsense.h
//a file which conatins function prototpyes for the capsense driver - configured to read from Virdtual EEPROM by default.

//The virtual EEPROM is a Cypress PSOC device which is running a custom touch controller. It will support standard EEPROM Read commands.

#ifndef CAPSENSE_H_
#define CAPSENSE_H_

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*							 Includes and dependencies						    */
/*------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "hal.h"

/*------------------------------------------------------------------------------*/
/*					 		Typedefs enums & structs					  	    */
/*------------------------------------------------------------------------------*/
typedef enum {
    CAPSENSE__MODE_NOT_INIT,
    CAPSENSE__MODE_EEPROM
} capsense_mode_t;

typedef enum {
    CAPSENSE__ADDRRESS_BUTTON_A         = 0x00,
    CAPSENSE__ADDRRESS_BUTTON_B         = 0x01,
    CAPSENSE__ADDRRESS_BUTTON_MIN_UP    = 0x02,
    CAPSENSE__ADDRRESS_BUTTON_MIN_DOWN  = 0x03,
    CAPSENSE__ADDRRESS_BUTTON_UP        = 0x04,
    CAPSENSE__ADDRRESS_BUTTON_DOWN      = 0x05,
    CAPSENSE__ADDRRESS_BUTTON_POWER     = 0x06,
    CAPSENSE__ADDRRESS_BUTTON_START     = 0x07,
    CAPSENSE__ADDRRESS_SLIDER           = 0x08,
    CAPSENSE__ADDRRESS_PROXIMITY        = 0x09,
    CAPSENSE__ADDRRESS_SLIDER1          = 0x0A,
    CAPSENSE__ADDRRESS_SLIDER2          = 0x0B,
    CAPSENSE__ADDRRESS_SLIDER3          = 0x0C,
    CAPSENSE__ADDRRESS_SLIDER4          = 0x0D
} capsense_ADDRRESS_t;

typedef struct capsense_values {
    bool button_A;
    bool button_B;
    bool button_min_up;
    bool button_min_down;
    bool button_up;
    bool button_down;
    bool button_power;
    bool button_start;
    uint8_t sliderValue; //range 1-10, value 0 means not active.
    uint8_t proximity;   //range 1-255, value 0 means not active.
    uint8_t slider1;     //range 1-3, value 0 means not active.
    uint8_t slider2;     //range 1-3, value 0 means not active.
    uint8_t slider3;     //range 1-3, value 0 means not active.
    uint8_t slider4;     //range 1-3, value 0 means not active.
} capsense_values_t;

typedef struct {
    capsense_mode_t mode;
    capsense_values_t values;
    uint8_t I2C_ADDRRESS;   //the I2C address of the device.
    int I2C_BUS;            //the I2C bus number which the device is connected to.
    int PIN_NUM_INTERRUPT;  //the interrupt pin which will trigger a read.
    bool enable_I2C;        //if I2C address does not exist, then don't attempt to read/write from the device to prevent errors.
} capsense_t;

/*-----------------------------------------------------------------------------*/
/*							    Function prototypes					     	   */
/*-----------------------------------------------------------------------------*/

//This driver does not allocate memory using MALLOC, rather, the memory is allocated elsewhere and an unintized struct is passed to the init function.
//This is done to allow the driver to be used in many different environments and prevent memory leaks.
//Initializes the capsense driver, if mode == CAPSENSE__MODE_EEPROM, then the driver will read from the virtual EEPROM on I2C bus <I2C_BUS_NUMBER>.
//If INTERRUPT_PIN transistions from "HIGH" to "LOW", this will trigger a read.
//returns 0 if successful, -1 if not.
int capsense_init(capsense_t *self, int INTERRUPT_PIN, int I2C_BUS_NUMBER, uint8_t I2C_SLAVE_ADDRRESS, capsense_mode_t mode);

//checks the the interrupt pin, if it is "LOW", then the driver will read the capsense values and store them in the "values" field of the capsense_t struct.
//returns 0 if successful, 1 if there is new data, -1 if error.
int capsense_tick(capsense_t *self, uint32_t elapsed_ms);

//Returns the latest values.
capsense_values_t capsense_get_values(capsense_t *self);

#ifdef __cplusplus
}
#endif

#endif