#include "sumo.hpp"

using namespace std;

void Sumo::run_state_machine() {
    while (true) {
        switch (_state) {
            // do nothing
            case state_idle:
                _state = state_intial;
                break;
            // initialize the robot
            case state_intial:
                _state = initial_action();
                break;
            // navigate the robot
            case state_navigate:
                _state = navigate(NAV_TURNING_DUTY_CYCLE);
                break;
            // attack the enemy
            case state_attack:
                _state = attack(ATTACK_FORWARD_DUTY_CYCLE);
                break;
            // border detected
            case state_border_detected:
                // Currently no action
                _state = border_detected(BORDER_AVOID_DUTY_CYCLE);
                break;
            case exit:
                return;
            default:
                break;
        }
    }
}

// Robot take intial action
Sumo::state Sumo::initial_action() {
    _state = state_intial;

    // Reset the motor position
    _motor_left .set_position_sp(0);
    _motor_right.set_position_sp(0);
    _medium_motor.set_position_sp(0);

    // Run the motors to the relative position
    _motor_left. set_position_sp( 360).set_speed_sp(80).run_to_rel_pos();
    _motor_right.set_position_sp(-360).set_speed_sp(80).run_to_rel_pos();

    // While the robot is turning
    while (_motor_left.state().count("running") || 
                _motor_right.state().count("running")) {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    brake();
    // Push down the medium motor
    _medium_motor.set_position_sp(720).set_speed_sp(1000).run_to_rel_pos();
    // Stop the medium motor
    _medium_motor.stop();
    // This need to do experiment to get the optimal speed and time to 
    // arrive nearby the center of the arena
    drive(-350, 1000);

    return state_navigate;
}

// Robot navigate enemy by turning to right at the duty_cycle
Sumo::state Sumo::navigate(int duty_cycle) {
    // Change the state to turn
    _state = state_navigate;

    // Calculate turn speed
    int  speed = _motor_left.max_speed() * duty_cycle / 100;

    if (_state != state_idle) brake();

    // Reset the motor position
    _motor_left .set_position_sp(0);
    _motor_right.set_position_sp(0);

    // Run the motors to the relative position
    while (true) {
        _motor_left. set_position_sp(-360).set_speed_sp(speed).run_to_rel_pos();
        _motor_right.set_position_sp(360).set_speed_sp(speed).run_to_rel_pos();

        while (_motor_left.state().count("running") || 
                    _motor_right.state().count("running")) {
            // Detected enemy
            if (_ultrasonic_s.distance_centimeters() < OBJ_DISTANCE) {
                brake();
                // Change the state to attack
                return state_attack;
            } else if (_color_s.reflected_light_intensity() <= 6) {
                brake();
                // Change the state to attack
                return state_border_detected;
            }

            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
}

// Robot attack the enemy by moving forward with the duty_cycle
// until the robot is detected the border
Sumo::state Sumo::attack(int duty_cycle) {
    bool state = true;

    if (_state != state_idle)
        brake();

    // move forward with the given duty_cycle
    while (state)
    {
        _motor_left.set_duty_cycle_sp(-duty_cycle).run_direct();
        _motor_right.set_duty_cycle_sp(-duty_cycle).run_direct();

        // if the robot is outside the line
        if(_color_s.reflected_light_intensity() <= 6) {
            brake();
            state = false;
        }
    }

    return state_border_detected;
}

Sumo::state Sumo::border_detected(int duty_cycle) {
    if (_state != state_idle)
        brake();

    _motor_left.set_duty_cycle_sp(duty_cycle).run_direct();
    _motor_right.set_duty_cycle_sp(duty_cycle).run_direct();

    // the 500 need to be experiment to get the optimal time to move backward
    this_thread::sleep_for(chrono::milliseconds(500));

    // move backward with the given duty_cycle
    while (_touch_s.is_pressed()) {
        _motor_left.set_duty_cycle_sp(duty_cycle).run_direct();
        _motor_right.set_duty_cycle_sp(duty_cycle).run_direct();
    }

    return state_navigate;   
}

// Drive the robot forward or backward with certain time
// The unit of speed is in tacho/sec, +ve value is forward, -ve value is backward
// The unit of time is in milliseconds
void Sumo::drive(int speed, int time) {
    _state = state_move;
    _motor_left.set_speed_sp(speed);
    _motor_right.set_speed_sp(speed);
    _motor_right.run_direct();

    if (time > 0) {
        _motor_left .set_time_sp(time).run_timed();
        _motor_right.set_time_sp(time).run_timed();

        while (_motor_left.state().count("running") || 
                    _motor_right.state().count("running"))
            this_thread::sleep_for(chrono::milliseconds(10));

        _state = state_idle;
    } else {
        _motor_left.run_forever();
        _motor_right.run_forever();
    }
}

// Use to turn the robot only
void Sumo::turn(int direction, int speed) {
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
void Sumo::stop() {
  _motor_left .set_stop_action("hold").stop();
  _motor_right.set_stop_action("hold").stop();

  _state = state_idle;
}

// Reduce the speed of the motor followed by a brake
void Sumo::brake(int brake_time) {
  // Set the motors to run in the opposite direction at the brake speed
  _motor_left.set_speed_sp(-_motor_left.time_sp()/32).run_forever();
  _motor_right.set_speed_sp(-_motor_right.time_sp()/32).run_forever();

  this_thread::sleep_for(chrono::milliseconds(brake_time));
  stop();
}

// Reset the status of the motor to the default parameter
void Sumo::reset() {
  if (_motor_left.connected())
    _motor_left.reset();

  if (_motor_right.connected())
    _motor_right.reset();

  _state = exit;
}