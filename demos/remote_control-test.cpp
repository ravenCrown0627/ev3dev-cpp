/*
 * test program for the ev3dev C++ remote_control class
 *
 * Copyright (c) 2014 - Franz Detro
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "../lib/ev3dev.h"

#include <iostream>


using namespace std;
using namespace ev3dev;

void print_button(const char *name, bool state)
{
  cout << name << " " << (state ? "pressed" : "released") << endl;
}

int main()
{
  remote_control r;

  if (!r.connected())
  {
    cout << "no infrared sensor found!" << endl;
    return 1;
  }

  r.on_red_down  = bind(print_button, "red down",  placeholders::_1);
  r.on_red_up    = bind(print_button, "red up",    placeholders::_1);
  r.on_blue_down = bind(print_button, "blue down", placeholders::_1);
  r.on_blue_up   = bind(print_button, "blue up",   placeholders::_1);

  bool stop = false;
  r.on_beacon = [&] (bool state) { if (state) stop = true; };

  cout << "ensure that channel 1 is selected," << endl
       << "press middle (beacon) button to exit!" << endl << endl;

  while (!stop)
    r.process();

  return 0;
}
