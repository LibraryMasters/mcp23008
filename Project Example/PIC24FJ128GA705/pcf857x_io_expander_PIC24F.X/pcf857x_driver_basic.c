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
 * File:   pcf857x_driver_basic.c
 * Author: Cedric Akilimali
 *
 * Created on November 24, 2023, 09:35 AM
 */

 #include "pcf857x_driver_basic.h"

 /**
 * @brief basic example initialize
 * @param[in] variant is the device type
 * @param[in] addr_pin is the i2c address pin config (A1, A2, A3)
 * @return status code
 *          - 0 success
 *          - 1 initialize failed
 * @note    none
 */

uint8_t pcf857x_basic_initialize(pcf857x_variant_t variant, pcf857x_address_t addr_pin)
{

    /*link function*/
    DRIVER_PCF857X_LINK_INIT(&pcf857x_handler, pcf857x_handle_t);                               /**< Link the  */
    DRIVER_PCF857X_LINK_I2C_INIT(&pcf857x_handler, pcf857x_interface_i2c_init);                 /**< Link the i2c initialize function */
    DRIVER_PCF857X_LINK_I2C_DEINIT(&pcf857x_handler, pcf857x_interface_i2c_deinit);             /**< Link the the i2c de-initialize function */
    DRIVER_PCF857X_LINK_I2C_WRITE(&pcf857x_handler, pcf857x_interface_i2c_write);               /**< Link the i2c  write function */
    DRIVER_PCF857X_LINK_I2C_READ(&pcf857x_handler, pcf857x_interface_i2c_read);                 /**< Link the i2c read function */
    DRIVER_PCF857X_LINK_DELAY_MS(&pcf857x_handler,pcf857x_interface_delay_ms);                  /**< Link delay function */
    DRIVER_PCF857X_LINK_DEBUG_PRINT(&pcf857x_handler, pcf857x_interface_debug_print);           /**< Link the the debug print function */

    /* PCF857X initialize */
    err = pcf857x_init(&pcf857x_handler);
    if(err)
    {
       pcf857x_interface_debug_print("initialize");
        return err;       /**< failed */
    }

    /* set device variant */
    err = pcf857x_set_variant(&pcf857x_handler, variant);
    if(err)
    {
        pcf857x_interface_debug_print("set device variant");
        return err;   /**< failed */
    }

    /* set i2c address mask */
    err = pcf857x_set_addr_pin(&pcf857x_handler, addr_pin);
    if(err)
    {
        pcf857x_interface_debug_print("set i2c address");
        return err;
    }

    pcf857x_interface_delay_ms(10);         /**< wait 10 ms after initialize complete */

    return 0;   /**< Initialize success */
}

/**
 * @brief     This function get the device variant
 * @param[out] pVariant point to the device variant
 * @return  status code
 *            - 0 success
 *            - 1 failed to get device variant
 * @note      none
 */
uint8_t pcf857x_basic_get_variant(pcf857x_variant_t *pVariant)
{
   err = pcf857x_get_variant(&pcf857x_handler, pVariant);
   return err;           /**< success */
}

/**
 * @brief    Basic implementation to get the address
 * @param[out] pI2c_address point to the chip i2c address
 * @return  status code
 *            - 0 success
 *            - 1 failed to get i2c address
 * @note      none
 */
uint8_t pcf857x_basic_get_addr(uint8_t  *pI2c_address)
{
   err = pcf857x_get_addr_pin(&pcf857x_handler, pI2c_address);
   return err;           /**< success */
}

/**
 * @brief basic example to write logic value to gpio port
 * @param[in] GPIOx is the gpio port to write
 * @param[in] level is the logic level to write (HIGH or LOW)
 * @return status code
 *          - 0 success
 *          - 1 failed
 * @note    none
 */
uint8_t pcf857x_basic_gpio_write(pcf857x_gpio_port_t GPIOx, pcf857x_gpio_port_t level)
{
   err = pcf857x_pin_write(&pcf857x_handler, GPIOx, level);             /**< write gpio pin */
   return err;           /**< success */

}

/**
 * @brief basic example to write logic value to all gpio ports
 * @param[in] level is the logic level to write (HIGH or LOW)
 * @return status code
 *          - 0 success
 *          - 1 failed
 * @note    none
 */
uint8_t pcf857x_basic_gpio_write_all(pcf857x_gpio_port_t level)
{
   err = pcf857x_pin_write_all(&pcf857x_handler, level);             /**< write gpio pin */
   return err;           /**< success */
}

/**
* @brief basic example to read gpio port
* @param[in] GPIOx is the gpio port to read
* @return GPIO read status
* @note    none
*/
uint8_t pcf857x_basic_gpio_read(pcf857x_gpio_port_t GPIOx)
{
   uint8_t pin_level;

   err = pcf857x_pin_write(&pcf857x_handler, GPIOx, HIGH);           /**< first write before read */
   if(err)
   {

   }

   pcf857x_pin_read(&pcf857x_handler, GPIOx, &pin_level);             /**< write gpio pin */
   return pin_level;       /**< success */
}

/**
* @brief basic example to read all gpio ports
* @param[in] GPIOx is the gpio port to read
* @return GPIO read status
* @note    none
*/
uint16_t pcf857x_basic_gpio_read_all(void)
{
   uint16_t pin_level;

   err = pcf857x_pin_read_all(&pcf857x_handler, (uint16_t *)&pin_level);             /**< write gpio pin */
   return pin_level;       /**< success */
}



 /**
 * @brief basic example of gpio toggle function
 * @param[in] GPIOx is the port to toggle
 * @return status code
 *          - 0 success
 *          - 1 failed to toggle
 * @note    none
 */
uint8_t pcf857x_basic_gpio_toggle(pcf857x_gpio_port_t GPIOx)
{
    err = pcf857x_pin_toggle(&pcf857x_handler, GPIOx);
    return err;       /**< success */
}

/**
 * @brief basic example write register
 * @param[in] reg is the i2c register address
 * @param[in] *pBuf points to a data buffer to write
 * @param[in] u8Length is the data buffer length
 * @return status code
 *          - 0 success
 *          - 1 failed to write register
 */
uint8_t pcf857x_basic_write_register(uint8_t u8Reg, uint8_t *pBuf, uint8_t u8Length)
{
  err = pcf857x_set_reg(&pcf857x_handler, u8Reg, (uint8_t *)pBuf, u8Length);
  return err;           /**< success */
}

/**
* @brief This function reads the device registers
* @param[in] reg is the i2c register address
* @param[out] *pBuf points to a data buffer
* @param[in] u8Length is the data buffer length
* @return status code
*           - 0 success
*           - 1 failed
* @note none
*/
uint8_t pcf857x_basic_read_reg(uint8_t u8Reg, uint8_t *pBuf, uint8_t u8Length)
{
    err = pcf857x_get_reg(&pcf857x_handler, u8Reg, (uint8_t *)pBuf, u8Length);
    return err;           /**< success */
}
