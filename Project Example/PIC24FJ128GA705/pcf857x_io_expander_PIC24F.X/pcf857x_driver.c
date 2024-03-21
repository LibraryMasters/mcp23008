/**
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * File:   pcf857x_driver.c
 * Author: Cedric Akilimali
 *
 * Created on November 23, 2023, 14:22 PM
 */

#include "pcf857x_driver.h"

/**
* @brief chip information definition
*/
#define CHIP_NAME                "PCF857X"                /**< chip name */
#define INTERFACE                 "I2C"                  /**< interface protocol */
#define MANUFACTURER_NAME         "NXP"                  /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.5f                   /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        6.0f                   /**< chip max supply voltage */
#define MAX_CURRENT               100                    /**< chip max current (μA)*/
#define MIN_CURRENT               300                    /**< chip min current (μA)*/
#define TEMPERATURE_MIN           -40.0f                 /**< chip min operating temperature (°C) */
#define TEMPERATURE_MAX           85.0f                  /**< chip max operating temperature (°C) */
#define MCU_FLASH_MIN             32                     /**< Micro-controller minimum recommended flash size (kB) */
#define MCU_RAM_MIN               2                      /**< Micro-controller minimum recommended RAM size (KB)*/
#define DRIVER_VERSION            1000                   /**< driver version */

/**
* @brief i2c write byte
* @param[in] *pHandle points to PCF857X handle structure
* @param[in] u8Reg is the slave device register address
* @param[in] *pBuf point to data to write
* @param[in] u8Length is the data length to write (number of byte)
* @return status code
            - 0 success
            - 1 failed to write
* @note none
*/
uint8_t a_pcf857x_i2c_write(pcf857x_handle_t *const pHandle, uint8_t u8Reg, void *pBuf, uint8_t u8Length)
{
    if(pHandle->i2c_write(pHandle->i2c_address, u8Reg, (uint8_t*)pBuf, u8Length) !=  PCF857X_DRV_OK)
    {
        return 1;                                      /**< return an error if failed to execute */
    }
    return 0;                                          /**< return success */
}

/**
* @brief i2c read byte
* @param[in] *pHandle points to PCF857X handle structure
* @param[in] u8Reg is the slave device register address
* @param[out] *pBuf point to data to read
* @param[in] u8Length is the data length to read (number of byte)
* @return status code
*          - 0 success
*          - 1 failed to read
* @note none
*/

uint8_t a_pcf857x_i2c_read(pcf857x_handle_t  *const pHandle, uint16_t u8Reg, void *pBuf, uint8_t u8Length)
{
    if(pHandle->i2c_read(pHandle->i2c_address, u8Reg, (uint8_t*)pBuf, u8Length) != PCF857X_DRV_OK)
    {
        return 1;                                       /**< return an error if failed to execute */
    }
    return 0;                                           /**< return success */
}

/**
* @brief This function prints the error message
* @param[in] *pHandle points to PCF857X handle structure
* @param[in] *pBuffer point to the string to be printed
* @return none
* @note   none
*/
void a_pcf857x_print_error_msg(pcf857x_handle_t *const pHandle, char *const pBuffer)
{
#ifdef PCF857X_DEBUG_MODE
    pHandle->debug_print("PCF857X: failed to %s.\r\n", pBuffer);
#endif // PCF857X_DEBUG_MODE
}

