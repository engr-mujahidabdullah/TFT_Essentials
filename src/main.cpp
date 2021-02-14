#include <Arduino.h>
#include "TFTessentials.h"
#include "extra.h"

#define rpm_pin 34
#define control_pin 27
#define fan_pin 14

// setting PWM properties
const int freq = 5000;
const int Channel = 0; 
const int resolution = 8;

TaskHandle_t Task1;
TaskHandle_t Task2;
SemaphoreHandle_t lvglMutex = NULL;

 const char * dt_data_ = "0";
 const char * sv_data_ = "0";
extern const char * disp_label_;
extern const char * flow_label_ ;

String temp_a; String temp_b;

lv_obj_t *scr_1 ;
lv_obj_t *scr_2 ;

lv_obj_t *Welcome ;

extern lv_obj_t *uni_lab;
extern lv_obj_t *vol_lab;
extern lv_obj_t * table;
extern lv_obj_t * _btn_;
extern lv_obj_t * page;

/* create a hardware timer */
hw_timer_t * timer = NULL;

/* Globals */
extern bool data_check;
extern bool start_process;

int integr_data;

bool data_done = false;

int count = 0;
unsigned long start_time;
unsigned long air_time;
int rpm ; 
int pwm = 100; 
int val = 0;
int extra = 0;
float Cli_factor = 0.00382;
float set_vol = 0.0;
float get_vol = 0.000;
float on_time = 0.000;
bool start_flag = false;
bool rpm_flag = false;
bool time_flag = false;
float flow_sum = 0.00;
/*----------------------------------*/

/* Private Functions*/


void TFT_Task( void *pvParameters);
void next_Task( void *pvParameters);
void save_Task( void *pvParameters) ;
void task_rpm(void* pvParameters);
void uniLabel_Task( void *pvParameters);
void screen_event_cb(lv_obj_t * obj, lv_event_t event);
void other_tasks();
void counter(void);
float volume_flow(float clebrated_factor, int rpm_);

/*----------------------------------*/

void setup() 
{
  
  Serial.begin(115200);
  SPIFFS.begin();
  vTaskDelay(200);

   pinMode(rpm_pin, INPUT_PULLUP);
   pinMode(fan_pin, OUTPUT);

      // configure  PWM functionalitites
      ledcSetup(Channel, freq, resolution);
      ledcAttachPin(control_pin, Channel);
  
  attachInterrupt(digitalPinToInterrupt(rpm_pin), counter, RISING);
  other_tasks();
}

void other_tasks()
{
  vTaskDelay(500);

  xTaskCreate(save_Task, "save_Task", 4096, NULL, 1, &Task2);
  xTaskCreate(task_rpm, "Task_rpm", 4096, NULL, 1, NULL);
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
    vTaskDelay(100);

    temp_a = readFile(SPIFFS, "/dt.txt"); dt_data_ = temp_a.c_str(); 
    temp_b = readFile(SPIFFS, "/sv.txt"); sv_data_ = temp_b.c_str();

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

/* ----------Task to save Variabels in SPIFFS------------ */
void save_Task( void *pvParameters) 
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
//--------------------------------------------//

void next_Task( void *pvParameters) 
{
  while(1)
  {
   vTaskDelay(1);
  }
}
//_______________________________________//

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
      start_flag = true;
      lv_event_send(uni_lab, LV_EVENT_REFRESH, NULL);
    
      if(start_flag == true)
      {
        flow_sum = 0; set_vol = atof(sv_data_); Serial.print(set_vol);
        digitalWrite(fan_pin, HIGH);
        ledcWrite(Channel, 255);vTaskDelay(1);
        while(flow_sum < set_vol)
        {
          vTaskDelay(1000);
          flow_sum = flow_sum + ((rpm * Cli_factor)/60);
          lv_event_send(vol_lab, LV_EVENT_REFRESH, NULL);
        }vTaskDelay(1);
      }
      ledcWrite(Channel, 0);       digitalWrite(fan_pin, LOW);
      vTaskDelay(500);
      lv_event_send(page, LV_EVENT_REFRESH, NULL);
       start_flag = false;  start_process = false;
 

      temp_a = readFile(SPIFFS, "/dt.txt"); dt_data_ = temp_a.c_str();
      temp_b = readFile(SPIFFS, "/sv.txt"); sv_data_ = temp_b.c_str(); 
      vTaskDelay(1);
   }vTaskDelay(1);
  }
}

/* ------------RPM Calculate Task-------------- */
void task_rpm(void* pvParameters)
{
  while(1)
  {
    if(start_flag == true)
    {
      start_time = millis();
      count = 0;
      while((millis() - start_time) < 1000){};
      rpm = (count * 60)/2;
      Serial.println(rpm);
    }
    vTaskDelay(1);
  }
}

/* ------------COUNT for RPM-------------- */
void counter(void)
{
  count++;
}
/* ------------ Volume Flow ---------------*/

float volume_flow(float clebrated_factor, int rpm_)
{
  float air_flow = clebrated_factor * rpm_ ;
  return air_flow;
}

//------------------------------------------------//

/*Screen event handler*/
void screen_event_cb(lv_obj_t * obj, lv_event_t event)
{
  if(event == LV_EVENT_CLICKED)
  {
    lv_scr_load(scr_2);
    lv_obj_del(obj);
  }vTaskDelay(1);
}

