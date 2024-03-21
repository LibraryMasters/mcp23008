#include <atmel_start.h>
#include "mcp23008_driver/mcp23008_driver_basic.h"

void print(char *const pBuffer, size_t u8Length);
uint8_t i2c_read(uint8_t addr, uint8_t *buf, uint16_t len);
uint8_t i2c_write(uint8_t addr, uint8_t *buf, uint16_t len);
void gpio_toggle_pin_blue(void);
void gpio_toggle_pin_green(void);

int variant;

int main(void)
{
	/* Initializes MCU, drivers and middle-ware */
	atmel_start_init();
	
	 gpio_set_pin_level(mcp23008_reset_pin, true);  /**< make the reset pin on the slave device is constantly high during communication */
	
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

	 //mcp23008_basic_gpio_set_direction(MCP23008_GPIO_PIN_7, MCP23008_OUTPUT);
	 mcp23008_basic_gpio_set_direction(MCP23008_GPIO_PIN_6, MCP23008_OUTPUT);
	 //mcp23008_basic_gpio_set_direction(MCP23008_GPIO_PIN_5, MCP23008_OUTPUT);
	 
	 /**< enable pin mode GPIO0 and 1 as input pull-up*/
	 mcp23008_basic_gpio_set_direction(MCP23008_GPIO_PIN_0, MCP23008_INPUT_PULLUP);
	 mcp23008_basic_gpio_set_direction(MCP23008_GPIO_PIN_1, MCP23008_INPUT_PULLUP);
	 
	 /**< enable interrupt on GPIO 0 and 1 on falling edge*/
	 mcp23008_basic_INT_enable(MCP23008_GPIO_PIN_0, MCP23008_INT_FALLING_EDGE);
	 mcp23008_basic_INT_enable(MCP23008_GPIO_PIN_1, MCP23008_INT_FALLING_EDGE);
	 
	 ext_irq_register(PIN_PA24, mcp23008_basic_irq_handler);
	 //mcp23008_basic_port_set_direction(MCP23008_OUTPUT);
	  
  
	/* Replace with your application code */
	while (1) {
		mcp23008_interface_delay_ms(1);
		mcp23008_basic_clr_INT_flag();

	 //gpio_toggle_pin_level(user_led_blue);
	 //mcp23008_basic_gpio_toggle(MCP23008_GPIO_PIN_7);
	 //mcp23008_basic_gpio_toggle(MCP23008_GPIO_PIN_6);
	 //mcp23008_basic_gpio_toggle(MCP23008_GPIO_PIN_5);
	 //mcp23008_basic_pin_write_all(LOW);
	 //mcp23008_interface_delay_ms(1000);
	 //mcp23008_basic_pin_write_all(HIGH);
	 //mcp23008_interface_delay_ms(300);
	 //mcp23008_basic_gpio_write(MCP23008_GPIO_PIN_7, MCP23008_GPIO_LOW);
	 //mcp23008_interface_delay_ms(300);
	 	 //if(!mcp23008_basic_gpio_read(MCP23008_GPIO_PIN_1) ||!mcp23008_basic_gpio_read(MCP23008_GPIO_PIN_0)){
		 	 //mcp23008_interface_delay_ms(1);
		 	 //if(!mcp23008_basic_gpio_read(MCP23008_GPIO_PIN_1) ||!mcp23008_basic_gpio_read(MCP23008_GPIO_PIN_0)  )
		 	 //mcp23008_basic_gpio_toggle(MCP23008_GPIO_PIN_5);
	 	 //}
	}
}

void print(char *const pBuffer, size_t u8Length)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);

	io_write(io, (char *)pBuffer, u8Length);
	
}

uint8_t i2c_read(uint8_t addr, uint8_t *buf, uint16_t len)
{
	struct io_descriptor *I2C_0_io;

	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_0_io);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, addr, I2C_M_SEVEN);
	io_read(I2C_0_io, buf,  len);
	return 0;
}

uint8_t i2c_write(uint8_t addr, uint8_t *buf, uint16_t len)
{
	struct io_descriptor *I2C_0_io;
	i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_0_io);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, addr, I2C_M_SEVEN);
	io_write(I2C_0_io, (uint8_t *)buf, len);
	return 0;
}

void gpio_toggle_pin_green(void)
{
	gpio_toggle_pin_level(user_led_green);
}

void gpio_toggle_pin_blue(void)
{
	gpio_toggle_pin_level(user_led_blue);
}