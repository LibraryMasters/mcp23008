/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  PIC24FJ128GA705
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/i2c1.h"
#include "pcf857x_driver_basic.h"

I2C1_MESSAGE_STATUS status;

uint8_t i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);
uint8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);

extern volatile uint32_t u32MillisCounter; /**< increment every ms (elapse time since power reset)*/

/**
 * @brief     tick count
 * @return    status code
 *            - system tick in milli seconds
 * @note      none
 */
uint32_t millis(void);

/**
 * @brief     delay function in milli seconds
 * @param[in] msTime is time delay
 * @return    status code
 *            - 0 delay success
 *            - 1 delay failed
 * @note      none
 */
uint8_t delay_ms(uint32_t msTime);

/**
 * @brief application example state machine
 */
typedef enum
{
	READ_WRITE = 0x00,
	TOGGLE     = 0x01,
	WRITE_ALL  = 0x03

}example_state_t;

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    example_state_t  example_state;
	
	err = pcf857x_basic_initialize(PCF8575_VARIANT,PCF857X_ADDRESS_A001);            /**< i2c pin address (A2, A1, A0) */
     if(err != PCF857X_DRV_OK){
         pcf857x_interface_debug_print("initialize failed, error code: %d\r", err);
     }

     /**< READ ME
      * NOTE THAT PCF8575C VARIANT PINS ARE CURRENT SINK, MEANING ONE CAN NOT WRITE A HIGH AS VDD. PLEASE YOUR DATA SHEET FOR DIFFERENCES BETWEEN PCF8575 VS PCF8575C
      * */

     err = pcf857x_info(&pcf857x_handler);

     pcf857x_interface_debug_print("Chip Name: \t%s\r\n", pcf857x_handler.info.chip_name);
     pcf857x_interface_debug_print("Manufacturer: \t%s\r\n",pcf857x_handler.info.manufacturer_name);
     pcf857x_interface_debug_print("Interface Protocol: \t%s\r\n", pcf857x_handler.info.interface);
     pcf857x_interface_debug_print("Supply Volt Max: \t%.1f V\r\n",pcf857x_handler.info.supply_voltage_max_v);
     pcf857x_interface_debug_print("Supply Volt Min: \t%.1f V\r\n",pcf857x_handler.info.supply_voltage_min_v);
     pcf857x_interface_debug_print("Maximum Current: \t%.1f mA\r\n",pcf857x_handler.info.max_current_ma);
     pcf857x_interface_debug_print("Max Temperature: \t%.1f C\r\n",pcf857x_handler.info.temperature_max);
     pcf857x_interface_debug_print("Diver Version: \t\tV%.1f.%.2d\r\n",(pcf857x_handler.info.driver_version /1000), (uint8_t)(pcf857x_handler.info.driver_version - (uint8_t)(pcf857x_handler.info.driver_version / 100)*100));
     pcf857x_interface_debug_print("\nRecommended min MCU flash: \t%dkB\r\n",pcf857x_handler.info.flash_size_min);
     pcf857x_interface_debug_print("Recommended min MCU RAM: \t%dkB\r\n",pcf857x_handler.info.ram_size_min);

     /**< uncomment one of below options to test the driver */
     example_state = READ_WRITE;
//     example_state = TOGGLE;
//     example_state = WRITE_ALL;

    while (1)
    {
        switch(example_state)
		 {

			 case READ_WRITE:
			 {
				 if(pcf857x_basic_gpio_read(PCF857X_GPIO_PIN_0) == LOW){           /**< read pin 0 and de-bounce button */
					 pcf857x_interface_delay_ms(10);
					 if(pcf857x_basic_gpio_read(PCF857X_GPIO_PIN_0) == LOW)
					 {
						 pcf857x_basic_gpio_write(PCF857X_GPIO_PIN_11, HIGH);
					 }

					 }else{
					 pcf857x_basic_gpio_write(PCF857X_GPIO_PIN_11, LOW);
				 }
				 break;
			 }

			 case TOGGLE:
			 {
				 pcf857x_basic_gpio_toggle(PCF857X_GPIO_PIN_7);               /**< toggle pin 7 every 500 ms */
				 pcf857x_interface_delay_ms(500);
				 break;
			 }

			 case WRITE_ALL:
			 {
				 pcf857x_basic_gpio_write_all(HIGH);               /**< Write high on all port */
				 pcf857x_interface_delay_ms(500);
				 pcf857x_basic_gpio_write_all(LOW);                /**< low high on all port */
				 pcf857x_interface_delay_ms(500);
				 break;
			 }

		 }
    }

    return 1;
}

/**
 * @brief i2c data transmit
 * @param[in] addr is the slave address
 * @param[in] reg the register to write
 * @param[in] buf the data to be written
 * @param[in] len size of data
 * @return  status code
 * 			- 0 success
 * 			- 1 failed to write
 * @note	none
 * */
uint8_t i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
     I2C1_MasterWrite((uint8_t *)buf, len, addr, &status);
    while (status == I2C1_MESSAGE_PENDING);

    if (status == I2C1_MESSAGE_FAIL) {
        return 1;
    }

    return 0; /**< success */
}

/**
 * @brief i2c data Read
 * @param[in] addr is the slave address
 * @param[in] reg the register to read
 * @param[out] buf point to data to read
 * @param[in] len size of data
 * @return  status code
 * 			- 0 success
 * 			- 1 failed to read
 * @note	none
 * */

uint8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{

    I2C1_MasterRead((uint8_t *) buf, len, addr, &status);
    while (status == I2C1_MESSAGE_PENDING);

    if (status == I2C1_MESSAGE_FAIL) {
        return 1;
    }

    return 0; /**< success */
}
/**
 End of File
*/

/**
 * @brief     tick count
 * @return    status code
 *            - system tick in milli seconds
 * @note      none
 */
uint32_t millis(void) {
    return u32MillisCounter;
}

/**
 * @brief     delay function in milli seconds
 * @param[in] msTime is time delay
 * @return    status code
 *            - 0 delay success
 *            - 1 delay failed
 * @note      none
 */
uint8_t delay_ms(uint32_t msTime) {

    volatile uint32_t delayCheck = millis();
    while ((millis() - delayCheck) < msTime);

    if (millis() - delayCheck < msTime) {
    }
    return 0;
}