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
  int const ANGLE=45;

  control         robot_control;
  if(!robot_control.initialized())
  {
    cerr << "Motor is not initialized" << endl;
    this_thread::sleep_for(chrono::milliseconds(3000));
    return -1;
  }

  // wait 3 seconds for start
  this_thread::sleep_for(chrono::milliseconds(3000));

  // move in a square
  for(int i = 0; i < 4; i++)
  {
    robot_control.move_in_centimeter(360, 30);
    this_thread::sleep_for(chrono::milliseconds(500));
    // .turn() takes the double value of the angle
    // for example, if you want to turn 90 degrees, you should pass 180
    // in this demo case, we want to turn 45 degrees
    robot_control.turn(ANGLE*2, 360);
    robot_control.brake();
    this_thread::sleep_for(chrono::milliseconds(400));
  }
  // robot_control.move_in_centimeter(360, 5);

  return 0;
}