#include "main.h"
#include "func.hpp"
#include "display.hpp"
#include <set>
#include <vector>

set<int> sorted;
int cycleStart = 0;
bool cubeInFClaw = false; //Ensure pickup using front claw

ADIDigitalOut offset_piston(1, !cubeInFClaw);
ADIDigitalOut drop_piston(2, false);

void autonomous() {
  Task armTask(armControl);
  Task visionTask(getLargest);
  int color = 0, prevColor;

  start:
    bool firstPick = true;
    int nearestPos = getNearestPos();
    setArm(nearestPos);
    cycleStart = nearestPos;
    //printf("Inserting %d\n",nearestPos);
  colorCheck:
    if(color == cycleStart){
      dropCube(drop_piston, offset_piston);
      sorted.insert(color);
      if(sorted.size() == 7) goto endProg;
      goto start;
    }
    else{
      prevColor = color;
      color = getColor();

      if(nearestPos == color && firstPick) {
        sorted.insert(color);
        if(sorted.size() == 7) goto endProg;
        goto start;
      }
      else{
        pickupCube(drop_piston, offset_piston);
        if(firstPick) goto skipDrop;
        dropCube(drop_piston, offset_piston);
        sorted.insert(prevColor);
        if(sorted.size() == 7) goto endProg;

        skipDrop:
          cubeInFClaw = !cubeInFClaw;
          firstPick = false;
          setArm(color);
          goto colorCheck;
      }
    }

  endProg:
    endTimer();
}
