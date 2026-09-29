#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "mpu6050.h"

#define I2C_MASTER_SCL_IO           5
#define I2C_MASTER_SDA_IO           4
#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          100000

static const char *TAG = "MPU6050_IAM";

static esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) return err;
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void app_main(void) {
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "Barramento I2C inicializado.");

    mpu6050_handle_t mpu6050 = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);
    if (mpu6050 == NULL) {
        ESP_LOGE(TAG, "Falha ao criar o sensor MPU6050");
        return;
    }
    ESP_LOGI(TAG, "Handle do MPU6050 criado.");

    esp_err_t err = mpu6050_config(mpu6050, ACCE_FS_2G, GYRO_FS_250DPS);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao configurar MPU6050: %s", esp_err_to_name(err));
        return;
    }
    err = mpu6050_wake_up(mpu6050);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao acordar o MPU6050: %s", esp_err_to_name(err));
        return;
    }
    ESP_LOGI(TAG, "MPU6050 inicializado com sucesso!");

    mpu6050_acce_value_t acce;
    mpu6050_gyro_value_t gyro;

    while (1) {
        ESP_LOGI(TAG, "Antes da leitura do MPU6050");
        if (mpu6050_get_acce(mpu6050, &acce) == ESP_OK && mpu6050_get_gyro(mpu6050, &gyro) == ESP_OK) {
            ESP_LOGI(TAG, "Leitura OK");
            ESP_LOGI(TAG, "Acel [X:%.2f, Y:%.2f, Z:%.2f] | Giro [X:%.2f, Y:%.2f, Z:%.2f]",
                     acce.acce_x, acce.acce_y, acce.acce_z,
                     gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
        } else {
            ESP_LOGE(TAG, "Falha na leitura do MPU6050");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}