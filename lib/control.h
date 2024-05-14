// this file contains the declaration of the control class

#include "ev3dev.h"

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
  large_motor       _motor_left;
  large_motor       _motor_right;
  ultrasonic_sensor _ultrasonic_s;
  bool              _terminate;

  // Constructor
  // ev3dev.h:L86
  // constexpr char OUTPUT_A[] = "ev3-ports:outA"; //!< Motor port A
  // constexpr char OUTPUT_B[] = "ev3-ports:outB"; //!< Motor port B
  // constexpr char OUTPUT_C[] = "ev3-ports:outC"; //!< Motor port C
  // constexpr char OUTPUT_D[] = "ev3-ports:outD"; //!< Motor port D
  control() :
    _motor_left(OUTPUT_A),
    _motor_right(OUTPUT_D),
    _ultrasonic_s(INPUT_2),
    _state(state_idle),
    _terminate(false)
  {
  }

  // Destructor
  ~control()
  {
    // reset();
    stop();
    this_thread::sleep_for(chrono::milliseconds(100));
    reset();
  }

  void drive(int speed, int time=0);
  void turn(int direction, int speed=0);
  void stop();
  void reset();
  int move_in_centimeter(int speed, int distance);

  bool initialized() const;
  void terminate() { _terminate = true; }

  state _state;

  // to be tested
  void brake(int brake_time=100);
  void turn_dc(int direction, int duty_cycle);
};