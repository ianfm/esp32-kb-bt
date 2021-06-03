
#include "ssd1306.h"

#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

// #define SSD1306_SENSOR_ADDR CONFIG_SSD1306_ADDR   /*!< I don't understand the menu system well enough to make this work. */
#define SSD1306_SENSOR_ADDR 0x78   /*!< slave address for SSD1306 sensor */
#define SSD1306_SA0 0b0         /* Slave Address bit 0 */

#define SSD1306_WRITE_BIT   0x0       /* R/W# from datasheet */
#define SSD1306_READ_BIT    0x1
#define SSD1306_COMMAND_BIT 0x0    /* D/C# from datasheet */
#define SSD1306_DATA_BIT    0x1
#define SSD1306_DATA_BYTES  0x1      /* Co from datasheet */
#define SSD1306_MIXED_BYTES 0x0

// Control byte defs
#define MULT_COMMAND 0x80
#define ONE_COMMAND  0x00
#define MULT_DATA    0xC0
#define ONE_DATA     0x40


// Fundamental Command byte defs 
#define SSD1306_CMD_SET_CONTRAST_CONTROL 0x81

#define SSD1306_CMD_DISPLAY_POWER_OFF    0xAE
#define SSD1306_CMD_DISPLAY_POWER_ON     0xAF

#define SSD1306_CMD_DISPLAY_MODE_NORMAL  0xA6
#define SSD1306_CMD_DISPLAY_MODE_INVERSE 0xA7

#define SSD1306_CMD_DISPLAY_PIXELS_RAM   0xA4
#define SSD1306_CMD_DISPLAY_PIXELS_ON    0xA5

// Scrolling Command byte defs

// Address Setting Command byte defs
// Page addressing mode only
// 0x0 - 0xF
#define SSD1306_CMD_SET_LOW_COL_ADDR    0x00
// 0x10 - 0x1F
#define SSD1306_CMD_SET_HI_COL_ADDR     0x10

// Addressing modes - 2 byte cmd
#define SSD1306_CMD_ADDR_MODE 0x20
#define SSD1306_CMD_ADDR_MODE_HORZ 0x00
#define SSD1306_CMD_ADDR_MODE_VERT 0x01
#define SSD1306_CMD_ADDR_MODE_PAGE 0x02

// Set col address for H or V mode
// 3 byte cmd
#define SSD1306_CMD_SET_COL_ADDR_RANGE 0X21

// H or V mode only
#define SSD1306_CMD_SET_PAGE_ADDR 0x22

// page addressing mode only
// B0 + 0-7
#define SSD1306_CMD_SET_PAGE_START 0xB0

// Hardware Config Command bytes
// 0x40 - 0X7F
#define SSD1306_CMD_DISP_START_LINE 0x40

#define SSD1306_CMD_SEG_REMAP_0 0xA0
#define SSD1306_CMD_SEG_REMAP_127 0xA1

// 2 byte cmd
// Set mux ratio to N-1 MUX
#define SSD1306_CMD_SET_MUX_RATIO 0xA8


esp_err_t ssd1306_send_address_byte(i2c_cmd_handle_t cmd) {
    return i2c_master_write_byte(cmd, (SSD1306_SENSOR_ADDR | (SSD1306_SA0 << 1) | SSD1306_WRITE_BIT), ACK_CHECK_EN);

}


esp_err_t ssd1306_send_control_byte(i2c_cmd_handle_t cmd, uint8_t control_byte) {
    return i2c_master_write_byte(cmd, control_byte, ACK_CHECK_EN);
}

//!------------------------------------------------------------

esp_err_t ssd1306_send_data(i2c_port_t i2c_num, uint8_t *data_wr, size_t size) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    ssd1306_send_address_byte(cmd, WRITE_BIT);
    ssd1306_send_control_byte(cmd, SSD1306_DATA_BYTES, SSD1306_DATA_BIT);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}


esp_err_t ssd1306_set_contrast(i2c_port_t i2c_num, uint8_t contrast) {
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

esp_err_t ssd1306_entire_display_on(i2c_port_t i2c_num, uint8_t pixels_on) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    ssd1306_send_address_byte(cmd, WRITE_BIT);
    ssd1306_send_control_byte(cmd, SSD1306_MIXED_BYTES, SSD1306_COMMAND_BIT);
    if (pixels_on) {
        i2c_master_write_byte(cmd, SSD1306_CMD_DISPLAY_PIXELS_ON, ACK_CHECK_EN);
    } else {
        i2c_master_write_byte(cmd, SSD1306_CMD_DISPLAY_PIXELS_RAM, ACK_CHECK_EN);
    }
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS); /* TODO: understand timing of this line */
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t ssd1306_invert_display(i2c_port_t i2c_num, uint8_t inverse) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    ssd1306_send_address_byte(cmd, WRITE_BIT);
    ssd1306_send_control_byte(cmd, SSD1306_MIXED_BYTES, SSD1306_COMMAND_BIT);
    if (inverse) {
        i2c_master_write_byte(cmd, SSD1306_CMD_DISPLAY_MODE_INVERSE, ACK_CHECK_EN);
    } else {
        i2c_master_write_byte(cmd, SSD1306_CMD_DISPLAY_MODE_NORMAL, ACK_CHECK_EN);
    }
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t ssd1306_turn_display_on_off(i2c_port_t i2c_num, uint8_t display_on) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    ssd1306_send_address_byte(cmd, WRITE_BIT);
    ssd1306_send_control_byte(cmd, SSD1306_MIXED_BYTES, SSD1306_COMMAND_BIT);
    if (display_on) {
        i2c_master_write_byte(cmd, SSD1306_CMD_DISPLAY_POWER_ON, ACK_CHECK_EN);
    } else {
        i2c_master_write_byte(cmd, SSD1306_CMD_DISPLAY_POWER_OFF, ACK_CHECK_EN);
    }
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}