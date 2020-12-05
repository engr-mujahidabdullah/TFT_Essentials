#include "TFTessentials.h"
#include <FS.h>
#include <Arduino.h>
/* Defined Globals */
lv_disp_buf_t disp_buf;
lv_color_t buf[LV_HOR_RES_MAX * 10];
extern const char * dt_data_  ;
extern const char * sv_data_  ;

const char * disp_label_ = "0";

bool data_check = false;
bool start_process = false;
/* Globals */

static lv_obj_t * keyB;
static lv_obj_t *delay_t ;
static lv_obj_t *sampling_v ;

lv_obj_t * table;

lv_obj_t *tabview;
lv_obj_t *tab1 ;
lv_obj_t *tab2 ;
lv_obj_t *tab3 ;

lv_obj_t *uni_lab;

lv_obj_t *set_btn;
lv_obj_t *set_lb;


lv_obj_t *lb ;
lv_obj_t *lb1 ;
lv_obj_t *lb2 ;


lv_obj_t *keyBoard ;


bool key_B = false;
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

    return false; /*Return `false` because we are not buffering and no more data to read*/
}


/*----------------------------------------------------------*/

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
    lv_obj_set_pos(label, x, y);
    lv_label_set_text(label, text);

}

/* Keypad */
void get_keyPad( lv_obj_t *ta,  lv_obj_t *scr)
{
    /*Create a keyboard and apply the styles*/
    keyB = lv_keyboard_create(scr, NULL);
    lv_obj_set_size(keyB,  320, 120);
    lv_keyboard_set_mode(keyB, LV_KEYBOARD_MODE_NUM);
    lv_obj_align(keyB, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
     lv_keyboard_set_textarea(keyB, ta);
     lv_keyboard_set_cursor_manage(keyB, true);
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


/*Text Box*/
void get_textBox(lv_obj_t *ta, lv_obj_t *scr, int id, int x, int y)
{
    ta = lv_textarea_create(scr, NULL);
    lv_textarea_set_cursor_hidden(ta, true);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_max_length(ta, 12);
    lv_obj_set_pos(ta, x, y);
    lv_textarea_set_text(ta,"");
    lv_textarea_set_placeholder_text(ta, "0000");
    lv_obj_set_event_cb(ta, ta_event_cb); 
    lv_obj_set_user_data(ta, id); 
}

/*Button*/
void Create_btn( lv_obj_t *btn, lv_obj_t *lab, lv_obj_t *scr, const char* title,int id, int x, int y)
{

    btn = lv_btn_create(scr, NULL);
    lv_obj_set_user_data(btn, id);
    lv_obj_set_pos(btn, x, y);
    lab = lv_label_create(btn, NULL);
    lv_obj_set_event_cb(btn, event_handler);
    lv_label_set_text(lab, title);
}

/*----------------------------------------------------------*/

void lab_event(lv_obj_t *obj, lv_event_t event)
{
    if(event == LV_EVENT_REFRESH)
    {
        lv_label_set_text(uni_lab,disp_label_);
    }
}

/* Text Box event handler*/
void ta_event_cb(lv_obj_t * ta, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) 
    {
        if(key_B == false) 
        {
            lv_obj_set_height(tabview, LV_VER_RES / 2);
            get_keyPad_active(ta);
            lv_indev_wait_release(lv_indev_get_act());
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



/*Button Handler*/
void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) 
    {
        int id = lv_obj_get_user_data(obj);
        if(id == 0)
            data_check = true;

        if(id == 1)
        {
            lv_obj_move_background(tab1);
            lv_ex_page_();
            start_process = true;
        }
        if(id == 3)
        {
            lv_obj_t *parent = lv_obj_get_parent(obj);
            lv_obj_move_foreground(tabview);
            lv_obj_clean(parent);
            lv_obj_del(parent);
        }
            
    }
vTaskDelay(1);
}

void tab_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) 
    {
       lv_event_send(table, LV_EVENT_VALUE_CHANGED, NULL);
    }
    vTaskDelay(1);
}

