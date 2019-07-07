#include <string>
#include <fstream>
#include "display.hpp"
#include "main.h"
#include <vector>

lv_obj_t * time_label;
static lv_style_t time_style;
lv_obj_t * logText;
double startTime;
bool timerEnd = false;
vector<string> db_log;
bool startProg = false;

void endTimer(){
  timerEnd = true;
  plog("Time elapsed: "+to_string(millis()-startTime));
}

void plog(string txt){
  db_log.push_back(txt);
  if(db_log.size()>8){
    db_log.erase(db_log.begin());
  }
  string output = "";
  for(string x:db_log){
    output+=x+"\n";
  }
  printf("%s\n",txt.c_str());
  lv_label_set_text(logText, output.c_str());
}

void timer(void * ignore){
  while(true){
    lv_label_set_text(time_label, to_string(((int)(millis()-startTime))).c_str());
    delay(25);
    if(millis()-startTime >= 180000 || timerEnd) break;
  }
  if(!timerEnd)
    lv_label_set_text(time_label, "180000");
}

static lv_res_t start(lv_obj_t * btn){
  startTime = millis();
  timerEnd = false;
  Task stopwatch(timer);
  startProg = true;
  return LV_RES_OK;
}

void initDisp(){
  int btnm_height = (LV_VER_RES-130)/ 2;

  /*Create a title label*/
	lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(title, "Time Elapsed: ");

	lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, -100, 15);
  static lv_style_t title_style;
  lv_style_copy(&title_style, &lv_style_plain);
  title_style.text.color = LV_COLOR_MAKE(0xFF,0xFF,0xFF);
  lv_label_set_style(title, &title_style);

  time_label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(time_label, "0");
  lv_style_copy(&time_style, &lv_style_plain);
  time_style.text.color = LV_COLOR_MAKE(0x00,0xFF,0x00);
  lv_label_set_style(time_label, &time_style);
  lv_obj_align(time_label, title, LV_ALIGN_IN_RIGHT_MID, 20, 0);

  lv_obj_t * start_btn = lv_btn_create(lv_scr_act(), NULL);
  static lv_style_t start_btn_style;
  lv_style_copy(&start_btn_style, &lv_style_btn_rel);
  start_btn_style.body.main_color = LV_COLOR_MAKE(0xFF, 0x69, 0xB4);
  start_btn_style.body.grad_color = LV_COLOR_MAKE(0xFF, 0x69, 0xB4);
  start_btn_style.body.border.width = 0;
  start_btn_style.body.border.opa = LV_OPA_50;
  start_btn_style.body.radius = 10;
  start_btn_style.text.color = LV_COLOR_MAKE(0x00,0x00,0x00);
  lv_btn_set_style(start_btn, LV_BTN_STYLE_REL, &start_btn_style);
  static lv_style_t start_btn_style_pr;
  lv_style_copy(&start_btn_style_pr, &start_btn_style);
  start_btn_style.body.main_color = LV_COLOR_MAKE(0xFF, 0x00, 0x7F);
  start_btn_style.body.grad_color = LV_COLOR_MAKE(0xFF, 0x00, 0x7F);
  lv_btn_set_style(start_btn, LV_BTN_STYLE_PR, &start_btn_style_pr);
  lv_btn_set_action(start_btn, LV_BTN_ACTION_CLICK, start);

  lv_obj_align(start_btn, time_label, LV_ALIGN_IN_RIGHT_MID, 220, 25);
  lv_obj_set_size(start_btn, 100, 30);
  lv_obj_t * start_btn_label = lv_label_create(start_btn, NULL);
  lv_label_set_text(start_btn_label, "START");

  logText = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(logText, "");
  lv_obj_align(logText, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 55);

}

void clearScreen(){
  lv_obj_clean(lv_scr_act());
}
