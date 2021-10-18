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
// clawPiston           digital_out   A               
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
  wait(3000, msec);
  //turnMoveToPoint(0, 0, 0);
  spotTurn(180, 100);
}

void autonomous(void) {

}

void usercontrol(void) {
  Controller1.ButtonA.pressed(clawToggle);
  /*while(1) {

    int LSpeed = logDrive(Controller1.Axis3.position(percent));
    int RSpeed = logDrive(Controller1.Axis2.position(percent));

    mLUpper.spin(forward, LSpeed, percent);
    mLLower.spin(forward, LSpeed, percent);
    mRUpper.spin(forward, RSpeed, percent);
    mRLower.spin(forward, RSpeed, percent);

    if (LSpeed == 0){
      mLLower.stop(hold);
      mLUpper.stop(hold);
    }
    if(RSpeed == 0){
      mRLower.stop(hold);
      mRUpper.stop(hold);
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
  }*/
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while(true) {
    wait(100, msec);
  }
}