/**
 * @brief     This function initialize the chip
 * @param[in] pHandle points to PCF857X pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 i2c initialization failed
 *            - 2 pHandle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t pcf857x_init(pcf857x_handle_t *const pHandle)
{
      if (pHandle == NULL)
        return 2;
    if (pHandle->debug_print == NULL)
        return 3;
    if (pHandle->i2c_init == NULL)
    {
#ifdef PCF857X_DEBUG_MODE
        pHandle->debug_print("PCF857X: i2c initialize is null\n");
#endif
        return 3;
    }
    if (pHandle->i2c_deinit == NULL)
    {
#ifdef PCF857X_DEBUG_MODE
        pHandle->debug_print("PCF857X: i2c_deint is null\n");
#endif
        return 3;
    }
    if (pHandle->i2c_read == NULL)
    {
#ifdef PCF857X_DEBUG_MODE
        pHandle->debug_print("PCF857X: i2c_read is null\n");
#endif
        return 3;
    }
    if (pHandle->i2c_write == NULL)
    {
#ifdef PCF857X_DEBUG_MODE
        pHandle->debug_print("PCF857X: i2c_write is null\n");
#endif
        return 3;
    }
    if (pHandle->delay_ms == NULL)
    {
#ifdef PCF857X_DEBUG_MODE
        pHandle->debug_print("PCF857X: delay_ms\n");
#endif
    }
    if (pHandle->i2c_init())
    {
#ifdef PCF857X_DEBUG_MODE
        pHandle->debug_print("PCF857X: i2c initialize failed\n");
#endif
        return 1;
    }

    pHandle->inited = 1; /* flag finish initialization */

    return 0;
}

/**
 * @brief     This function close the chip
 * @param[in] pHandle points to PCF857X pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 i2c deinit failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t pcf857x_deinit(pcf857x_handle_t *const pHandle)
{
    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pHandle->i2c_deinit() != 0)
    {
        a_pcf857x_print_error_msg(pHandle, "de-initialize i2c");
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief     This function set the address pin
 * @param[in] pHandle points to a PCF857X pHandle structure
 * @param[in] addr_pin is the i2c addresses pins
 * @return  status code
 *            - 0 success
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t pcf857x_set_addr_pin(pcf857x_handle_t *const pHandle, pcf857x_address_t addr_pin)
{
    uint8_t default_address;

    if(pHandle == NULL)
        return 2;      /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(pHandle->device_variant == PCF8574A_VARIANT)
        default_address = PCF8574A_ADDRESS_DEFAULT;
    else
        default_address = PCF857X_ADDRESS_DEFAULT;

    pHandle->i2c_address = default_address;
    pHandle->i2c_address |= addr_pin;

    return 0;                   /**< success */
}

/**
 * @brief     This function set the address pin
 * @param[in] pHandle points to a PCF857X pHandle structure
 * @param[out] addr_pin point to both i2c addresses pins
 * @return  status code
 *            - 0 success
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t pcf857x_get_addr_pin(pcf857x_handle_t *const pHandle, pcf857x_address_t *pAddr_pin)
{
    uint8_t default_address;

    if (pHandle == NULL) /* check handle */
    return 2; /* return error */

    if(pHandle->device_variant == PCF8574A_VARIANT)
        default_address = PCF8574A_ADDRESS_DEFAULT;
    else
        default_address = PCF857X_ADDRESS_DEFAULT;

    *pAddr_pin = (pcf857x_address_t) ((pHandle->i2c_address | (default_address)));

     return 0;
}

/**
 * @brief     This function set the chip variant
 * @param[in] pHandle points to PCF857X handle structure
 * @param[in] variant is the chip variant type
 * @return    status code
 *            - 0 success
 *            - 1 set variant failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid variant
 * @note      none
 */
uint8_t pcf857x_set_variant(pcf857x_handle_t *const pHandle, pcf857x_variant_t variant)
{
    if(pHandle == NULL)
        return 2;      /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    if(variant > 0x02 || variant < 0 ){
       a_pcf857x_print_error_msg(pHandle, "set variant, invalid");
        return 4;      /**< invalid variant return error*/
    }

    pHandle->device_variant = variant;
    if(pHandle->device_variant != variant)
    {
        return 1;           /**< failed error */
    }

    return 0;           /**< success */
}

/**
 * @brief     This function get the chip variant
 * @param[in] pHandle points to PCF857X handle structure
 * @param[out] pVariant point to the chip variant type
 * @return    status code
 *            - 0 success
 *            - 1 get variant failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf857x_get_variant(pcf857x_handle_t *const pHandle, pcf857x_variant_t *pVariant)
{
     if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    *pVariant = pHandle->device_variant;
    if( *pVariant != pHandle->device_variant)
    {
        return 1;           /**< failed error */
    }

    return 0;               /**< success */
}

