#include "../lib/ev3dev.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace ev3dev;
using namespace std;

enum state
{
  state_idle,
  state_driving,
  state_turning
};

class control
{
public:
  control();
  ~control();

  void drive(int speed, int time=0);
  void turn(int direction, int speed=0);
  void stop();
  void reset();
  int move_in_centimeter(int speed, int distance);

  bool initialized() const;
  void terminate() { _terminate = true; }


  state _state;

protected:
  large_motor     _motor_left;
  large_motor     _motor_right;
  bool            _terminate;
};

// Constructor
// ev3dev.h:L86
// constexpr char OUTPUT_A[] = "ev3-ports:outA"; //!< Motor port A
// constexpr char OUTPUT_B[] = "ev3-ports:outB"; //!< Motor port B
// constexpr char OUTPUT_C[] = "ev3-ports:outC"; //!< Motor port C
// constexpr char OUTPUT_D[] = "ev3-ports:outD"; //!< Motor port D
control::control() :
  _motor_left(OUTPUT_B),
  _motor_right(OUTPUT_C),
  _state(state_idle),
  _terminate(false)
{
}

// Destructor
control::~control()
{
  reset();
}

// Drive the robot forward or backward
// The unit of speed is in tacho/sec, +ve value is forward, -ve value is backward
// The unit of time is in milliseconds
void control::drive(int speed, int time)
{
  _motor_left.set_speed_sp(speed);
  _motor_right.set_speed_sp(speed);

  _state = state_driving;

  if (time > 0)
  {
    _motor_left .set_time_sp(time).run_timed();
    _motor_right.set_time_sp(time).run_timed();

    while (_motor_left.state().count("running") || 
              _motor_right.state().count("running"))
      this_thread::sleep_for(chrono::milliseconds(10));

    _state = state_idle;
  }
  else
  {
    _motor_left.run_forever();
    _motor_right.run_forever();
  }
}

// Turn the robot either left or right
// The unit of direction is in degree, +ve value is right, -ve value is left
// The unit of speed is in tacho/sec
//
// Formula to calculate the direction requried:
//    R_wheel * Theta_wheel = R_robot * Theta_robot
//    R_wheel     : Radius of the wheel
//    Theta_wheel : Angle of the wheel needed to turn
//    R_robot     : Radius of the robot (half of the distance between the two wheels)
//    Theta_robot : Angle of the robot needed to turn
void control::turn(int direction, int speed)
{
  if (_state != state_idle)
    stop();

  if (direction == 0)
    return;

  _state = state_turning;

  // Reset the motor position
  _motor_left .set_position_sp(0);
  _motor_right.set_position_sp(0);

  // Run the motors to the relative position
  _motor_left. set_position_sp( direction).set_speed_sp(speed).run_to_rel_pos();
  _motor_right.set_position_sp(-direction).set_speed_sp(speed).run_to_rel_pos();

  while (_motor_left.state().count("running") || _motor_right.state().count("running"))
    this_thread::sleep_for(chrono::milliseconds(10));

  _state = state_idle;
}

// Stop the robot with the hold action
void control::stop()
{
  _motor_left .set_stop_action("hold").stop_action();
  _motor_right.set_stop_action("hold").stop_action();

  _state = state_idle;
}

// Reset the status of the motor to the default parameter
void control::reset()
{
  if (_motor_left.connected())
    _motor_left.reset();

  if (_motor_right.connected())
    _motor_right.reset();

  _state = state_idle;
}

// Check the connection status of the motors
bool control::initialized() const
{
  return (_motor_left.connected() && _motor_right.connected());
}

// speed is in degrees per second, distance is in cm
int control::move_in_centimeter(int speed, int distance)
{
  double circumference = 17.8; // cm
  double speed_in_cms = speed / 360.0 * circumference;
  
  int time = distance / speed_in_cms * 1000;

  cout <<"\n\n\nTime: " << time << " ms\n";

  return time;
}

int main()
{
  const int speed     = 180;
  const int distance  = 20;

  control robot_ctrl;

  if (!robot_ctrl.initialized())
    return -1;

  int time = robot_ctrl.move_in_centimeter(speed, distance);
  cout << "\n###Time: " << time << " ms" << endl;

  // Calculate the time it takes to drive x meter
  for (int i = 0; i < 4; i++)
  {
    robot_ctrl.drive(speed, time);
    robot_ctrl.turn(191, 300);   // 194 degrees, 300 degrees/sec
  }

  // Allowing the robot to move further a bit to avoid slipping curve
  robot_ctrl.drive(180, 100); // 180 degrees/sec, 0.1 seconds

  robot_ctrl.stop();
  return 0;
}