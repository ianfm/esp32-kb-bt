// Driver Interface for SSD1306 OLED 128 x 64 Display

#include "esp_log.h"
#include "driver/i2c.h"


/**
 * @brief wrapper to assemble address byte for ssd1306
 *  @note SA0 is managed in config
 * 
 * @param cmd an i2c command queue handle
 * 
 * @note read_write always write for i2c interface
 */
esp_err_t ssd1306_send_address_byte(i2c_cmd_handle_t cmd);


/**
 * @brief Send a control byte to ssd1306
 *
 * 
 * @param cmd an i2c command queue handle
 * 
 * @param control_byte one of MULT_COMMAND, ONE_COMMAND, MULT_DATA, ONE_DATA, indicating the 
 *                      meaning of the bytes that follow.
 */
esp_err_t ssd1306_send_control_byte(i2c_cmd_handle_t cmd, uint8_t contol_byte);



//! -------------- Higher level functions ---------------------

/**
 * @brief initialize most aspects of OLED
 * 
 * @param 12cnum
 */
esp_err_t ssd1306_init(i2c_port_t i2c_num);

/**
 * @brief Send multiple bytes of data to GDDRAM
 * 
 * @param i2c_num
 * @param data_wr
 * @param size
 */
esp_err_t ssd1306_send_data(i2c_port_t i2c_num, uint8_t *data_wr, size_t size);

/**
 * @brief Set display contrast 
 *
 * @param contrast a value from 1 to 256, where contrast increases as the value increases
 */
esp_err_t ssd1306_set_contrast(i2c_port_t i2c_num, uint8_t contrast);


/**
 * @brief Turn all pixels on, or show RAM contents
 *
 * @param  pixels_on if on=0 display RAM contents on screen, if on=1 ignore RAM and entire display turns on.
 */
esp_err_t ssd1306_entire_display_on(i2c_port_t i2c_num, uint8_t pixels_on);


/**
 * @brief  Set display mode to normal or inverse
 *
 * @param  inverse inverse=1 inverts display of RAM contents so that 1 in RAM --> OFF in display.
 *                  inverse=0 resets display to normal, so 0 in RAM --> OFF in display.
 */
esp_err_t ssd1306_invert_display(i2c_port_t i2c_num, uint8_t inverse);


/**
 * @brief  Turn display on or off
 *
 * @param  display_on Set to 1 to turn display on or set to 0 to put display to sleep.
 */
esp_err_t ssd1306_turn_display_on_off(i2c_port_t i2c_num, uint8_t display_on);


/**
 * @brief  
 *
 * @param  
 */
// static esp_err_t ssd1306_(uint8_t );