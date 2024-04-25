#ifndef ROM_MSD_H
#define ROM_MSD_H

#include "esp_err.h"
#include "esp_log.h" // Include the ESP logging header
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_types.h" // Include the header for sdmmc_card_t



#define MOUNT_POINT "/sdcard"

// Pin assignments can be set in menuconfig, see "SD SPI Example Configuration" menu.
// You can also change the pin assignments here by changing the following 4 lines.
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   15

#define KEY_LENGTH 10

#define SD_CARD_TAG "SDcard"



extern sdmmc_host_t host; // Declare the host variable as extern

extern sdmmc_card_t *card;

extern bool regist;

esp_err_t LMPD_device_register(const char *filename);
esp_err_t LMPD_device_writing(const char *filename, char *parameter, float data);
esp_err_t LMPD_device_writing_space(const char *filename);
esp_err_t LMPD_device_writing_time(const char *filename, char *parameter, char* data);
esp_err_t LMPD_device_read_block(const char *filename, char *block_buffer, size_t buffer_size);




esp_err_t sd_card_init(void);
esp_err_t sd_card_config(void);
esp_err_t rename_file(const char *old_filename, const char *new_filename);
esp_err_t sd_card_write(const char *filename, const char *data);
esp_err_t sd_card_read(const char *filename, char *buffer, size_t buffer_size);

#endif // SD_CARD_DRIVER_H
