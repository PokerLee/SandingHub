#include "can.h"

/* --------------------------- Tasks and Functions -------------------------- */

static void twai_transmit_task(void *arg)
{
    twai_message_t tx_msg = {
        // Message type and format settings
        .extd = 0,              // Standard Format message (11-bit ID)
        .rtr = 0,               // Send a data frame
        .ss = 0,                // Not single shot
        .self = 0,              // No self reception (not loopback)
        .dlc_non_comp = 0,      // DLC is less than 8
        // Message ID and payload
        .identifier = MSG_ID,
        .data_length_code = 1,
        .data = {0},
    };

    for (int iter = 0; iter < NO_OF_ITERS; iter++) {
        xSemaphoreTake(tx_sem, portMAX_DELAY);
        for (int i = 0; i < NO_OF_MSGS; i++) {
            // Transmit messages over the bus
            tx_msg.data[0] = i;
            // ESP_ERROR_CHECK(twai_transmit(&tx_msg, portMAX_DELAY));
            esp_err_t err = twai_transmit(&tx_msg, portMAX_DELAY);
            if (err != ESP_OK) {
                ESP_LOGE(EXAMPLE_TAG, "Transmission failed with error: %s", esp_err_to_name(err));
            }
            else
            {
                ESP_LOGI(EXAMPLE_TAG, "Transmitted Message: ID 0x%lx, Data = %d", tx_msg.identifier, tx_msg.data[0]);
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    vTaskDelete(NULL);
}

static void twai_control_task(void *arg)
{
    xSemaphoreTake(ctrl_sem, portMAX_DELAY);
    for (int iter = 0; iter < NO_OF_ITERS; iter++) {
        // Start TWAI Driver for this iteration
        ESP_ERROR_CHECK(twai_start());
        ESP_LOGI(EXAMPLE_TAG, "Driver started");

        // Trigger TX task to start transmitting
        xSemaphoreGive(tx_sem);

        // Wait a bit after sending before stopping the driver
        vTaskDelay(pdMS_TO_TICKS(100));

        // Stop the TWAI Driver, ensure TWAI is properly stopped
        ESP_ERROR_CHECK(twai_stop());  
        ESP_LOGI(EXAMPLE_TAG, "Driver stopped");
        vTaskDelay(pdMS_TO_TICKS(100)); // Add some delay here to ensure driver is stopped properly

        vTaskDelay(pdMS_TO_TICKS(100)); // Delay before starting the next iteration
    }

    xSemaphoreGive(done_sem);
    vTaskDelete(NULL);
}

void can_init(void)
{
    // Create synchronization primitives
    tx_sem = xSemaphoreCreateBinary();
    ctrl_sem = xSemaphoreCreateBinary();
    done_sem = xSemaphoreCreateBinary();

    // Create the tasks
    xTaskCreatePinnedToCore(twai_control_task, "TWAI_ctrl", 4096, NULL, CTRL_TSK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(twai_transmit_task, "TWAI_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);

    // Install TWAI driver
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(EXAMPLE_TAG, "Driver installed");

    // Start control task
    xSemaphoreGive(ctrl_sem);
    // Wait for all iterations and tasks to complete running
    xSemaphoreTake(done_sem, portMAX_DELAY);

    // Uninstall TWAI driver
    ESP_ERROR_CHECK(twai_driver_uninstall());
    ESP_LOGI(EXAMPLE_TAG, "Driver uninstalled");

    // Cleanup
    vSemaphoreDelete(tx_sem);
    vSemaphoreDelete(ctrl_sem);
    vQueueDelete(done_sem);
}
