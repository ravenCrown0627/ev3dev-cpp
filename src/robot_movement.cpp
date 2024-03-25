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
  void turn(int direction);
  void stop();
  void reset();

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
  stop();
  reset();
}

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

void control::turn(int direction)
{
  if (_state != state_idle)
    stop();

  if (direction == 0)
    return;

  _state = state_turning;

  _motor_left. set_position_sp( direction).set_speed_sp(500).run_to_rel_pos();
  _motor_right.set_position_sp(-direction).set_speed_sp(500).run_to_rel_pos();

  while (_motor_left.state().count("running") || _motor_right.state().count("running"))
    this_thread::sleep_for(chrono::milliseconds(10));

  _state = state_idle;
}

void control::stop()
{
  _motor_left .set_stop_action("hold").stop_action();
  _motor_right.set_stop_action("hold").stop_action();

  _state = state_idle;
}

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

int main()
{
  control robot_ctrl;

  if (!robot_ctrl.initialized())
    return -1;
  
  // Calculate the time it takes to drive x meter
  robot_ctrl.drive(180);

  return 0;
}