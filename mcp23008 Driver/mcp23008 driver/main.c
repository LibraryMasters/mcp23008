#include "mcp23008_driver_basic.h"


void hex_to_bin(uint16_t hex_number_in, char *bin_number_out);
char dataOut[];

uint8_t buffer_des[4];


int main()
{
    mcp23008_basic_initialize(MCP23008_I2C_ADDRESS_PIN_A110);
    mcp23008_info(&mcp23008_handle);

    mcp23008_interface_debug_print("Chip name :\t%s\n\r", mcp23008_handle.info.chip_name);
    mcp23008_interface_debug_print("Manufacturer: \t%s\n\r",  mcp23008_handle.info.manufacturer_name);

    mcp23008_interface_debug_print("Interface: \t%s\n\r",  mcp23008_handle.info.interface);
    mcp23008_interface_debug_print("Supply voltage max : \t%0.2fV\n\r",  mcp23008_handle.info.supply_voltage_max_v);
    mcp23008_interface_debug_print("Supply voltage min: \t%0.2fV\n\r",  mcp23008_handle.info.supply_voltage_min_v);
    mcp23008_interface_debug_print("Maximum current: \t%0.1fmA\n\r",  mcp23008_handle.info.max_current_ma);
    mcp23008_interface_debug_print("Temperature Max: \t%.1fC\n\r",  mcp23008_handle.info.temperature_max);
    mcp23008_interface_debug_print("Temperature Min: \t%.1fC\n\r",  mcp23008_handle.info.temperature_min);
    mcp23008_interface_debug_print("Driver version: \tV%.1f.%.2d\n\r", ( mcp23008_handle.info.driver_version / 1000), (uint8_t)( mcp23008_handle.info.driver_version - (uint8_t)( mcp23008_handle.info.driver_version / 100)*100));


    memset((uint8_t *)buffer_des, 0, 4);   /**< clear buffer before write */
    return 0;
}


/**
 * @brief convert hexadecimal to binary
 * @param[in] hex_number_in in the number to pass into the function
 * @param[out] *bin_number_out point to converted binary string
 * @return status code
 * @note the function currently support 2 byte
 */
void hex_to_bin(uint16_t hex_number_in, char *bin_number_out) {
    uint8_t index;
    for (index = 0; index < 8; index++) {     /**< 16 and 0x8000 for 2 byte*/
        if (hex_number_in & (0x80 >> index))
            *bin_number_out = '1';
        else
            *bin_number_out = '0';

        bin_number_out++;
    }
    *bin_number_out = '\0';
}
