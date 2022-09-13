#ifndef __APP_MAIN_H
#define __APP_MAIN_H

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

/** Wrapper for a queue payload */
typedef struct {
    uint32_t *pulCounterVal;
    TickType_t delta_t;
} counter_and_tick_t;

void logger_task(void *pvParameters);

void counter_increment_task(void *pvParameters);

#endif