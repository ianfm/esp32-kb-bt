
#include "ssd1306.h"

#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

#define SSD1306_SENSOR_ADDR CONFIG_SSD1306_ADDR   /*!< slave address for SSD1306 sensor */

#define SSD1306_WRITE_BIT 0x0       /* R/W# from datasheet */
#define SSD1306_READ_BIT 0x1
#define SSD1306_COMMAND_BIT  0x0    /* D/C# from datasheet */
#define SSD1306_DATA_BIT  0x1
#define SSD1306_DATA_BYTES  0x1      /* Co from datasheet */
#define SSD1306_MIXED_BYTES  0x0

// Command byte defs 
#define SSD1306_CMD_SET_CONTRAST_CONTROL 0x81
#define SSD1306_CMD_DISPLAY_POWER_ON     0xAE
#define SSD1306_CMD_DISPLAY_POWER_OFF    0xAF
#define SSD1306_CMD_DISPLAY_MODE_NORMAL  0xA6
#define SSD1306_CMD_DISPLAY_MODE_INVERSE 0xA7
#define SSD1306_CMD_DISPLAY_PIXELS_RAM   0xA4
#define SSD1306_CMD_DISPLAY_PIXELS_ON    0xA5


static esp_err_t ssd1306_send_address_byte(i2c_cmd_handle_t cmd, uint8_t read_write) {
    return i2c_master_write_byte(cmd, (SSD1306_SENSOR_ADDR << 1) | SSD1306_WRITE_BIT, ACK_CHECK_EN);

}


static esp_err_t ssd1306_send_control_byte(i2c_cmd_handle_t cmd, uint8_t Co, uint8_t data_command) {
    return i2c_master_write_byte(cmd, (Co << 7 | data_command << 6), ACK_CHECK_EN);
}

//------------------------------------------------------------


static esp_err_t ssd1306_set_contrast(i2c_port_t i2c_num, uint8_t contrast) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    ssd1306_send_address_byte(cmd, WRITE_BIT);
    ssd1306_send_control_byte(cmd, SSD1306_MIXED_BYTES, SSD1306_COMMAND_BIT);
    i2c_master_write_byte(cmd, SSD1306_CMD_SET_CONTRAST_CONTROL, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, contrast, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}