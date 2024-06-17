#include "../lib/ev3dev.h"
#include "../lib/sumo.hpp"

using namespace ev3dev;
using namespace std;

int main()
{
    Sumo sumo;

    while (sumo._state != Sumo::exit) {
        sumo.run_state_machine();
    }
    return 0;
}