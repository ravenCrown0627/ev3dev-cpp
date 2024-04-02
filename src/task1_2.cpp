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

  // move in a square
  for(int i = 0; i < 4; i++)
  {
    robot_control.move_in_centimeter(360, 30);
    this_thread::sleep_for(chrono::milliseconds(500));
    robot_control.turn(90, 360);
    robot_control.brake();
    this_thread::sleep_for(chrono::milliseconds(400));
  }
  // robot_control.move_in_centimeter(360, 5);

  return 0;
}