#ifndef SUMO_HPP
#define SUMO_HPP

#include <iostream>
#include <thread>
#include <chrono>
#include "ev3dev.h"

using namespace std;
using namespace ev3dev;

class Sumo {
public:
    enum state {
        state_idle,
        state_intial,
        state_navigate,
        state_attack,
        state_border_detected,
        state_move,
        state_turning,
        exit
    };

    // Peripheral devices declaration
    /* Motor */
    large_motor       _motor_left;
    large_motor       _motor_right;
    medium_motor      _medium_motor;
    /* Sensor */
    color_sensor      _color_s;
    touch_sensor      _touch_s;
    ultrasonic_sensor _ultrasonic_s;
    state             _state;

    // Constant
    const int NAV_TURNING_DUTY_CYCLE = 40;
    const int ATTACK_FORWARD_DUTY_CYCLE = 90;
    const int OBJ_DISTANCE = 30;    // 30 cm
    const int BORDER_AVOID_DUTY_CYCLE = 90;

    Sumo() : 
        _motor_left(OUTPUT_A),
        _motor_right(OUTPUT_D),
        _medium_motor(OUTPUT_B),
        _color_s(INPUT_1),
        // _touch_s(INPUT_2),
        _ultrasonic_s(INPUT_3),
        _state(state_idle) {};
    ~Sumo() {
        stop();
        this_thread::sleep_for(chrono::milliseconds(100));
        reset();
    }

    void run_state_machine();
private:
    // Main function used in state
    state initial_action();
    state navigate(int duty_cycle);
    state attack(int duty_cycle);
    state border_detected(int duty_cycle);

    // Utility functions
    void drive(int speed, int time = 0);
    void brake(int brake_time = 100);
    void stop();
    void reset();
    int move_in_centimeter(int speed, int distance);
    void turn(int direction, int speed);

    void turn_at_own_axis(int duty_cycle = 40);

    // Check the connection status of the motors
    bool initialized() const { 
        return (_motor_left.connected() && _motor_right.connected());
    };
};

#endif