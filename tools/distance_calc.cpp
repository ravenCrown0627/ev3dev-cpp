#include <iostream>

using namespace std;

int main() {
    double circumference, distance;

    cout << "==========================" << endl; 
    cout << "Distance Calculation Tool" << endl;
    cout << "==========================" << endl;

    cout << "Enter the circumference of a tyre (cm): ";
    cin >> circumference;
    cout << "Enter the distance of the robot (cm): ";
    cin >> distance;

    // Calculate the degree of rotation required
    double degree = (distance * 360) / circumference;

    cout << "\n\nThe robot needs to rotate " << degree << " degrees to travel " << distance << " cm." << endl;
}