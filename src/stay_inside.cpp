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
  // speed in duty cycle
  int FORWARD_SPEED = 90;
  int TURN_SPEED = 40;

  control         robot;
  color_sensor    color1(INPUT_1);

  if(!robot.initialized())
  {
    cerr << "Motor is not initialized" << endl;
    this_thread::sleep_for(chrono::milliseconds(3000));
    return -1;
  }

  // initiliaze the color sensor to ensure the reading is valid
  color1.reflected_light_intensity();

  // wait 3 seconds for start
  this_thread::sleep_for(chrono::milliseconds(3000));
  
  // randomize the initial direction
  // using time seed
  srand(time(0));
  robot.turn_dc(rand() % 360, TURN_SPEED);

  // loop forever, stay inside the rectangle
  while (true)
  {
    robot._motor_left.set_duty_cycle_sp(FORWARD_SPEED).run_direct();
    robot._motor_right.set_duty_cycle_sp(FORWARD_SPEED).run_direct();

    // if the robot is outside the line
    if(color1.reflected_light_intensity() < 5)
    {
      robot.brake();
      // reverse for a short distance
      robot.move_in_centimeter(-FORWARD_SPEED * 5, 10);
      // turn the robot at a degree between 90 to 270
      robot.turn_dc(rand() % 180 + 90, TURN_SPEED);
    }
  }
}