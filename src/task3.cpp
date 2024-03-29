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
  int DUTY_CYCLE = 90;

  color_sensor    color1(INPUT_1);
  control         robot_control;

  if (!robot_control.initialized())
  {
    cerr << "Motor is not initialized" << endl;
    this_thread::sleep_for(chrono::milliseconds(3000));
    return -1;
  }
  
  // initiliaze the color sensor to ensure the reading is valid
  color1.reflected_light_intensity();

  this_thread::sleep_for(chrono::milliseconds(3000));
  // robot_control._motor_right.run_direct();
  
  // tested 1/2 3/4 4/5 7/8
  robot_control._motor_left.set_duty_cycle_sp(DUTY_CYCLE*9/10).run_direct();
  robot_control._motor_right.set_duty_cycle_sp(DUTY_CYCLE).run_direct();
  robot_control._motor_left.set_duty_cycle_sp(DUTY_CYCLE);

  while(true)
  {
    // if(color1.color() == 1)
    /*
    use reflectivity to set the threshold
    */
    if(color1.reflected_light_intensity() < 5)
    {
      robot_control.brake();
      break;
    }
    this_thread::sleep_for(chrono::milliseconds(10));
  }

  return 0;
}