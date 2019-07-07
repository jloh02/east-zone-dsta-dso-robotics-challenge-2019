#ifndef _FUNC_HPP_
#define _FUNC_HPP_

#include "main.h"
#include <set>

extern bool cubeInFClaw;
extern set<int> sorted;
void armControl(void * ignore);
void setArm(int posNum);
int getNearestPos();
int getColor(ADIDigitalOut drop_piston);

void dropCube(ADIDigitalOut drop_piston, ADIDigitalOut offset_piston);
void pickupCube(ADIDigitalOut drop_piston, ADIDigitalOut offset_piston);

#endif
