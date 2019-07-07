#include "main.h"
#include "display.hpp"

void initialize() {
  initDisp();
  while(true){
    if(startProg) break;
    //printf("waiting\n");
    delay(25);
  }
  autonomous();
}

void disabled() {}

void competition_initialize() {}
