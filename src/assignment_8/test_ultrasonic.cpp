#include "../lib/ev3dev.h"
#include "../lib/control.h"

#include <iostream>
#include <thread>

using namespace std;
using namespace ev3dev;

int main(int argc, char* argv[]) {
    control         robot;

    if(!robot.initialized()) {
        cerr << "Motor is not initialized" << endl;
        this_thread::sleep_for(chrono::milliseconds(3000));
        return -1;
    }
    
    int time_to_exit;
    int exit_idx        = stoi(argv[1]);

    // Move the robot forward for 2 meter 
    robot.move_in_centimeter(300, 200);
    robot.stop();
    time_to_exit = robot._total_time - robot._obstacle_distance[exit_idx - 1];
    robot.drive(-300, time_to_exit - ((robot._number_of_exit + 1 - exit_idx) * 500));
    robot.stop();
    robot.turn(90, 300);
    robot.stop();
    robot.move_in_centimeter(-300, 50);
    robot.stop();

    return 0;
}