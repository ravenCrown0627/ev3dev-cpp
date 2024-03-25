#include "../lib/ev3dev.h"

#include <iostream>

using namespace ev3dev;
using namespace std;

int main() {
    large_motor motor_left(OUTPUT_B);
    large_motor motor_right(OUTPUT_C);
    gyro_sensor gyro(INPUT_4);

    

    gyro.set_mode("GYRO-ANG");
    motor_left.reset();
    motor_right.reset();

    while(gyro.angle() < 80) {
        motor_left.set_speed_sp(200);
        motor_right.set_speed_sp(-200);
        motor_left.run_forever();
        motor_right.run_forever();
    }
    // while(true) {
    //     cout << "Angle: " << gyro.angle() << "\n";
    // }
    
    return 0;
}