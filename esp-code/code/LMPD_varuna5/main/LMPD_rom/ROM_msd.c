#include "ROM_msd.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include <time.h>
#include "esp_random.h"



sdmmc_host_t host = SDSPI_HOST_DEFAULT();

sdmmc_card_t *card;

bool regist = false;

static long file_position = 0;

static FILE *csv_file = NULL;

#define MAX_LINE_LENGTH 256



esp_err_t sd_card_init(void)
{
    ESP_LOGI(SD_CARD_TAG, "Initializing SD card");

    // Initialize SD card
    esp_err_t sd_init_result = sd_card_config();
    if (sd_init_result != ESP_OK) {
        ESP_LOGE(SD_CARD_TAG, "SD card initialization failed");
    }

    ESP_LOGI(SD_CARD_TAG, "Generating and registering a random key");

    // Generate and register a random key
    esp_err_t register_result = LMPD_device_register(MOUNT_POINT"/regist.txt");
    if (register_result != ESP_OK) {
        ESP_LOGE(SD_CARD_TAG, "Device registration failed");
    }


    ESP_LOGI(SD_CARD_TAG, "Reading and displaying the registered key");

    // Read and display the registered key
    char key_buffer[64]; // Adjust size as needed
    esp_err_t read_result = sd_card_read(MOUNT_POINT"/regist.txt", key_buffer, sizeof(key_buffer));
    if (read_result == ESP_OK) {
        ESP_LOGI(SD_CARD_TAG, "Registered Key: %s", key_buffer);

    } else {
        ESP_LOGE(SD_CARD_TAG, "Failed to read registered key");
    }

    // Unmount SD card (if needed)
    //esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
    //spi_bus_free(host.slot);

    regist = true;
    return ESP_OK;
}


esp_err_t sd_card_config(void)
{
    esp_err_t ret;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(SD_CARD_TAG, "Initializing SD card");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.max_freq_khz = 2000;  // Set desired clock frequency here

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(SD_CARD_TAG, "Failed to initialize bus.");
        return ret;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;

    ESP_LOGI(SD_CARD_TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(SD_CARD_TAG, "Failed to mount filesystem. "
                     "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(SD_CARD_TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return ret;
    }
    ESP_LOGI(SD_CARD_TAG, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);

    // Return the card initialization status
    return ESP_OK;
}

esp_err_t rename_file(const char *old_filename, const char *new_filename)
{
    if (rename(old_filename, new_filename) != 0) {
        ESP_LOGE(SD_CARD_TAG, "Rename failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t sd_card_write(const char *filename, const char *data)
{

    FILE *f;

    if(regist)
    {
        f = fopen(filename, "a");
    }
    else
    {
        f = fopen(filename, "w");
    }

    if (f == NULL) {
        ESP_LOGE(SD_CARD_TAG, "Failed to open file '%s' for writing", filename);
        return ESP_FAIL;
    }
    fprintf(f, "%s\n", data);
    fclose(f);

    return ESP_OK;
}


esp_err_t sd_card_read(const char *filename, char *buffer, size_t buffer_size)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        // Handle error
        return ESP_FAIL;
    }

    fgets(buffer, buffer_size, f);
    fclose(f);

    return ESP_OK;
}

esp_err_t LMPD_device_read_block(const char *filename, char *block_buffer, size_t buffer_size) {
    if (csv_file == NULL) {
        // Open the file if it's not already open
        csv_file = fopen(filename, "r");
        if (csv_file == NULL) {
            return ESP_FAIL; // File open error
        }
    }

    // Clear the block buffer
    block_buffer[0] = '\0';

    char line_buffer[MAX_LINE_LENGTH];
    int empty_line_count = 0;

    while (fgets(line_buffer, sizeof(line_buffer), csv_file) != NULL) {
        // Trim newline characters
        size_t len = strlen(line_buffer);
        if (len > 0 && (line_buffer[len - 1] == '\n' || line_buffer[len - 1] == '\r')) {
            line_buffer[len - 1] = '\0';
        }

        // Check for an empty line
        if (line_buffer[0] == '\0') {
            // Increment empty line count
            empty_line_count++;

            // If two consecutive empty lines are encountered, stop reading
            if (empty_line_count >= 2) {
                break;
            }

            continue;
        }

        // Reset empty line count when a non-empty line is encountered
        empty_line_count = 0;

        // Append the current line to the block buffer
        strncat(block_buffer, line_buffer, buffer_size - strlen(block_buffer) - 1);

        // Ensure the buffer does not exceed its size
        if (strlen(block_buffer) >= buffer_size - 1) {
            printf("Block buffer overflow\n");
            break;
        }
    }

    // Check if end of file is reached or error occurred
    if (feof(csv_file) || ferror(csv_file)) {
        // Close the file
        fclose(csv_file);
        csv_file = NULL;
        return ESP_FAIL; // Return error if EOF or file error
    }

    return ESP_OK;
}


esp_err_t LMPD_device_register(const char *filename)
{
    // Characters to choose from for the key
    const char *key_chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *file_registration = MOUNT_POINT"/registration.txt";

    // Seed the random number generator with current time
    srand(esp_random());
    
    // Generate the key
    char random_key[KEY_LENGTH + 1]; // +1 for null-terminator
    for (int i = 0; i < KEY_LENGTH; i++) {
        int index = rand() % (strlen(key_chars));
        printf("Generated index: %d\n", index); // Print the generated index
        random_key[i] = key_chars[index];
    }
    random_key[KEY_LENGTH] = '\0'; // Null-terminate the key

    // Format the key and data to be written to the file
    char data[200]; // Adjust size as needed
    snprintf(data, sizeof(data), "Device Registration Information\n"
                                  "-----------------------------\n"
                                  "\n"
                                  "Device ID:          LIMPID\n"
                                  "Firmware Version:  1.0.0\n"
                                  "\n"
                                  "Key Information:\n"
                                  "----------------\n"
                                  "Generated Key:     %s\n", random_key);
    
    // Call the sd_card_write function to write the data to the file
    esp_err_t write_result = sd_card_write(filename, data);
    
    if (write_result != ESP_OK) {
        // Handle write error
        return write_result;
    }
    /*
    esp_err_t rename_result = rename_file(filename, file_registration);

    if (rename_result != ESP_OK) {
        // Handle rename error
        return rename_result;
    }*/

    return ESP_OK;
}

esp_err_t LMPD_device_writing(const char *filename, char *parameter, float data)
{   

    char info[200]; // Adjust size as needed
    snprintf(info, sizeof(info), "%s,%f", parameter, data);
    // Call the sd_card_write function to write the data to the file
    esp_err_t write_result = sd_card_write(filename, info);
    
    if (write_result != ESP_OK) {
        // Handle write error
        return write_result;
    }

    return ESP_OK;
}

esp_err_t LMPD_device_writing_time(const char *filename, char *parameter, char* data)
{   

    char info[200]; // Adjust size as needed
    snprintf(info, sizeof(info), "%s, %s", parameter, data);
    // Call the sd_card_write function to write the data to the file
    esp_err_t write_result = sd_card_write(filename, info);
    
    if (write_result != ESP_OK) {
        // Handle write error
        return write_result;
    }

    return ESP_OK;
}

esp_err_t LMPD_device_writing_space(const char *filename)
{   

    char info[2]; // Adjust size as needed
    snprintf(info, sizeof(info), "\n");
    // Call the sd_card_write function to write the data to the file
    esp_err_t write_result = sd_card_write(filename, info);
    
    if (write_result != ESP_OK) {
        // Handle write error
        return write_result;
    }

    return ESP_OK;
}