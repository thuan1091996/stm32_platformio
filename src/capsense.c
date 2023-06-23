
/*------------------------------------------------------------------------------*/
/*							 Includes and dependencies						    */
/*------------------------------------------------------------------------------*/
#include "capsense.h"

#include "hal.h"

/*------------------------------------------------------------------------------*/
/*					  		   Preprocessor defines			    			    */
/*------------------------------------------------------------------------------*/
#define READ_I2C_DATA_BUFFER    // comment out the macro to read individual i2c data

/*------------------------------------------------------------------------------*/
/*					  	 Function prototypes Implement	    				    */
/*------------------------------------------------------------------------------*/
int capsense_init(capsense_t *self, int INTERRUPT_PIN, int I2C_BUS_NUMBER, uint8_t I2C_SLAVE_ADDRRESS, capsense_mode_t mode)
{
    param_check( self );

    int ret = SUCCESS;

    /* Set zero value */
    memset(self, 0, sizeof(capsense_t));

    /* Init GPIO input pin */
    ret = hal__setState(INTERRUPT_PIN, 0);
    error_check( ret != SUCCESS, ret );

    /* Init handle value */
    self->mode              = mode;
    self->I2C_ADDRRESS      = I2C_SLAVE_ADDRRESS;
    self->I2C_BUS           = I2C_BUS_NUMBER;
    self->PIN_NUM_INTERRUPT = INTERRUPT_PIN;
    self->enable_I2C        = hal__I2CEXISTS(I2C_BUS_NUMBER, I2C_SLAVE_ADDRRESS);

    return ret;
}

int capsense_tick(capsense_t *self, uint32_t elapsed_ms)
{
    param_check( self );
    param_check( elapsed_ms > 0 );
    param_check( self->mode == CAPSENSE__MODE_EEPROM );

    int ret = FAILURE;
    capsense_values_t new_value = {0};
    uint8_t buffer[14] = {0};
    uint32_t time_start = HAL_GetTick();

    while ( (uint32_t)(HAL_GetTick() - time_start) <= elapsed_ms )  //cast to (uint32_t) to avoid overflow tick counter
    {
        if ( hal__read(self->PIN_NUM_INTERRUPT) == 0 )  //Low level
        {
            self->enable_I2C = hal__I2CEXISTS(self->I2C_BUS, self->I2C_ADDRRESS);

            if (self->enable_I2C)
            {
#ifdef READ_I2C_DATA_BUFFER
                ret = hal__I2CREAD(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_A, buffer, sizeof(buffer));
#else
                ret  = hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_A,        &buffer[0]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_B,        &buffer[1]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_MIN_UP,   &buffer[2]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_MIN_DOWN, &buffer[3]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_UP,       &buffer[4]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_DOWN,     &buffer[5]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_POWER,    &buffer[6]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_BUTTON_START,    &buffer[7]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_SLIDER,          &buffer[8]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_PROXIMITY,       &buffer[9]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_SLIDER1,         &buffer[10]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_SLIDER2,         &buffer[11]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_SLIDER3,         &buffer[12]);
                ret |= hal__I2CREAD_uint8(self->I2C_BUS, self->I2C_ADDRRESS, CAPSENSE__ADDRRESS_SLIDER4,         &buffer[13]);
#endif
                error_check( ret != SUCCESS, ret );

                new_value.button_A        = buffer[0];
                new_value.button_B        = buffer[1];
                new_value.button_min_up   = buffer[2];
                new_value.button_min_down = buffer[3];
                new_value.button_up       = buffer[4];
                new_value.button_down     = buffer[5];
                new_value.button_power    = buffer[6];
                new_value.button_start    = buffer[7];
                new_value.sliderValue     = buffer[8];
                new_value.proximity       = buffer[9];
                new_value.slider1         = buffer[10];
                new_value.slider2         = buffer[11];
                new_value.slider3         = buffer[12];
                new_value.slider4         = buffer[13];

                /* Check new data */
                if ( memcmp( &new_value, &self->values, sizeof(capsense_values_t) ) != 0 )
                {
                    ret = 1;
                }

                self->values = new_value;

                break;
            }
        }
    }

    return ret;
}

capsense_values_t capsense_get_values(capsense_t *self)
{
    capsense_values_t latest_value = {0};
    memset(&latest_value, 0, sizeof(capsense_values_t));

    if ( (self) && (self->mode == CAPSENSE__MODE_EEPROM) )
    {
        latest_value = self->values;
    }

    return latest_value;
}