/**
 * @brief Write logic level to gpio pin
 * @param[in] *handle point to PCF857X handle structure
 * @param[in] pin is the gpio pin to write (GP0 - GP17)
 * @param[in] logic_level is the logic level to assign to the pin
 * @return status code
 *          - 0 success
 *          - 1 failed to write pin
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 *          - 4 invalid gpio pin
 * @note    none
 */
uint8_t pcf857x_pin_write(pcf857x_handle_t *const pHandle, pcf857x_gpio_port_t pin, pcf857x_port_logic_level_t logic_level)
{
    uint8_t status[2];
    uint16_t formated_data;

    memset(status, 0, 2);                    /**< clear buffer before write */

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */
    if(pin > PCF857X_GPIO_PIN_17 || pin < PCF857X_GPIO_PIN_0)
        return 4;      /**< invalid gpio pin */

    switch(pHandle->device_variant)
    {

        case PCF8575_VARIANT:
        {
            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 2);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }
            formated_data = (status[1] << 8)|(status[0]);   /**< format data read in reverse */

            if(pin > PCF857X_GPIO_PIN_7)
            {
                pin = pin - 2;
                formated_data &= ~(1 << pin);                   /**< clear bit */
                formated_data |= (logic_level << pin);          /**< write logic level */

                status[0] = (formated_data & 0x00ff);           /**< split the two byte and tore in array */
                status[1] = (formated_data & 0xff00) >> 8;

                err = a_pcf857x_i2c_write(pHandle, NULL, (uint8_t *)status, 2);    /**< write logic level to slave device*/
                if(err)
                {
                    a_pcf857x_print_error_msg(pHandle, "write port register");
                    return 1;
                }
            }else{
                formated_data &= ~(1 << pin);                   /**< clear bit */
                formated_data |= (logic_level << pin);          /**< load logic level into variable */

                err = a_pcf857x_i2c_write(pHandle, NULL, (uint8_t *)&formated_data, 2);    /**< write logic level to slave device*/
                if(err)
                {
                    a_pcf857x_print_error_msg(pHandle, "write port register");
                    return 1;
                }
            }

            break;
        }

        case PCF8574A_VARIANT:
        {
            if(pin > PCF857X_GPIO_PIN_7){

                a_pcf857x_print_error_msg(pHandle, "to write, invalid gpio pin");
                return 4;
            }
            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 1);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }
            formated_data = status[0];
            formated_data &= ~(1 << pin);                   /**< clear bit */
            formated_data |= (logic_level << pin);          /**< write logic level */

            err = a_pcf857x_i2c_write(pHandle, NULL, (uint8_t *)&formated_data, 1);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "write port register");
                return 1;
            }

            break;
        }

        case PCF8574_VARIANT:
        {
             if(pin > PCF857X_GPIO_PIN_7){

                a_pcf857x_print_error_msg(pHandle, "to write pin, invalid gpio");
                return 4;
            }
            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 1);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }
            formated_data = status[0];
            formated_data &= ~(1 << pin);                   /**< clear bit */
            formated_data |= (logic_level << pin);          /**< write logic level */

            err = a_pcf857x_i2c_write(pHandle, NULL, (uint8_t *)&formated_data, 1);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "write port register");
                return 1;
            }

            break;
        }

        default:
            return 1;
        break;

    }

    return 0;        /**<  success */
}

/**
 * @brief Write logic level to all gpio pins
 * @param[in] *handle point to PCF857X handle structure.
 * @return status code
 *          - 0 success
 *          - 1 failed to write pin
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 * @note    none
 */
