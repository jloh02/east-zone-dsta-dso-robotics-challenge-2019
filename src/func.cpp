#include "func.hpp"
#include "main.h"
#include <vector>
#include "display.hpp"

#define debugMode false
#define manualColor false

#define encdPerDeg 7.5923883442265795200030065359478

double armTarget = 0;  //In degrees
int backclaw, frontclaw;

#define kP 4.5
#define kD 6.0

Motor arm1(1,E_MOTOR_GEARSET_18,false,E_MOTOR_ENCODER_DEGREES);
Motor arm2(20,E_MOTOR_GEARSET_18,false,E_MOTOR_ENCODER_DEGREES);
Motor clawF(10,E_MOTOR_GEARSET_18,true,E_MOTOR_ENCODER_DEGREES);
Motor clawB(9,E_MOTOR_GEARSET_18,true,E_MOTOR_ENCODER_DEGREES);

void armControl(void * ignore){
  double oldError = 0;
  double power = 0;
  arm1.tare_position();
  arm2.tare_position();
  while(true){
    double error = (double)(armTarget - (arm1.get_position()+arm2.get_position())/2/encdPerDeg);

    while(error > 180) error -= 360;
    while(error < -180) error += 360;

    double targetPower = error*kP + (error-oldError)*kD;

    if(fabs(targetPower-power) > 8){
      if(targetPower-power > 0) power += 8;
      else power -= 8;
    }
    else power = targetPower;

    oldError = error;

    if(power > 60) power = 60;
    if(power < -60) power = -60;

    if(power !=0 && fabs(power) < 10) printf("%f\n", power);

    arm1.move(power);
    arm2.move(power);
    delay(25);
  }
}

void setArm(int posNum){
  armTarget = (posNum-1) * 45;
  double time = millis();
  if(debugMode) delay(1000);
  else while(fabs((double)(armTarget - (arm1.get_position()+arm2.get_position())/2/encdPerDeg)) > 8 && millis()-time < 3000) {
    delay(25);
  }
  plog("Arm position: "+to_string(armTarget));
}

int getNearestPos(){
  int nearestPos = 0;
  double leastErr = numeric_limits<double>::max();
  for (int i=1;i<=8;i++){
    if(sorted.find(i) == sorted.end()){
      //printf("Assessing %d\n",i);
      double error = (double)((i-1)*45 - armTarget);
      while(error > 180) error -= 360;
      while(error < -180) error += 360;
      if(fabs(error) < leastErr){
        leastErr = error;
        nearestPos = i;
      }
    }
  }
  plog("Chosen: "+to_string(nearestPos));
  return nearestPos;
}

int getColor(){
  if(debugMode||manualColor) {
    int retVal;
    cin >> retVal;
    return retVal;
  }
  if(cubeInFClaw) {
    //while(backclaw == 255) delay(25);
    if(backclaw == 255/* || sorted.find(backclaw) != sorted.end()*/) return 8;
    return backclaw;
  }
  if(frontclaw == 255/* || sorted.find(frontclaw) != sorted.end()*/) return 8;
  return frontclaw;
}

void getLargest (void * ignore){
  Vision vision(8);
  bool leftclaw = true;
  bool doublecube = true;

  while (true){
    vision_object_s_t obj1 = vision.get_by_size(0);
    vision_object_s_t obj2 = vision.get_by_size(1);
    vision_object_s_t obj3 = vision.get_by_size(2);
    // Gets the largest object
    //plog("Vision: " + to_string(obj1.signature));
    //plog("Vision2: " + to_string(obj2.signature));
    // plog("Vision3: " + to_string(obj3.signature));
    // plog("cubeInFClaw: " + to_string(cubeInFClaw));
    // if(cubeInFClaw) plog("Color: " + to_string(backclaw));
    // else plog("Color: " + to_string(frontclaw));
    // plog("Vision Mid: " + to_string(obj1.x_middle_coord));

    //plog("Vision: " + to_string(obj1.signature));

    //doublecube = (obj3.signature != 255 && fabs(obj1.x_middle_coord - obj2.x_middle_coord) > 40);
    //if(doublecube) plog("Aloy sucks");

    if (obj1.x_middle_coord < 140)
    {
      frontclaw = obj1.signature;
    /*  if (doublecube)
        if(obj2.signature == 3 || obj2.signature == 2) backclaw = 7;
        else backclaw = 8;
      else*/
        backclaw = obj2.signature;
    }
    else //if (obj1.x_middle_coord < 315)
    {
      backclaw = obj1.signature;
  /*    if (doublecube)
        if(obj2.signature == 3 || obj2.signature == 2) frontclaw = 7;
        else frontclaw = 8;
      else*/
       frontclaw = obj2.signature;
    }
    delay(25);
  }
}

void dropCube(ADIDigitalOut drop_piston, ADIDigitalOut offset_piston){
  offset_piston.set_value(!cubeInFClaw);
  if(!cubeInFClaw) plog("Pushed to front");
  else plog("Pushed to back");
  delay(800);
  drop_piston.set_value(true);
  delay(700);
  if(cubeInFClaw){
    clawF.move(-80);
    delay(500);
    clawF.move(-5);
    plog("Front Claw Open");
  }
  else{
    clawB.move(-80);
    delay(500);
    clawB.move(-5);
    plog("Back Claw Open");
  }
  drop_piston.set_value(false);
  delay(700);
}

void pickupCube(ADIDigitalOut drop_piston, ADIDigitalOut offset_piston){
  //offset_piston.set_value(cubeInFClaw);
  //if(cubeInFClaw) plog("Pushed to front");
  //else plog("Pushed to back");
  //delay(800);
  drop_piston.set_value(true);
  delay(700);
  if(!cubeInFClaw){
    clawF.move(80);
    delay(500);
    clawF.move(20);
    plog("Front Claw Close");
  }
  else{
    clawB.move(80);
    delay(500);
    clawB.move(20);
    plog("Back Claw Close");
  }
  drop_piston.set_value(false);
  delay(700);
}
