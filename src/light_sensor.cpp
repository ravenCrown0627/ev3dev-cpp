#include "../lib/ev3dev.h"

#include <iostream>

using namespace ev3dev;
using namespace std;

class color
{
public:
    color();
    int cal_reflectivity();
    int cal_ambient();
    int detect_black();

protected:
    color_sensor    _color_sensor;
};

// Constructor 
// constexpr char INPUT_1[]  = "ev3-ports:in1";  //!< Sensor port 1
// constexpr char INPUT_2[]  = "ev3-ports:in2";  //!< Sensor port 2
// constexpr char INPUT_3[]  = "ev3-ports:in3";  //!< Sensor port 3
// constexpr char INPUT_4[]  = "ev3-ports:in4";  //!< Sensor port 4
color::color() :
  _color_sensor(INPUT_1)
{
}


int color::cal_reflectivity()
{
    return _color_sensor.reflected_light_intensity();
}

int color::cal_ambient()
{
    return _color_sensor.ambient_light_intensity();
}

int color::detect_black()
{
    return _color_sensor.color();
}


int main()
{
    color color1;
    while(true)
    {
        cout << "Reflectivity: " << color1.cal_reflectivity() << endl;
        cout << "Ambient: " << color1.cal_ambient() << endl;
        cout << "Color: " << color1.detect_black() << endl;
        //this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}