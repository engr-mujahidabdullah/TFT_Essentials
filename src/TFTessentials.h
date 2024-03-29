#ifndef TFTessentials_h
#define TFTessentials_h
   

#include <TFT_eSPI.h>
#include "lvgl.h"
#include "save.h"


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

void lvgl_keyboard(lv_obj_t * kb, lv_obj_t * ta);

void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc);

bool touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data);

void ta_event_cb(lv_obj_t * ta, lv_event_t event);

void keyB_event_cb(lv_obj_t * keyB, lv_event_t event);

void Create_label(lv_obj_t *label, const char* text, lv_obj_t *scr);

void get_keyPad( lv_obj_t *ta,  lv_obj_t *scr);

void get_keyPad_active( lv_obj_t *ta);

void get_textBox(lv_obj_t *ta, lv_obj_t *scr, int id, int x, int y);

void Create_labelPos(lv_obj_t *label, const char* text, lv_obj_t *scr, int x, int y);

void Create_btn( lv_obj_t *btn, lv_obj_t *lab, lv_obj_t *scr, const char* title,int id, int x, int y);

void lv_ex_btnmatrix(lv_obj_t * btnmM, lv_obj_t *scr);

void my_refr_func(void);

void tab_handler(lv_obj_t * obj, lv_event_t event);

void lab_event(lv_obj_t *obj, lv_event_t event);

void table_handler(lv_obj_t *obj, lv_event_t event);

void get_data();

void lv_ex_page_(void);

void Create_Bar(lv_obj_t *scr);

void lv_ex_table( lv_obj_t * scr);

void lv_ex_tabview_( lv_obj_t *scr);

void Create_slider(lv_obj_t *slider, lv_obj_t *lb, lv_obj_t *scr, int x, int y);

void time_ta_event_cb(lv_obj_t * ta, lv_event_t event);

void Time_textarea(lv_obj_t *ta, lv_obj_t *scr, int x, int y);

void slider_event_cb(lv_obj_t * slider, lv_event_t event);

void event_handler(lv_obj_t * obj, lv_event_t event);

void page_handler(lv_obj_t * obj, lv_event_t event);

void msg_handler(lv_obj_t * obj, lv_event_t event);

void lv_close_page_(void);

void mainDisplay(void);

void lab2_event(lv_obj_t *obj, lv_event_t event);

#endif