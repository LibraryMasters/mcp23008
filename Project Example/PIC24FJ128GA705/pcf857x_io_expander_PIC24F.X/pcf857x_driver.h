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
 * File:   pcf857x_driver.h
 * Author: Cedric Akilimali
 *
 * Created on November 23, 2023, 14:22 PM
 */

#ifndef PCF857X_DRIVER_H_INCLUDED
#define PCF857X_DRIVER_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define PCF857X_DEBUG_MODE             /**< enables/disabled all printout on the code*/

/**
 * @defgroup driver_pcf857x PCF857X driver function
 * @brief    pcf857x driver modules
 * @{
 */

/**
 * @addtogroup pcf857x_base_driver
 * @{
 */

 /* GPIO Pin Mask */

#define PCF857X_GP0_MASK                           0x01                  /**< GPIO pin 0 Mask */
#define PCF857X_GP1_MASK                           0x02                  /**< GPIO pin 1 Mask */
#define PCF857X_GP2_MASK                           0x04                  /**< GPIO pin 2 Mask */
#define PCF857X_GP3_MASK                           0x08                  /**< GPIO pin 3 Mask */
#define PCF857X_GP4_MASK                           0x10                  /**< GPIO pin 4 Mask */
#define PCF857X_GP5_MASK                           0x20                  /**< GPIO pin 5 Mask */
#define PCF857X_GP6_MASK                           0x40                  /**< GPIO pin 6 Mask */
#define PCF857X_GP7_MASK                           0x80                  /**< GPIO pin 7 Mask */

#define PCF8575_GP10_MASK                          0x0100                /**< GPIO pin 10 Mask */
#define PCF8575_GP11_MASK                          0x0200                /**< GPIO pin 11 Mask */
#define PCF8575_GP12_MASK                          0x0400                /**< GPIO pin 12 Mask */
#define PCF8575_GP13_MASK                          0x0800                /**< GPIO pin 13 Mask */
#define PCF8575_GP14_MASK                          0x1000                /**< GPIO pin 14 Mask */
#define PCF8575_GP15_MASK                          0x2000                /**< GPIO pin 15 Mask */
#define PCF8575_GP16_MASK                          0x4000                /**< GPIO pin 16 Mask */
#define PCF8575_GP17_MASK                          0x8000                /**< GPIO pin 17 Mask */

#define PCF8574_MAX_NUM_GPIO_PIN                   0x08                  /**< 8 max number of gpio pin for the pcf874/4A variant */
#define PCF8575_MAX_NUM_GPIO_PIN                   0x10                  /**< 16 max number of gpio pin count for the PCF8575 variant */

#define LOW                                        0x00                  /**< low logic level definition */
#define HIGH                                       0x01                  /**< high logic level definition */


 /**> Global Variable definition **/

static uint8_t err;                                                    /**< driver error status read variable */

 static uint16_t pcf857x_pin_mask_arr[PCF8575_MAX_NUM_GPIO_PIN] = { PCF857X_GP0_MASK,
                                                                    PCF857X_GP1_MASK,
                                                                    PCF857X_GP2_MASK,
                                                                    PCF857X_GP3_MASK,
                                                                    PCF857X_GP4_MASK,
                                                                    PCF857X_GP5_MASK,
                                                                    PCF857X_GP6_MASK,
                                                                    PCF857X_GP7_MASK,
                                                                    PCF8575_GP10_MASK,
                                                                    PCF8575_GP11_MASK,
                                                                    PCF8575_GP12_MASK,
                                                                    PCF8575_GP13_MASK,
                                                                    PCF8575_GP14_MASK,
                                                                    PCF8575_GP15_MASK,
                                                                    PCF8575_GP16_MASK,
                                                                    PCF8575_GP17_MASK};

 /***> End of global variable definition **/

 /**
* @brief PCF857X execution status enumeration
*/
typedef enum
{
    PCF857X_DRV_OK          = 0x00,                                      /**< status execute success */
    PCF857X_DRV_FAILED      = 0x01,                                      /**< status execute failed */
    PCF857X_DRV_ERR_HANDLER = 0x02,                                      /**< status execute failed, handle is null */
    PCF857X_DRV_ERR_INIT    = 0x03                                       /**< status execute failed, handle not initialize */
} pcf857x_driver_execute_stat_t;


