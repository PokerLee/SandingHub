#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/twai.h"

/* --------------------- Definitions and static variables ------------------ */

// Example Configurations
#define NO_OF_MSGS              100
#define NO_OF_ITERS             3
#define TX_GPIO_NUM             40
#define RX_GPIO_NUM             41
#define TX_TASK_PRIO            8       // Sending task priority
#define CTRL_TSK_PRIO           10      //Control task priority
#define MSG_ID                  0x555   // 11 bit standard format ID
#define EXAMPLE_TAG             "TWAI Send Only"

static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
// Filter all other IDs except MSG_ID
static const twai_filter_config_t f_config = {.acceptance_code = (MSG_ID << 21),
                                              .acceptance_mask = ~(TWAI_STD_ID_MASK << 21),
                                              .single_filter = true
                                             };
// Set to normal mode for external communication
static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL);

static SemaphoreHandle_t tx_sem;
static SemaphoreHandle_t ctrl_sem;
static SemaphoreHandle_t done_sem;

/* --------------------------- Tasks and Functions prototypes -------------------------- */
void can_init(void);