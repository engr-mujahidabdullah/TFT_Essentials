#include "TFTessentials.h"
#include <FS.h>

/* Defined Globals */
lv_disp_buf_t disp_buf;
lv_color_t buf[LV_HOR_RES_MAX * 10];
const char * _data_ ;
bool data_check = false;

/* Globals */
lv_obj_t * keyB;
lv_obj_t *textBox0 ;
lv_obj_t *textBox1 ;
lv_obj_t *textBox2 ;

lv_obj_t *tabview;
lv_obj_t *tab1 ;
lv_obj_t *tab2 ;
lv_obj_t *tab3 ;
lv_obj_t *tab4 ;

lv_obj_t *lb ;
lv_obj_t *lb1 ;
lv_obj_t *lb2 ;
/* TFT instance */
TFT_eSPI tft = TFT_eSPI(); 

/* TFT Initialie */

void TFT_Init(int rotation)
{
    //lv_init();

    #if USE_LV_LOG != 0
        lv_log_register_print_cb(my_print); /* register print function for debugging */
    #endif

        tft.init(); 
    tft.begin(); /* TFT init */

    tft.setRotation(rotation); /* orientation */

    uint16_t calData[5] = {492,3324,320,3459,3};
    tft.setTouch(calData);

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

void Create_labelPos(lv_obj_t *label, const char* text, lv_obj_t *scr, int x, int y)
{
    label = lv_label_create(scr, NULL);
    lv_label_set_text(label, text);
    lv_obj_set_pos(label, x, y);
}

/* Keypad */
void get_keyPad( lv_obj_t *ta,  lv_obj_t *scr)
{
    /*Create a keyboard and apply the styles*/
    keyB = lv_keyboard_create(scr, NULL);
    lv_obj_set_size(keyB,  320, 120);
    lv_keyboard_set_mode(keyB, LV_KEYBOARD_MODE_NUM);
   // lv_obj_align(keyB, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
     lv_keyboard_set_textarea(keyB, ta);
     lv_keyboard_set_cursor_manage(keyB, true);
   //  lv_obj_set_event_cb(keyB, keyB_event_cb);
}

void get_keyPad_active( lv_obj_t *ta)
{
    /*Create a keyboard and apply the styles*/
    keyB = lv_keyboard_create(lv_scr_act(), NULL);
    lv_obj_set_size(keyB,  320, 120);
    lv_keyboard_set_mode(keyB, LV_KEYBOARD_MODE_NUM);
    lv_obj_align(keyB, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_keyboard_set_textarea(keyB, ta);
    lv_keyboard_set_cursor_manage(keyB, true);
    lv_obj_set_event_cb(keyB, keyB_event_cb);
}

void get_textBox(lv_obj_t *ta, lv_obj_t *scr, int x, int y)
{
    ta = lv_textarea_create(scr, NULL);
    lv_textarea_set_cursor_hidden(ta, true);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_max_length(ta, 12);
    lv_textarea_set_text(ta, "");
    lv_obj_set_size(ta, 315, 30);
    lv_obj_set_event_cb(ta, ta_event_cb);
    lv_obj_set_pos(ta, x, y);
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

      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;
  
      //Serial.print("Data x");
      //Serial.println(touchX);
      
      //Serial.print("Data y");
      //Serial.println(touchY);

    return false; /*Return `false` because we are not buffering and no more data to read*/
}

void ta_event_cb(lv_obj_t * ta, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED)
    {
        get_keyPad_active(ta);
    }
    if(event == LV_EVENT_RELEASED) 
    {
        if(keyB != NULL) {
            lv_obj_set_height(tabview, LV_VER_RES / 2);
            lv_indev_wait_release(lv_indev_get_act());
             get_keyPad_active(ta);

        }
        lv_textarea_set_cursor_hidden(ta, false);
        lv_page_focus(tab2, lv_textarea_get_label(ta), LV_ANIM_ON);
        lv_keyboard_set_textarea(keyB, ta);
    } 
    if(event == LV_EVENT_DEFOCUSED) 
    {
        lv_textarea_set_cursor_hidden(ta, true);
    }
}

void keyB_event_cb(lv_obj_t * keyB, lv_event_t event)
{
    lv_keyboard_def_event_cb(keyB, event);
    if(event == LV_EVENT_APPLY)
    {
       lv_obj_t *ta = lv_keyboard_get_textarea(keyB);
       _data_ = lv_textarea_get_text(ta); data_check = true;
        lv_obj_del(keyB);
        lv_obj_set_height(tabview, LV_VER_RES);
        keyB = NULL;
    }
    if (event == LV_EVENT_CANCEL)
    {
        lv_obj_del(keyB);
        lv_obj_set_height(tabview, LV_VER_RES);
        keyB = NULL;
    }
}

void lv_ex_tabview_( lv_obj_t *scr)
{
    /*Create a Tab view object*/
    
    tabview = lv_tabview_create(scr, NULL);
    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    tab1 = lv_tabview_add_tab(tabview, "Start");
    tab2 = lv_tabview_add_tab(tabview, "Config");
    tab3 = lv_tabview_add_tab(tabview, "Setting");
    tab4 = lv_tabview_add_tab(tabview, "About");

    /*Add content to the tabs*/
    Create_labelPos(lb, "Sampling Time:", tab2, 0,0);
    get_textBox(textBox0, tab2, 0, 20);
    Create_labelPos(lb1, "Delay Time:", tab2, 0,60);
    get_textBox(textBox1, tab2, 0, 80);
    Create_labelPos(lb2, "Fan Speed", tab2, 0,110);
    get_textBox(textBox2, tab2, 0, 130);
}