/**
* @brief PCF857X variant type enumeration
*/
typedef enum
{
    PCF8574_VARIANT   = 0x00,                                             /**< pcf8574 variant type */
    PCF8574A_VARIANT  = 0x01,                                             /**< pcf8574a variant type */
    PCF8575_VARIANT   = 0x02,                                             /**< PCF8575 variant type */
}pcf857x_variant_t;

/**
 *@brief PCF857X address enumeration definition
 */
typedef enum
{
    PCF857X_ADDRESS_DEFAULT = 0x20,                                        /**< pcf874/5 7 bit default slave address*/
    PCF8574A_ADDRESS_DEFAULT = 0x38,                                       /**< pcf874A 7 bit default slave address*/
    PCF857X_ADDRESS_A000 = 0x00,                                           /**< A2_A1_A0 000 */
    PCF857X_ADDRESS_A001 = 0x01,                                           /**< A2_A1_A0 001 */
    PCF857X_ADDRESS_A010 = 0x02,                                           /**< A2_A1_A0 010 */
    PCF857X_ADDRESS_A011 = 0x03,                                           /**< A2_A1_A0 011 */
    PCF857X_ADDRESS_A100 = 0x04,                                           /**< A2_A1_A0 100 */
    PCF857X_ADDRESS_A101 = 0x05,                                           /**< A2_A1_A0 101 */
    PCF857X_ADDRESS_A110 = 0x06,                                           /**< A2_A1_A0 110 */
    PCF857X_ADDRESS_A111 = 0x07                                            /**< A2_A1_A0 111 */
}pcf857x_address_t;

 /**
 * @brief PCF857X boolean enumeration
 */
typedef enum
{
    PCF857X_BOOL_FALSE  = 0x00,                                              /**< boolean state false */
    PCF857X_BOOL_TRUE   = 0x01                                               /**< boolean state true  */
}pcf857x_bool_t;

/**
 * @brief pcf857x gpio port enumeration
 */
typedef enum {
    PCF857X_GPIO_PIN_0 = 0x00,                                               /**< gpio port GP0 */
    PCF857X_GPIO_PIN_1 = 0x01,                                               /**< gpio port GP1 */
    PCF857X_GPIO_PIN_2 = 0x02,                                               /**< gpio port GP2 */
    PCF857X_GPIO_PIN_3 = 0x03,                                               /**< gpio port GP3 */
    PCF857X_GPIO_PIN_4 = 0x04,                                               /**< gpio port GP4 */
    PCF857X_GPIO_PIN_5 = 0x05,                                               /**< gpio port GP5 */
    PCF857X_GPIO_PIN_6 = 0x06,                                               /**< gpio port GP6 */
    PCF857X_GPIO_PIN_7 = 0x07,                                               /**< gpio port GP7 */

    PCF857X_GPIO_PIN_10 = 0x0A,                                              /**< gpio port GP10 */
    PCF857X_GPIO_PIN_11 = 0x0B,                                              /**< gpio port GP11 */
    PCF857X_GPIO_PIN_12 = 0x0C,                                              /**< gpio port GP12 */
    PCF857X_GPIO_PIN_13 = 0x0D,                                              /**< gpio port GP13 */
    PCF857X_GPIO_PIN_14 = 0x0E,                                              /**< gpio port GP14 */
    PCF857X_GPIO_PIN_15 = 0x0F,                                              /**< gpio port GP15 */
    PCF857X_GPIO_PIN_16 = 0x10,                                              /**< gpio port GP16 */
    PCF857X_GPIO_PIN_17 = 0x11                                               /**< gpio port GP17 */
} pcf857x_gpio_port_t;

/**
 * @brief pcf857x logic level enumeration
 */
typedef enum {
    PCF857X_GPIO_LOW  = 0x00,                                               /**< GPIO port logic level low */
    PCF857X_GPIO_HIGH = 0x01                                                /**< GPIO port logic level high */
} pcf857x_port_logic_level_t;

 /**
 * @brief PCF857X information structure definition
 */
typedef struct pcf857x_info_s
{
    char chip_name[25];                                                 /**< chip name */
    char manufacturer_name[25];                                         /**< manufacturer name */
    char interface[8];                                                  /**< chip interface name */
    float supply_voltage_min_v;                                         /**< chip min supply voltage */
    float supply_voltage_max_v;                                         /**< chip max supply voltage */
    float max_current_ma;                                               /**< chip max current */
    float temperature_min;                                              /**< chip min operating temperature */
    float temperature_max;                                              /**< chip max operating temperature */
    uint8_t flash_size_min;                                             /**< Micro-controller minimum recommended flash size */
    uint8_t ram_size_min;                                               /**< Micro-controller minimum recommended flash size */
    float driver_version;                                               /**< driver version */
} pcf857x_info_t;

