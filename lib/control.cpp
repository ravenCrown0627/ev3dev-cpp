#include "control.h"

// Drive the robot forward or backward
// The unit of speed is in tacho/sec, +ve value is forward, -ve value is backward
// The unit of time is in milliseconds
void control::drive(int speed, int time)
{
    _motor_left.set_speed_sp(speed);
    _motor_right.set_speed_sp(speed);

    _state = state_driving;
    _motor_right.run_direct();

    if (time > 0 && speed > 0) {
        _motor_left .set_time_sp(time).run_timed();
        _motor_right.set_time_sp(time).run_timed();

        bool is_obstacle = true;
        int obstacle_distance_idx = 0;
        int no_obstacle_distance_idx = 0;

        // Get a timestamp of the current time
        auto start = chrono::system_clock::now();
        while (_motor_left.state().count("running") || _motor_right.state().count("running")) {
            if (_ultrasonic_s.distance_centimeters() > 30) {
                if (is_obstacle) {
                    // Get a timestamp of the current time
                    auto end = chrono::system_clock::now();
                    chrono::duration<double> elapsed_seconds = end - start;
                    // Count the time difference between the time elapsed and the time set
                    int time_diff = time - elapsed_seconds.count() * 1000 - (obstacle_distance_idx * 100);
                    _obstacle_distance[obstacle_distance_idx] = elapsed_seconds.count() * 1000;
                    is_obstacle = false;
                    _number_of_exit++;
                    sound::beep("", true);
                    stop();
                    _motor_left .set_time_sp(time_diff).run_timed();
                    _motor_right.set_time_sp(time_diff).run_timed();
                }
            } else {
                if (!is_obstacle) {
                    is_obstacle = true;
                    auto end = chrono::system_clock::now();
                    chrono::duration<double> elapsed_seconds = end - start;
                    int time_diff = _obstacle_distance[obstacle_distance_idx] - (time - elapsed_seconds.count() * 1000);
                    _no_obstacle_distance[no_obstacle_distance_idx] = elapsed_seconds.count() * 1000;
                    // Change the pointer to point to next element in the array
                    obstacle_distance_idx++;
                    no_obstacle_distance_idx++;
                }
            }

            this_thread::sleep_for(chrono::milliseconds(100));
        }

        _state = state_idle;
    } else if (time > 0 && speed < 0) {
        _motor_left .set_time_sp(time).run_timed();
        _motor_right.set_time_sp(time).run_timed();

        while (_motor_left.state().count("running") || _motor_right.state().count("running"))
            this_thread::sleep_for(chrono::milliseconds(10));

        _state = state_idle;
    } else {
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
    
    direction *= 2;
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
  _motor_left .set_stop_action("hold").stop();
  _motor_right.set_stop_action("hold").stop();

  this_thread::sleep_for(chrono::milliseconds(100));

//   _state = state_idle;
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
  int abs_speed;

  if (speed < 0) {
    abs_speed = -speed;
  } else {
    abs_speed = speed;
  }

  double circumference = 17.8; // cm
  double speed_in_cms = abs_speed / 360.0 * circumference;
  int time = distance / speed_in_cms * 1000;
  _total_time = time;
  
  control::drive(speed, time);
}

void control::brake(int brake_time)
{
  // Set the motors to run in the opposite direction at the brake speed
  _motor_left.set_speed_sp(-_motor_left.time_sp()/32).run_forever();
  _motor_right.set_speed_sp(-_motor_right.time_sp()/32).run_forever();

  this_thread::sleep_for(chrono::milliseconds(brake_time));

  stop();

  _state = state_idle;
}

void control::turn_dc(int direction, int duty_cycle)
{
  if (_state != state_idle)
    stop();

  if (direction == 0)
    return;

  direction *= 2;

  _state = state_turning;

  // Calculate turn speed
  int speed = _motor_left.max_speed() * duty_cycle / 100;

  // Reset the motor position
  _motor_left .set_position_sp(0);
  _motor_right.set_position_sp(0);

  // Run the motors to the relative position
  _motor_left. set_position_sp(-direction).set_speed_sp(speed).run_to_rel_pos();
  _motor_right.set_position_sp( direction).set_speed_sp(speed).run_to_rel_pos();

  while (_motor_left.state().count("running") || _motor_right.state().count("running")) {
    if (_ultrasonic_s.distance_centimeters() < 30) {
      brake();
      return;
    }
        
    this_thread::sleep_for(chrono::milliseconds(10));
  }

  _state = state_idle;
}

void control::push_down()
{
  _medium_motor.set_speed_sp(1000).run_forever();
  this_thread::sleep_for(chrono::milliseconds(500));
  _medium_motor.stop();
}