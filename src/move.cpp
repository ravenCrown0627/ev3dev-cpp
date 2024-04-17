#include "../lib/ev3dev.h"
#include "../lib/control.h"
#include <cstdlib> // for std::atoi
#include <vector>

using namespace ev3dev;
using namespace std;

/*
  Port A = left motor
  Port D = right motor
  Port 1 = color sensor
*/

// The exectuable is expected to receive 6 arguments
// The first argument is the angle (with polarity, in degrees)
// The rest of the arguments are distances (in cm)
int main(int argc, char* argv[])
{
    // Parse arguments
    if(argc < 7) {
        cerr << "Please provide an angle as a command line argument." << endl;
        return -1;
    }

    int ANGLE[3];
    int DISTANCE[3];

    int idx_dist = 0;
    int idx_angle = 0;
    for (int i = 1; i < argc; i++) {
        if (i % 2 == 0) {
            DISTANCE[idx_dist] = atoi(argv[i]);
            idx_dist++;
        } else {
            ANGLE[idx_angle] = atoi(argv[i]);
            idx_angle++;
        }
    }
    // End of argument parsing

    control robot_control;
    
    if(!robot_control.initialized())
    {
        cerr << "Motor is not initialized" << endl;
        this_thread::sleep_for(chrono::milliseconds(3000));
        return -1;
    }

    // wait 3 seconds for start
    this_thread::sleep_for(chrono::milliseconds(3000));

    // move accroding to the value of the arguments
    for(int i = 0; i < sizeof(DISTANCE) / sizeof(int); i++) {
        robot_control.turn(ANGLE[i] * 2, 360);
        robot_control.brake();
        this_thread::sleep_for(chrono::milliseconds(400));
        robot_control.move_in_centimeter(360, DISTANCE[i]);
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return 0;
}