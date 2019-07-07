# East Zone DSTA-DSO Robotics Challenge 2019

Contributers: Jonathan Loh (8059A), Aloysius Wong (8059X)  
Robot built by: Marcus Wong (8059A), Jaren Chen (8059A), Ng Zhi Kang (8059A)

Challenge Site can be found at: https://sites.google.com/vjc.sg/ez-robotics-challenge-2019/home

## Robot Specifications
* 2 motor 1 side base connected to end of a joint from the center of the circle (Think circular motion)
* Double claw mechanism
* Piston to shift double claw mechanism towards and away from center of circle
* 2 Piston to drop mechanism onto cube
* 2 Vision sensor

## Code
* `initialize.cpp`: Perpetual while loop to wait for start button
* `func.cpp`
  * `encdPerDeg`: Specifies a scaling to convert average encoder counts on base to degress of arm turned
  * `kP`, `kD`: Specifies PD constants for base
  * `armControl`: Task to control base
  * `setArm`: Set target for base to move to
  * `getNearestPos`: Used when blocks have reached 1 cycle and not all have been sorted. Uses a set to determine which blocks have not been sorted and returns position of the nearest block 
  * `getColor`: Based on cubeInFClaw, determine which vision sensor to use (ie the opposite of cubeInFClaw) and return the position where the cube is to be placed
  * `dropCube`, `pickupCube`: Does what it says
* `display.cpp`: All code to display a timer, start button and log files
* `autonomous.cpp`: Driver program
