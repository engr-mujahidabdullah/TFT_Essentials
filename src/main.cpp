#include <Arduino.h>
#include "TFTessentials.h"

/* Globals */

lv_obj_t *main_label ;
/*----------------------------------*/
void setup() 
{
  Serial.begin(115200);

  TFT_Init(1);
  disp_Init();
  TFT_Input();
  //vTaskDelay(500);
  //Create_label(main_label, "Air Sampler");
  //vTaskDelay(5000);
  lvgl_keyboard();
}

void loop() 
{
      lv_task_handler(); /* let the GUI do its work */
      delay(5);
}