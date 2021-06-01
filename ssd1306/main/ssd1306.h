// Driver Interface for SSD1306 OLED 128 x 64 Display

#include "esp_log.h"
#include "driver/i2c.h"


/**
 * @brief wrapper to assemble address byte for ssd1306
 *  @note SA0 is managed in config
 * 
 * @param cmd an i2c command queue handle
 * 
 * @param read_write always write for i2c interface
 */
static esp_err_t ssd1306_send_address_byte(i2c_cmd_handle_t cmd, uint8_t read_write);


/**
 * @brief Send a control byte to ssd1306
 *
 * 
 * @param cmd an i2c command queue handle
 * 
 * @param Co If the Co bit is set as logic “0”, the transmission of the following information will contain 
 *           data bytes only
 * @param data_command The D/C# bit determines the next data byte is acted as a command or a data. If the D/C# bit is 
 *           set to logic “0”, it defines the following data byte as a command. If the D/C# bit is set to 
 *           logic “1”, it defines the following data byte as a data which will be stored at the GDDRAM. 
 *           The GDDRAM column address pointer will be increased by one automatically after each 
 *           data write. 
 */
static esp_err_t ssd1306_send_control_byte(i2c_cmd_handle_t cmd, uint8_t Co, uint8_t data_command);





//! -------------- Higher level functions ---------------------

/**
 * @brief set display contrast 
 *
 * @param contrast a value from 1 to 256, where contrast increases as the value increases
 */
static esp_err_t ssd1306_set_contrast(i2c_port_t i2c_num, uint8_t contrast);


/**
 * @brief Turn all pixels on, or show RAM contents
 *
 * @param  pixels_on if on=0 display RAM contents on screen, if on=1 ignore RAM and entire display turns on.
 */
static esp_err_t ssd1306_entire_display_on(i2c_port_t i2c_num, uint8_t pixels_on);


/**
 * @brief  Set display mode to normal or inverse
 *
 * @param  inverse inverse=1 inverts display of RAM contents so that 1 in RAM --> OFF in display.
 *                  inverse=0 resets display to normal, so 0 in RAM --> OFF in display.
 */
static esp_err_t ssd1306_invert_display(i2c_port_t i2c_num, uint8_t inverse);


/**
 * @brief  Turn display on or off
 *
 * @param  display_on Set to 1 to turn display on or set to 0 to put display to sleep.
 */
static esp_err_t ssd1306_turn_display_on_off(i2c_port_t i2c_num, uint8_t display_on);


/**
 * @brief  
 *
 * @param  
 */
// static esp_err_t ssd1306_(uint8_t );