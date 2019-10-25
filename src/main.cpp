#include "main.h"

void setup() {
  ads1293::spi_Init(18, 19, 23, 4);
  attachInterrupt(digitalPinToInterrupt(ADS1293_DRDY_PIN), DRDYHandler, FALLING);
  Serial.begin(256000);
  vTaskPrioritySet(NULL, 1);
  loop_task = xTaskGetCurrentTaskHandle();
  xTaskCreate(LEDTask, "LED", 512, NULL, 4, NULL);
  //xTaskCreate(ATCommandTask, "ATCom", 4096, NULL, 3, NULL);
  //xTaskCreate(SerialTask, "Serial", 4096, NULL, 1, NULL);
  ads1293::Init(&is_ads1293_init);
}

uint8_t i,j,k=0, tmp[6];
uint32_t uecgtemp, DRDY_notify = 0;
int32_t secgtemp, sum[2] = {0,0};
void loop() {
  //Serial.println(DRDY_counter);
  DRDY_notify = ulTaskNotifyTake(pdTRUE, 1);
  if (DRDY_notify > 0) {
    ads1293::Read_Data_Stream(&tmp[0], 3);
    j = 0; i=0;
    //for (i = 0; i < 6; i += 3)  // data outputs is (24 bits CH1 +  24 bits CH2)
    //{
      uecgtemp = (unsigned long) (  ((unsigned long)tmp[i] << 16) | ( (unsigned long) tmp[i + 1] << 8) |  (unsigned long) tmp[i + 2]);
      uecgtemp = (unsigned long) (uecgtemp << 8);
      secgtemp = (signed long) (uecgtemp);
      secgtemp = (signed long) (secgtemp >> 8);
      sum[j++] = secgtemp;
    //}
    //k++;
    //if (k==4) {
      //k = 0;
      //sum[0] /= 4;
      //sum[1] /= 4;
      //Serial.write((sum[0] >> 24) & 0xff);
      Serial.write((sum[0] >> 16) & 0xff);
      Serial.write((sum[0] >> 8) & 0xff);
      Serial.write(sum[0] & 0xff);
      //Serial.write((sum[1] >> 24) & 0xff);
      //Serial.write((sum[1] >> 16) & 0xff);
      //Serial.write((sum[1] >> 8) & 0xff);
      //Serial.write(sum[1] & 0xff);
      Serial.write('\n');
      //sum[0] = 0;
      //sum[1] = 0;
    //}
    // create packet from filtered_ecg
    // fill_packet(filtered_ecg[0], filtered_ecg[1]);
  }
}

void IRAM_ATTR DRDYHandler(void)
{
  if (!is_ads1293_init) return;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  configASSERT(loop_task != NULL);
  vTaskNotifyGiveFromISR(loop_task, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR();
}

void LEDTask(void *pvParameters) {
  pinMode(LED_BUILTIN, OUTPUT);
  for (;;) {
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void SerialTask(void *pvParameters) {
  while(true) {
    //only send data when connected
  }
  vTaskDelete(NULL);
}