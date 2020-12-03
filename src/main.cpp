#include <Arduino.h>
#include "TFTessentials.h"

#define test 13

TaskHandle_t Task1;
TaskHandle_t Task2;
SemaphoreHandle_t lvglMutex = NULL;

extern const char * dt_data_ ;
extern const char * st_data_ ;
extern const char * sv_data_ ;

lv_obj_t *scr_1 ;
lv_obj_t *scr_2 ;

lv_obj_t *Welcome ;
lv_obj_t * btnM;


/* create a hardware timer */
hw_timer_t * timer = NULL;

/* Globals */
extern bool data_check;
extern bool data_print;


int integr_data;



bool data_done = false;
/*----------------------------------*/

/* Private Functions*/
void TFT_Task( void *pvParameters);
void next_Task( void *pvParameters);
void led_Task( void *pvParameters) ;
void screen_event_cb(lv_obj_t * obj, lv_event_t event);
void other_tasks();

/*----------------------------------*/

void setup() 
{
  
  Serial.begin(115200);
  lvglMutex = xSemaphoreCreateMutex();
  vTaskDelay(500);
  other_tasks();
}

void other_tasks()
{
  vTaskDelay(500);

  xTaskCreate(led_Task, "led_Task", 4096, NULL, 1, &Task2);
  xTaskCreate(TFT_Task, "TFT_Task", 100000, NULL, 1, NULL);
  xTaskCreate(next_Task, "next_Task", 4096, NULL, 1, &Task1);
}

void loop() 
{

}

void TFT_Task( void *pvParameters) 
{
  vTaskDelay(1);
    lv_init();
    TFT_Init(1);
    disp_Init();
    TFT_Input();

      /*Create a screen*/
    scr_1 = lv_obj_create(NULL, NULL);
    scr_2 = lv_obj_create(NULL, NULL);

    lv_scr_load(scr_1);          /*Load the screen*/
    lv_obj_set_event_cb(scr_1, screen_event_cb);   /*Assign an event callback*/

    Create_label(Welcome, "Air Sampler", scr_1);
    lv_ex_tabview_(scr_2);
    vTaskDelay(1);
    
    vTaskDelay(1);
    while(1)
    {
      lv_task_handler();
       lv_tick_inc(1);
      vTaskDelay(1);
    }
}

void led_Task( void *pvParameters) 
{
  while(1)
  {
    if(data_check == true)
      {
        Serial.print(dt_data_);
        data_check = false;
        vTaskDelay(10);
      }
    vTaskDelay(1);
  }
}

void next_Task( void *pvParameters) 
{
  while(1)
  {
   vTaskDelay(1);
  }
}



/*Screen event handler*/
void screen_event_cb(lv_obj_t * obj, lv_event_t event)
{
  if(event == LV_EVENT_CLICKED)
  {
    lv_scr_load(scr_2);
    lv_obj_del(obj);
  }vTaskDelay(1);
}