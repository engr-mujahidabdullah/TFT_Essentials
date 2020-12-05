#include <Arduino.h>
#include "TFTessentials.h"

#define test 13

TaskHandle_t Task1;
TaskHandle_t Task2;
SemaphoreHandle_t lvglMutex = NULL;

 const char * dt_data_ = "0";
 const char * sv_data_ = "0";
extern const char * disp_label_;

lv_obj_t *scr_1 ;
lv_obj_t *scr_2 ;

lv_obj_t *Welcome ;

extern lv_obj_t *uni_lab;
extern lv_obj_t * table;

/* create a hardware timer */
hw_timer_t * timer = NULL;

/* Globals */
extern bool data_check;
extern bool start_process;


int integr_data;



bool data_done = false;
/*----------------------------------*/

/* Private Functions*/
void TFT_Task( void *pvParameters);
void next_Task( void *pvParameters);
void led_Task( void *pvParameters) ;
void uniLabel_Task( void *pvParameters);
void screen_event_cb(lv_obj_t * obj, lv_event_t event);
void other_tasks();
void refr_func(lv_task_t * task);

/*----------------------------------*/

void setup() 
{
  
  Serial.begin(115200);
  SPIFFS.begin();
  vTaskDelay(100);



  lvglMutex = xSemaphoreCreateMutex();
  vTaskDelay(500);
  other_tasks();
}

void other_tasks()
{
  vTaskDelay(500);

  xTaskCreate(led_Task, "led_Task", 4096, NULL, 1, &Task2);
  xTaskCreate(uniLabel_Task, "uniLabel_Task", 4096, NULL, 1, NULL);
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

      String dat = readFile(SPIFFS, "/dt.txt");
  dt_data_ = dat.c_str();

  String bat = readFile(SPIFFS, "/sv.txt");
  sv_data_ = bat.c_str();
  
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
        writeFile(SPIFFS, "/dt.txt", dt_data_);
        writeFile(SPIFFS, "/sv.txt", sv_data_);
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

void uniLabel_Task( void *pvParameters) 
{
  while(1)
  {
   if(start_process == true)
   {
      disp_label_ = dt_data_;
      int x = atoi(disp_label_);
      while(x > 0)
      {
        lv_event_send(uni_lab, LV_EVENT_REFRESH, NULL);
        x --;
        itoa(x, (char*)disp_label_, 10);
        vTaskDelay(1000);
      }vTaskDelay(1);
      disp_label_ = "Running ...";
      lv_event_send(uni_lab, LV_EVENT_REFRESH, NULL);
      start_process = false;
   }vTaskDelay(1);
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


