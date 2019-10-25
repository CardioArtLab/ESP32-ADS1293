#include <Arduino.h>
#include "ads1293.h"

bool is_ads1293_init = false;
volatile uint8_t DRDY_counter = 0;

TaskHandle_t loop_task = NULL;

void IRAM_ATTR DRDYHandler(void);
void LEDTask(void *pvParameters);
void SerialTask(void *pvParameters);