#include <iostream>

using namespace std;

int main() {
    double circumference = 17.8;
    double distance;
    double speed = 800;

    cout << "==========================" << endl; 
    cout << "Distance Calculation Tool" << endl;
    cout << "==========================" << endl;

    // cout << "Enter the circumference of a tyre (cm): ";
    // cin >> circumference;
    cout << "Enter the distance of the robot (cm): ";
    cin >> distance;
    // cout << "Enter the speed of robot move (tacho counts per second): ";
    // cin >> speed;

    double speed_in_cms = speed / 360.0 * circumference;
    int time = distance / speed_in_cms * 1000;

    cout << "\n\nThe robot needs to rotate " << time << " s to travel " << distance << " cm." << endl;
}