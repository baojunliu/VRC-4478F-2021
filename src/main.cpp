#include "vex.h"
#include "odometry.h"
#define TOGGLED_ON true
#define TOGGLED_OFF false

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// mLUpper              motor         12              
// mLLower              motor         11              
// mRUpper              motor         20              
// mRLower              motor         19              
// GPS                  gps           16              
// mConveyor            motor         18              
// mLTray               motor         1               
// mArm                 motor         13              
// clawPiston           digital_out   F               
// GYRO                 inertial      17              
// mRTray               motor         14              
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;

template <typename T> 
int sgn(T val) { //SIGNUM
    return (T(0) < val) - (val < T(0));
}

int logDrive(int s) {
  return (s*s) / 100 * sgn(s);
}

competition Competition;

bool clawState = TOGGLED_OFF;

void clawToggle() {
  clawPiston.set(!clawState);
  clawState = !clawState;
}

void pre_auton() {
  vexcodeInit();
  //GPS.startCalibration();
  GYRO.startCalibration();
  while(GPS.isCalibrating() || GYRO.isCalibrating()) {
    wait(100, msec);
  }
  wait(2500, msec);
  
  //moveForward(24, 50);
  //spotTurn(180, 50);
  //move(720, 50);*/
}

void autonomous(void) {
  mArm.setPosition(0, degrees);
  mLTray.setPosition(0, degrees);
  mRTray.setPosition(0, degrees);
  //left side code with awp
  spotTurn(9.5, 100);

  move(-49, 100); 

  clawToggle();
  wait(300, msec);

  move(35, 100);

  clawToggle();
  wait(300, msec);

  move(7, 100);

  spotTurn(280, 100);

  //lowerTilter(false);
  lowerTilterWithValue(false, -540);
  wait(800, msec);
  move(16, 100);

  raiseTilterWithGoal(false);

  move(-18, 100);  
  spinConveyor();
  wait(900, msec);
  mConveyor.stop(coast);


  //finished right side code (with awp)
  /*
  lowerTilter(false);

  move(46, 100);

  raiseTilterWithGoal(false);
  wait(250, msec);
  move(-20, 100);
  
  spotTurn(180, 100);

  move(27, 100);

  lowerTilter(true);

  move(-12, 100);

  spotTurn(90, 100);

  move(18, 80);

  raiseTilterWithGoal(false);
  wait(500, msec);
  spinConveyor();
  move(-18, 100);
  wait(500, msec);
  mConveyor.stop(coast);

  lowerTilter(false);
  */
}

void usercontrol(void) {
  
  Controller1.ButtonA.pressed(clawToggle);
  bool LockDrive=false;
  while(1) {

    int LSpeed = logDrive(Controller1.Axis3.position(percent));
    int RSpeed = logDrive(Controller1.Axis2.position(percent));

    mLUpper.spin(forward, LSpeed, percent);
    mLLower.spin(forward, LSpeed, percent);
    mRUpper.spin(forward, RSpeed, percent);
    mRLower.spin(forward, RSpeed, percent);

    
    if ((LSpeed == 0)&&(LockDrive)){
      mLLower.stop(hold);
      mLUpper.stop(hold);
    }
    else if (LSpeed == 0){
      mLLower.stop(coast);
      mLUpper.stop(coast);
    }

    if((RSpeed == 0)&&(LockDrive)){
      mRLower.stop(hold);
      mRUpper.stop(hold);
    }
    else if (RSpeed == 0){
      mRLower.stop(coast);
      mRUpper.stop(coast);
    }


    //DriveLock
    if(Controller1.ButtonX.pressing())
    {
      LockDrive=true;
    }
    else if(Controller1.ButtonY.pressing())
    {
      LockDrive = false;
    }

    //ring conveyor
    if (Controller1.ButtonL1.pressing()) {
      mConveyor.spin(forward, 180, rpm);
    } else if (Controller1.ButtonL2.pressing()){
      mConveyor.spin(reverse, 180, rpm);
    } else {
      mConveyor.stop(hold);
    }

    //arm
    if (Controller1.ButtonR1.pressing()) {
      mArm.spin(forward, 100, percent);
    } else if (Controller1.ButtonR2.pressing()) {
      mArm.spin(reverse, 100, percent);
    } else {
      mArm.stop(hold);
    }

    //MOGO Tray
    if(Controller1.ButtonUp.pressing()){
      mLTray.spin(forward, 100, percent);
      mRTray.spin(forward, 100, percent);
    } else if (Controller1.ButtonDown.pressing()){
      mLTray.spin(reverse, 100, percent);
      mRTray.spin(reverse, 100, percent);
    } else {
      mLTray.stop(hold);
      mRTray.stop(hold);
    }
    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while(true) {
    wait(100, msec);
  }
}
