#ifndef TFTessentials_h
#define TFTessentials_h
   

#include <TFT_eSPI.h>
#include "lvgl.h"



#define CALIBRATION_FILE "/TouchCalData1"
// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

#define screenWidth 240
#define screenHeight 320

void disp_Init();

void TFT_Input(void);

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

void TFT_Init(int rotation);

void touch_calibrate();

void Create_label(lv_obj_t *label, const char* text);

void lvgl_keyboard(lv_obj_t * kb, lv_obj_t * ta);

void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc);

bool touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);

static void ta_event_cb(lv_obj_t * ta, lv_event_t event);

void get_keyPad(lv_obj_t *ta);
void get_textBox(lv_obj_t *ta);

#endif