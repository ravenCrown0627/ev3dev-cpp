#include "../lib/ev3dev.h"
#include "../lib/control.h"

using namespace ev3dev;
using namespace std;

/*
Port A = left motor
Port D = right motor
Port 1 = color sensor
*/

int main()
{
  control         robot_control;
  if(!robot_control.initialized())
  {
    cerr << "Motor is not initialized" << endl;
    this_thread::sleep_for(chrono::milliseconds(3000));
    return -1;
  }

  // wait 3 seconds for start
  this_thread::sleep_for(chrono::milliseconds(3000));

  // input speed in tacho/sec
  // input distance in cm
  // move the robot forward
  robot_control.move_in_centimeter(180, 7);

  this_thread::sleep_for(chrono::milliseconds(3000));

  // move the robot backward
  robot_control.move_in_centimeter(-180, 12);

  return 0;
}