/**
* @brief pcf857x handle enumeration
*/
typedef struct pcf857x_handle_s
{
    uint8_t (*i2c_init)(void);                                                                  /**< point to a i2c init function address */
    uint8_t (*i2c_deinit)(void);                                                                /**< point to a i2c deinit function address */
    uint8_t (*i2c_read)(uint8_t u8Addr, uint8_t u8Reg, uint8_t *pBuf,  uint8_t u8Length);       /**< point to a i2c read function address */
    uint8_t (*i2c_write)(uint8_t u8Addr, uint8_t u8Reg, uint8_t *pBuf,  uint8_t u8Length);      /**< point to a i2c write function address */
    void (*delay_ms)(uint32_t u32Ms);                                                           /**< point to a delay_ms function address */
    void(*debug_print)(char *fmt, ...);                                                         /**< point to a debug_print function address */
    pcf857x_info_t info;
    uint8_t i2c_address;                                                                        /**< i2c device address */
    uint8_t device_variant;                                                                     /**< Device variant */
    uint8_t gpio_pin_count;                                                                      /**< variant number gpio count */
    uint8_t inited;                                                                             /**< Driver initialize status bit */
} pcf857x_handle_t;

 /**
 * @}
 */

 /**
* @defgroup pcf857x pcf857x link driver function
* @brief    pcf857x link driver modules
* @ingroup  pcf857x driver
* @{
*/

/**
 * @brief     initialize pcf857x_handle_t structure
 * @param[in] pHandle points to pcf857x pHandle structure
 * @param[in] STRUCTURE is pcf857x_handle_t
 * @note      none
 */
#define DRIVER_PCF857X_LINK_INIT(pHandle, STRUCTURE)           memset(pHandle, 0, sizeof(STRUCTURE))

/**
 * @brief     link i2c_init function
 * @param[in] pHandle points to pcf857x pHandle structure
 * @param[in] FUC points to a i2c_init function address
 * @note      none
 */
#define DRIVER_PCF857X_LINK_I2C_INIT(pHandle, FUC)              (pHandle)->i2c_init = FUC


/**
 * @brief     link i2c_deinit function
 * @param[in] pHandle points to pcf857x pHandle structure
 * @param[in] FUC points to a i2c_deinit function address
 * @note      none
 */
#define DRIVER_PCF857X_LINK_I2C_DEINIT(pHandle, FUC)            (pHandle)->i2c_deinit = FUC


/**
 * @brief     link i2c_write function
 * @param[in] pHandle points to pcf857x pHandle structure
 * @param[in] FUC points to a i2c_write function address
 * @note      none
 */
#define DRIVER_PCF857X_LINK_I2C_WRITE(pHandle, FUC)             (pHandle)->i2c_write = FUC


/**
 * @brief     link i2c_read function
 * @param[in] pHandle points to pcf857x pHandle structure
 * @param[in] FUC points to a i2c_read function address
 * @note      none
 */
#define DRIVER_PCF857X_LINK_I2C_READ(pHandle, FUC)              (pHandle)->i2c_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] pHandle points to pcf857x pHandle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_PCF857X_LINK_DELAY_MS(pHandle, FUC)             (pHandle)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] pHandle points to pcf857x pHandle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_PCF857X_LINK_DEBUG_PRINT(pHandle, FUC)          (pHandle)->debug_print = FUC

/**
 * @}
 */

 /**
* @defgroup pcf857x_base_driver pcf857x base driver function
* @brief    pcf857x base driver modules
* @ingroup  pcf857x_driver
* @{
*/

/**
 * @brief      get chip's information
 * @param[in] pcf857x_handle_t *const pHandle
 * @return  status code
 *             - 0 success
 *             - 2 pHandle is NULL
 * @note       none
 */
uint8_t pcf857x_info(pcf857x_handle_t *const pHandle);

