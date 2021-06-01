/* i2c - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "ssd1306.h"
#include "sdkconfig.h"

static const char *TAG = "i2c-example";

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 240                  /*!< Data buffer length of test buffer */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL               /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA               /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define SSD1306_SENSOR_ADDR CONFIG_SSD1306_ADDR   /*!< slave address for SSD1306 sensor */

#define WRITE_BIT I2C_MASTER_WRITE              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                       /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                             /*!< I2C ack value */
#define NACK_VAL 0x1                            /*!< I2C nack value */

SemaphoreHandle_t print_mux = NULL;



/**
 * @brief test code to operate an SSD1306
 *
 * 1. Write data to GDDRAM
 * ________________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | ... | stop |
 * --------|---------------------------|---------------------|-----|------|
 * 2. Turn display on
 * ______________________________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 0xAF + ack  |  | stop |
 * --------|---------------------------|-------------------|--------------------|------|
 * 2. wait more than (24)? ms
 */
esp_err_t i2c_master_write_display(i2c_port_t i2c_num, uint8_t* data)
{
    ssd1306_turn_display_on_off(i2c_num, 1);
    // generate data buffer for GDDRAM
    for(int i=0; i<30; ++i){
        static uint8_t val = 0xA7;
        val = ((i * val) % 0xF1) & (0x95 * i);
        data[i*8] = val;    // funky display formula
    }
    //! I haven't read through enough data write notes to know exactly where this will write in RAM
    esp_err_t ret = ssd1306_send_data(i2c_num, data, DATA_LENGTH);
    return ret;
}

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}


/**
 * @brief test function to show buffer
 */
static void disp_buf(uint8_t *buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

static void display_test_task(void *arg)
{
    int ret;
    uint8_t *data = (uint8_t *)malloc(DATA_LENGTH);
    uint32_t task_idx = (uint32_t)arg;
    int cnt = 0;
    while (1) {
        ESP_LOGI(TAG, "TASK[%d] test cnt: %d", task_idx, cnt++);

        ret = i2c_master_write_display(I2C_MASTER_NUM, data); // What is 12cnum???

        // TODO: print out some logging information
        xSemaphoreTake(print_mux, portMAX_DELAY);
        if (ret == ESP_ERR_TIMEOUT) {
            ESP_LOGE(TAG, "I2C Timeout");
        } else if (ret == ESP_OK) {
            printf("*******************\n");
            printf("TASK[%d]  MASTER WRITE DISPLAY( SSD1306 )\n", task_idx);
            printf("*******************\n");
            printf("240 bytes written to GDDRAM\n");
            disp_buf(data, DATA_LENGTH);
            printf("*******************\n");
            printf("cnt: %02x\n", cnt);
        } else {
            ESP_LOGW(TAG, "%s: No ack, display not connected...skip...", esp_err_to_name(ret));
        }
        xSemaphoreGive(print_mux); 

        vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_RATE_MS);
    }
    vSemaphoreDelete(print_mux);
    free(data);
    vTaskDelete(NULL);
}

void app_main(void)
{
    print_mux = xSemaphoreCreateMutex();
    ESP_ERROR_CHECK(i2c_master_init());
    xTaskCreate(display_test_task, "i2c_test_task", 1024 * 2, (void *)0, 10, NULL);
}
