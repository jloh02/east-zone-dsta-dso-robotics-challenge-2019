#include "main.h"
#include "display.hpp"

void initialize() {
  initDisp();
  ADIDigitalOut offset_piston(1, true);
  ADIDigitalOut drop_piston(2, false);

  /*
  bool flip = false;
  while(true){
    offset_piston.set_value(flip);
    flip = !flip;
    drop_piston.set_value(true);
    delay(300);
    drop_piston.set_value(false);
    delay(300);
  }
  */

  while(true){
    if(startProg) break;
    delay(25);
  }
  autonomous();
}

void disabled() {}

void competition_initialize() {}
