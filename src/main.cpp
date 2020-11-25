#include <Arduino.h>
#include "TFTessentials.h"

/* Globals */

lv_obj_t *keyBoard ;
lv_obj_t *textBox ;
/*----------------------------------*/
void setup() 
{
  Serial.begin(115200);

  TFT_Init(1);
  disp_Init();
  TFT_Input();

  get_textBox(textBox);

}

void loop() 
{
      lv_task_handler(); /* let the GUI do its work */
      delay(5);
}