uint8_t pcf857x_pin_write_all(pcf857x_handle_t *const pHandle, pcf857x_port_logic_level_t logic_level)
{
    uint8_t gpio_index;
    uint8_t status[2];

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

   switch(pHandle->device_variant)
    {

        case PCF8575_VARIANT:
        {
            if(logic_level == HIGH){status[0] = 0xff; status[1] = 0xff;}
            else{status[0] = 0x00; status[1] = 0x00; }
            err = a_pcf857x_i2c_write(pHandle, NULL, (uint8_t *)status, 2);
            if (err != PCF857X_DRV_OK){
                return err;
            }
            break;
        }
        case PCF8574A_VARIANT:
        {
          if(logic_level == HIGH){logic_level = 0xff;}
          err = a_pcf857x_i2c_write(pHandle, NULL, (uint8_t *)&logic_level, 1);
          if (err != PCF857X_DRV_OK){
            return err;
          }
            break;
        }

        case PCF8574_VARIANT:
        {
          if(logic_level == HIGH){logic_level = 0xff;}
          err = a_pcf857x_i2c_write(pHandle, NULL, (uint8_t *)&logic_level, 1);
          if (err != PCF857X_DRV_OK){
            return err;
          }
            break;
        }

        default:
            return 1;
        break;

    }

    return 0;       /**<  success */
}

/**
 * @brief Read logic level of gpio pin
 * @param[in] *handle point to PCF857X handle structure
 * @param[in] pin is the gpio pin to read (GP0 - GP17)
 * @param[out] *logic_level point to the logic level assigned to the pin
 * @return status code
 *          - 0 success
 *          - 1 failed read pin
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 *          - 4 invalid gpio pin
 * @note    none
 */
uint8_t pcf857x_pin_read(pcf857x_handle_t *const pHandle, pcf857x_gpio_port_t pin, pcf857x_port_logic_level_t *logic_level)
{
    uint8_t status[2];
    uint16_t formated_data;

    memset(status, 0, 2);                    /**< clear buffer before write */

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */
    if(pin > PCF857X_GPIO_PIN_17 || pin < PCF857X_GPIO_PIN_0)
        return 4;      /**< invalid gpio pin */

    switch(pHandle->device_variant)
    {

        case PCF8575_VARIANT:
        {
            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 2);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }
            formated_data = (status[1] << 8)|(status[0]);   /**< format data read in reverse */

            if(pin > PCF857X_GPIO_PIN_7)
            {
                pin = pin - 2;
            }
            *logic_level = (pcf857x_port_logic_level_t)((formated_data & pcf857x_pin_mask_arr[pin]) >> pin);

            break;
        }

        case PCF8574A_VARIANT:
        {

            if(pin > PCF857X_GPIO_PIN_7){

                a_pcf857x_print_error_msg(pHandle, "to read pin, invalid gpio");
                return 4;
            }

            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 1);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }

            formated_data = status[0];
            *logic_level = (pcf857x_port_logic_level_t)((formated_data & pcf857x_pin_mask_arr[pin]) >> pin);

            break;
        }

        case PCF8574_VARIANT:
        {
            if(pin > PCF857X_GPIO_PIN_7){

                a_pcf857x_print_error_msg(pHandle, "to read pin, invalid gpio");
                return 4;
            }

            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 1);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }

            formated_data = status[0];
            *logic_level = (pcf857x_port_logic_level_t)((formated_data & pcf857x_pin_mask_arr[pin]) >> pin);

            break;
        }

        default:
            return 1;
        break;

    }

    return 0;       /**<  success */
}

/**
 * @brief Read logic level of all gpio pins
 * @param[in] *handle point to PCF857X handle structure
 * @param[out] *logic_level point to the logic level assigned to the pin
 * @return status code
 *          - 0 success
 *          - 1 failed read pin
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 * @note    none
 */
