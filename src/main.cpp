#include <Arduino.h>
#include "TFTessentials.h"

/* Globals */

lv_obj_t *scr_1 ;
lv_obj_t *scr_2 ;
lv_obj_t *keyBoard ;
lv_obj_t *textBox ;

/*----------------------------------*/
void setup() 
{
  Serial.begin(115200);

  TFT_Init(1);
  disp_Init();
  TFT_Input();
  /*Create a screen*/
  scr_1 = lv_obj_create(NULL, NULL);
  lv_scr_load(scr_1);          /*Load the screen*/
  get_textBox(textBox, scr_1);
}

void loop() 
{
      lv_task_handler(); /* let the GUI do its work */
      delay(5);
}