/**
 * @brief     This function initialize the chip
 * @param[in] pHandle points to pcf857x pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 i2c initialization failed
 *            - 2 pHandle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t pcf857x_init(pcf857x_handle_t *const pHandle);

/**
 * @brief     This function close the chip
 * @param[in] pHandle points to pcf857x pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 i2c deinit failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t pcf857x_deinit(pcf857x_handle_t *const pHandle);

/**
 * @brief     This function execute irq pHandler
 * @param[in] pHandle points to pcf857x pHandle structure
 * @return  status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t pcf857x_irq_pHandler(pcf857x_handle_t *const pHandle);

/**
 * @brief     This function set the address pin
 * @param[in] pHandle points to a pcf857x pHandle structure
 * @param[in] addr_pin is the i2c addresses pins
 * @return  status code
 *            - 0 success
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t pcf857x_set_addr_pin(pcf857x_handle_t *const pHandle, pcf857x_address_t addr_pin);

/**
 * @brief     This function set the address pin
 * @param[in] pHandle points to a pcf857x pHandle structure
 * @param[out] addr_pin point to both i2c addresses pins
 * @return  status code
 *            - 0 success
 *            - 2 pHandle is NULL
 *            - 3 pHandle is not initialized
 * @note      none
 */
uint8_t pcf857x_get_addr_pin(pcf857x_handle_t *const pHandle, pcf857x_address_t *pAddr_pin);

/**
 * @brief     This function set the chip variant
 * @param[in] pHandle points to pcf857x handle structure
 * @param[in] variant is the chip variant type
 * @return    status code
 *            - 0 success
 *            - 1 set variant failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 invalid variant
 * @note      none
 */
uint8_t pcf857x_set_variant(pcf857x_handle_t *const pHandle, pcf857x_variant_t variant);

/**
 * @brief     This function get the chip variant
 * @param[in] pHandle points to pcf857x handle structure
 * @param[out] pVariant point to the chip variant type
 * @return    status code
 *            - 0 success
 *            - 1 get variant failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t pcf857x_get_variant(pcf857x_handle_t *const pHandle, pcf857x_variant_t *pVariant);

/**
 * @brief Write logic level to gpio pin
 * @param[in] *handle point to pcf857x handle structure
 * @param[in] pin is the gpio pin to write (GP0 - GP17)
 * @param[in] logic_level is the logic level to assign to the pin
 * @return status code
 *          - 0 success
 *          - 1 failed read pin
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 *          - 4 invalid gpio pin
 * @note    none
 */
uint8_t pcf857x_pin_write(pcf857x_handle_t *const pHandle, pcf857x_gpio_port_t pin, pcf857x_port_logic_level_t logic_level);

/**
 * @brief Write logic level to all gpio pins
 * @param[in] *handle point to pcf857x handle structure
 * @param[in] logic_level is the logic level to assign to the pins
 * @return status code
 *          - 0 success
 *          - 1 failed read pin
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 *          - 4 invalid gpio pin
 * @note    none
 */
uint8_t pcf857x_pin_write_all(pcf857x_handle_t *const pHandle, pcf857x_port_logic_level_t logic_level);

/**
 * @brief Read logic level of gpio pin
 * @param[in] *handle point to pcf857x handle structure
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
uint8_t pcf857x_pin_read(pcf857x_handle_t *const pHandle, pcf857x_gpio_port_t pin, pcf857x_port_logic_level_t *logic_level);

/**
 * @brief Read logic level of all gpio pins
 * @param[in] *handle point to pcf857x handle structure
 * @param[out] *logic_level point to the logic level assigned to the pin
 * @return status code
 *          - 0 success
 *          - 1 failed read pin
 *          - 2 handle is NULL
 *          - 3 handle is not initialized
 *          - 4 invalid gpio pin
 * @note    none
 */
uint8_t pcf857x_pin_read_all(pcf857x_handle_t *const pHandle, uint16_t *logic_level);

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
uint8_t pcf857x_pin_toggle(pcf857x_handle_t *const pHandle, pcf857x_gpio_port_t pin);

/**
 * @brief     set the chip register
 * @param[in] pHandle points to a pcf857x handle structure
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
uint8_t pcf857x_set_reg(pcf857x_handle_t *const pHandle, uint8_t u8Reg, uint8_t *buf, size_t u8Len);

/**
 * @brief      get the chip register
 * @param[in]  pHandle points to a pcf857x handle structure
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
uint8_t pcf857x_get_reg(pcf857x_handle_t *const pHandle, uint8_t u8Reg, uint8_t *buf, size_t u8Len);


#endif // PCF857X_DRIVER_H_INCLUDED
