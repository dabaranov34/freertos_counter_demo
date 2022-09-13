#include "app_main.h"

/** The size of a queue from conter task to logger task. */
#define LOGGER_QUEUE_SIZE (1UL)

/** Time of a queue holdup while value pending. */
#define LOGGER_QUEUE_HOLDUP_TIME (10UL)


/** !ime between counter increments in miliseconds . */
#define COUNTER_DELTA_T (5000UL)

/** TAG for logger module */
static const char *TAG = "Counter Example";

/** A nandle for queue between counter and logger */
QueueHandle_t xAppMainLoggerQueue;

/**
 * @brief default callback function for logging
 * @param data some data to log
 */
static void logger_default_callback(counter_and_tick_t data)
{
    ESP_LOGI(TAG, "Counter value %d. Delta_t is  %d.", data.pulCounterVal, data.delta_t);
}

/** 
 * @brief A task which increments counter.
 * Creates a queue and continiously increments the counter and then puts its value into the queue.
 * @param pvParameters is empty.
 */
void counter_increment_task(void *pvParameters)
{
    xAppMainLoggerQueue = xQueueCreate(LOGGER_QUEUE_SIZE, sizeof(uint32_t));
    static uint32_t ulCounter = 0;
    while (1) {
        ulCounter++;
        vTaskDelay(COUNTER_DELTA_T / portTICK_PERIOD_MS);
        xQueueSend(xAppMainLoggerQueue, &ulCounter, (TickType_t)LOGGER_QUEUE_SIZE);
    }
}

/** 
 * @brief A task which calls logger.
 * Calculates a time from prevuius call and 
 * @param pvParameters is empty.
 */
void logger_task(void *pvParameters)
{
    uint32_t *pulCounterVal;

    TickType_t previous_tick = xTaskGetTickCount();
    TickType_t current_tick = xTaskGetTickCount();

    counter_and_tick_t counter_and_tick;
    void *(*callback_function)(counter_and_tick_t) = (void *)(pvParameters);
    while (1) {
        if (xAppMainLoggerQueue != 0) {
            if (xQueueReceive(xAppMainLoggerQueue, &(pulCounterVal), (TickType_t)LOGGER_QUEUE_HOLDUP_TIME)) {
                current_tick = xTaskGetTickCount();
                counter_and_tick.pulCounterVal = pulCounterVal;
                counter_and_tick.delta_t = (current_tick - previous_tick)*portTICK_PERIOD_MS;
                callback_function(counter_and_tick);
                previous_tick = current_tick;
            }
        }
    }
}

/** 
 * @brief Main application. 
 * Simply creates two tasks.
 */
void app_main(void)
{
    xTaskCreate(counter_increment_task, "CNTR_INCREMENT", 2048, NULL, 5, NULL);
    xTaskCreate(logger_task, "LOGGER", 2048, logger_default_callback, 5, NULL);
}