uint8_t pcf857x_pin_read_all(pcf857x_handle_t *const pHandle, uint16_t *logic_level)
{
    uint16_t status[2], formated_data;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    switch(pHandle->device_variant)
    {

        case PCF8575_VARIANT:
        {
            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 2);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }
            formated_data = (status[0] << 8)|(status[1]);   /**< format data read*/
            *logic_level = formated_data;
            break;
        }

        case PCF8574A_VARIANT:
        {
            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 1);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }

            *logic_level = status[0];
            break;
        }

        case PCF8574_VARIANT:
        {
            err = a_pcf857x_i2c_read(pHandle, NULL, (uint8_t *)status, 1);
            if(err)
            {
                a_pcf857x_print_error_msg(pHandle, "read port register");
                return 1;
            }

            *logic_level = status[0];
            break;
        }

        default:
            return 1;
        break;

    }

    return 0;       /**<  success */
}

/**
 * @brief This function toggles gpio pin logic level
 * @param[in] *handle point to pcf857x handle structure
 * @param[in] pin is the gpio pin to read (GP0 - GP17)
 * @return status code
 *          - 0 success
 *          - 1 failed read pin
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 *          - 4 invalid gpio pin
 * @note    none
 */
uint8_t pcf857x_pin_toggle(pcf857x_handle_t *const pHandle, pcf857x_gpio_port_t pin)
{
    uint8_t logic_level;

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = pcf857x_pin_read(pHandle, pin, &logic_level);
    if(err)
    {
        a_pcf857x_print_error_msg(pHandle, "toggle pin");
        return 1;
    }

    err =  pcf857x_pin_write(pHandle, pin, !logic_level);
    if(err)
    {
        a_pcf857x_print_error_msg(pHandle, "toggle pin");
        return 1;
    }
    return 0;
}

/**
 * @brief     set the chip register
 * @param[in] pHandle points to a PCF857X handle structure
 * @param[in] reg is the i2c register address
 * @param[in] *buf points to a data buffer
 * @param[in] u8Len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf857x_set_reg(pcf857x_handle_t *const pHandle, uint8_t u8Reg, uint8_t *buf, size_t u8Len)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = a_pcf857x_i2c_write(pHandle, u8Reg, (uint8_t *)buf, u8Len);
    if(err)
    {
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief      get the chip register
 * @param[in]  pHandle points to a PCF857X handle structure
 * @param[in]  reg is the i2c register address
 * @param[out] *buf points to a data buffer
 * @param[in]  u8Len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t pcf857x_get_reg(pcf857x_handle_t *const pHandle, uint8_t u8Reg, uint8_t *buf, size_t u8Len)
{

    if(pHandle == NULL)
        return 2;     /**< return failed error */
    if(pHandle->inited != 1)
        return 3;      /**< return failed error */

    err = a_pcf857x_i2c_read(pHandle, u8Reg, (uint8_t *)buf, u8Len);
    if(err)
    {
        return 1;           /**< failed error */
    }

    return 0;              /**< success */
}

/**
 * @brief      get chip's information
 * @param[in]  pHandle points to PCF857X structure
 * @return  status code
 *             - 0 success
 *             - 2 pHandle is NULL
 * @note       none
 */
uint8_t pcf857x_info(pcf857x_handle_t *const pHandle)
{
    strncpy(pHandle->info.chip_name, CHIP_NAME, 25);                        /**< copy chip name */
    strncpy(pHandle->info.interface, INTERFACE, 8);                         /**< copy interface name */
    strncpy(pHandle->info.manufacturer_name, MANUFACTURER_NAME, 25);        /**< copy manufacturer name */
    pHandle->info.supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /**< set minimal supply voltage */
    pHandle->info.supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /**< set maximum supply voltage */
    pHandle->info.max_current_ma = MAX_CURRENT;                             /**< set maximum current */
    pHandle->info.temperature_max = TEMPERATURE_MAX;                        /**< set minimal temperature */
    pHandle->info.temperature_min = TEMPERATURE_MIN;                        /**< set maximum temperature */
    pHandle->info.flash_size_min = MCU_FLASH_MIN;                           /**< set the Micro-controller minimum recommended flash size */
    pHandle->info.ram_size_min = MCU_RAM_MIN;                               /**< set the Micro-controller minimum recommended ram size */
    pHandle->info.driver_version = DRIVER_VERSION;                          /**< set driver version */

    return 0;                                                               /**< success */
}