void table_handler(lv_obj_t *obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
        {
            lv_table_set_cell_value(obj, 0, 1, sv_data_);
            lv_table_set_cell_value(obj, 1, 1, dt_data_);
        }
}

/* Key_pad event handler*/
void keyB_event_cb(lv_obj_t * keyB, lv_event_t event)
{
    lv_keyboard_def_event_cb(keyB, event);
    if(event == LV_EVENT_APPLY)
    {
        get_data(); 
        lv_obj_set_height(tabview, LV_VER_RES);
        lv_obj_del(keyB);
       keyB = NULL;
       key_B = false;
    }
    if (event == LV_EVENT_CANCEL)
    {
        if(keyB) 
        {
            get_data(); 
            lv_obj_set_height(tabview,  LV_VER_RES);
            lv_obj_del(keyB);
            keyB = NULL;
            key_B = false;
        }
    }
}
/*-------------------------------------*/
/* OTHER FUNCTIONS */

void get_data()
{
    lv_obj_t *t = lv_keyboard_get_textarea(keyB);
    int x = lv_obj_get_user_data(t);
    if(x == 1) sv_data_ = lv_textarea_get_text(t);
    if(x == 2) dt_data_ = lv_textarea_get_text(t);
}

void change_dispValue()
{
    lv_table_set_cell_value(table, 0, 1,(const char*) sv_data_);
    lv_table_set_cell_value(table, 1, 1, dt_data_);
}


/*-------------------------------------*/
void lv_ex_tabview_( lv_obj_t *scr)
{
    /*Create a Tab view object*/
    tabview = lv_tabview_create(scr, NULL);
    lv_obj_set_event_cb(tabview, tab_handler);

    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    tab1 = lv_tabview_add_tab(tabview, "Start");
    tab2 = lv_tabview_add_tab(tabview, "Config");
    tab3 = lv_tabview_add_tab(tabview, "About");

    /*Add content to the tab1*/
    lv_ex_table(tab1);
    Create_btn(set_btn, set_lb, tab1, "START", 1, 100, 255);

    /*Add content to the tab2*/
    Create_btn(set_btn, set_lb, tab2, "SET", 0, 100, 135);
    Create_labelPos(lb1, "Sampling Air Volume (L):", tab2, 0,10);
    get_textBox(sampling_v, tab2, 1, 0, 35);
    Create_labelPos(lb1, "Delay Time (sec): ", tab2, 0,70);
    get_textBox(delay_t, tab2, 2, 0, 95);

}

/* Table */
void lv_ex_table( lv_obj_t * scr)
{
    table = lv_table_create(scr, NULL);
    lv_table_set_col_cnt(table, 2);
    lv_table_set_row_cnt(table, 2);
    lv_obj_align(table, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(table, table_handler);
    /*Fill the first column*/
    lv_table_set_cell_value(table, 0, 0, "Sample Volume(L)");
    lv_table_set_cell_value(table, 1, 0, "Delay Time(s)");
    /*Fill the second column*/
    
    lv_table_set_cell_value(table, 0, 1, sv_data_);
    lv_table_set_cell_value(table, 1, 1, dt_data_);
}

void lv_ex_page_(void)
{
    /*Create a page*/
    lv_obj_t * page = lv_page_create(lv_scr_act(), NULL);
    lv_obj_set_size(page,320, 240);
    lv_obj_align(page, NULL, LV_ALIGN_CENTER, 0, 0);

    uni_lab = lv_label_create(page, NULL);
    lv_label_set_text(uni_lab, dt_data_);
    lv_obj_set_event_cb(uni_lab, lab_event);
    lv_obj_align(uni_lab, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *_btn_; lv_obj_t *lab_btn;
    Create_btn(_btn_, lab_btn, page, "Stop/Close", 3, 80, 160);
}




