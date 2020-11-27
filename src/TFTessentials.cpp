#include "TFTessentials.h"
#include <FS.h>

/* Defined Globals */
lv_disp_buf_t disp_buf;
lv_color_t buf[LV_HOR_RES_MAX * 10];
const char * _data_ ;

/* Globals */
lv_obj_t * keyB;

/* TFT instance */
TFT_eSPI tft = TFT_eSPI(); 

/* TFT Initialie */

void TFT_Init(int rotation)
{
    lv_init();

    #if USE_LV_LOG != 0
        lv_log_register_print_cb(my_print); /* register print function for debugging */
    #endif

        tft.init(); tft.begin(); /* TFT init */

    uint16_t calData[5] = {437, 3364, 331, 3480, 3 };
    tft.setTouch(calData);

    tft.setRotation(rotation); /* orientation */

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);
}


/* Display_Flush */

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}


/* LVGl Display Init */

void disp_Init()
{
    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenHeight;
    disp_drv.ver_res = screenWidth;
    disp_drv.flush_cb = disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);
}

/* LVGl Input Driver */
void TFT_Input(void)
{
    /*Initialize the (dummy) input device driver*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
}

/* LABEL */
void Create_label(lv_obj_t *label, const char* text, lv_obj_t *scr)
{
    label = lv_label_create(scr, NULL);
    lv_label_set_text(label, text);
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
}

/* Keypad */
void get_keyPad( lv_obj_t *ta,  lv_obj_t *scr)
{
    /*Create a keyboard and apply the styles*/
    keyB = lv_keyboard_create(scr, NULL);
    lv_obj_set_size(keyB,  250, 140);
    lv_keyboard_set_mode(keyB, LV_KEYBOARD_MODE_NUM);
   // lv_obj_align(keyB, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
     lv_keyboard_set_textarea(keyB, ta);
     lv_obj_set_event_cb(ta, Get_TaData);
}

void get_keyPad_active( lv_obj_t *ta)
{
    /*Create a keyboard and apply the styles*/
    keyB = lv_keyboard_create(lv_scr_act(), NULL);
    lv_obj_set_size(keyB,  250, 140);
    lv_keyboard_set_mode(keyB, LV_KEYBOARD_MODE_NUM);
    lv_obj_align(keyB, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_keyboard_set_textarea(keyB, ta);
    lv_obj_set_event_cb(ta, Get_TaData);
}

void get_textBox(lv_obj_t *ta, lv_obj_t *scr)
{
    ta = lv_textarea_create(scr, NULL);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_max_length(ta, 12);
    lv_textarea_set_one_line(ta, true);
    //lv_obj_align(ta, NULL, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_textarea_set_text(ta, "");
    lv_obj_set_size(ta, 240, 70);
    lv_obj_set_event_cb(ta, ta_event_cb);
    
}


#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{

    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    Serial.flush();
}
#endif

/*Read the touchpad*/
bool touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch(&touchX, &touchY, 600);

    if(!touched)
    {
      data->state = LV_INDEV_STATE_REL;
      return false;
    }
    else
    {
      data->state = LV_INDEV_STATE_PR;
    }

    if(touchX>screenWidth || touchY > screenHeight)
    {
      //Serial.println("Y or y outside of expected parameters..");
      //Serial.print("y:");
      //Serial.print(touchX);
      //Serial.print(" x:");
      //Serial.print(touchY);
    }
    else
    {
      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;
  
      //Serial.print("Data x");
      //Serial.println(touchX);
      
      //Serial.print("Data y");
      //Serial.println(touchY);
    }

    return false; /*Return `false` because we are not buffering and no more data to read*/
}

static void ta_event_cb(lv_obj_t * ta, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED )
    {
        if(keyB == NULL)  get_keyPad_active(ta);
    }
}

static void Get_TaData(lv_obj_t * ta, lv_event_t event)
{
    if(event == LV_EVENT_APPLY)
        _data_ = lv_textarea_get_text(ta);
    Serial.print(_data_);
}