#!/usr/bin/env python3
import os
import math
from time import sleep

from ev3dev2.display import Display
import ev3dev2.fonts as fonts
from ev3dev2.button import Button

class InputManager:
    def __init__(self):   
        self.display = Display()
        self.button = Button()
        self.inputs = {
            "x": 0,
            "y": 0,
            # 1 is manhattan distance, 2 is euclidean distance
            "mode": 1
        }
        self.input_mode = "x" # set the initial input mode to x
        self.input_mode_list = ["x", "y", "mode"]
        # sef the value as positive int
        self.value = 0

        #initialize the button
        self.button.on_up = self.increase
        self.button.on_down = self.decrease
           
    # a method to print text on the display
    def print_text(self, text:str, x0: int, y0: int):
        self.display.text_pixels(text, clear_screen=False, x=x0, y=y0,
                                text_color='black', font=fonts.load('ncenB10'))
        
    def refresh_display(self):
        self.display.clear()

        if self.input_mode == "x":
            self.print_text("Please enter x: ", 0, 64)
        elif self.input_mode == "y":
            self.print_text("Please enter y: ", 0, 64)
        elif self.input_mode == "mode":
            self.print_text("Please enter mode: ", 0, 64)
            self.print_text("0: Manhattan", 0, 74)
            self.print_text("1: Euclidean", 0, 84)

        self.print_text(str(self.value), 168, 64)
        self.display.update()
        pass

    def increase(self, state):
        if not state:
            self.value += 1
            if self.input_mode == "x":
                # x from 0-4
                self.value = self.value % 5
            elif self.input_mode == "y":
                # y from 0-3
                self.value = self.value % 4
            elif self.input_mode == "mode":
                # mode from 0-1
                self.value = self.value % 2                     
            self.refresh_display()
        return

    def decrease(self, state):
        if not state:
            self.value -= 1
            if self.input_mode == "x":
                # x from 0-4
                self.value = self.value % 5
            elif self.input_mode == "y":
                # y from 0-3
                self.value = self.value % 4
            elif self.input_mode == "mode":
                # mode from 0-1
                self.value = self.value % 2
            self.refresh_display()
        
        return
    
    def set_inputs(self):
        self.inputs[self.input_mode] = self.value
        self.value = 0

if __name__ == "__main__":
    menu = InputManager()

    menu.print_text("Compile done!", 0, 0)
    sleep(2)

    for menu.input_mode in menu.input_mode_list:
        menu.refresh_display()

        while not menu.button.enter:
            menu.button.process()
            sleep(0.01)
        menu.set_inputs()
        # avoid detect pressed button twice
        sleep(1)
    
    # print the inputs line by line
    # for key, value in menu.inputs.items():
    #     menu.display.clear()
    #     menu.print_text("{}: {}".format(key, value), 0, 0)
    #     menu.display.update()
    #     sleep(2)
    
    SCALE_FACTOR = 30
    # assume initial coordinate is (0,0)
    x, y = 0, 0
    x_target, y_target = menu.inputs["x"], menu.inputs["y"]
    # 0 is manhattan distance, 1 is euclidean distance
    mode = menu.inputs["mode"]

    # initialize the angles and distances lists
    angles = []
    distances = []

    # mode 0, manhattan distance
    # assume initial direction is y-positive
    if mode == 0:
        angles.append(0)
        distances.append((y_target-y)*SCALE_FACTOR)
        # move to the target x
        angles.append(90)
        distances.append((x_target-x)*SCALE_FACTOR)

    elif mode == 1:
        # mode 1, euclidean distance
        angles.append(math.degrees(math.atan((x_target-x)/(y_target-y))))
        distances.append((math.sqrt((y_target-y)**2 + (x_target-x)**2))*SCALE_FACTOR)

    # print the angles and distances line by line
    for i in range(len(angles)):
        menu.display.clear()
        menu.print_text("Angle: {}".format(angles[i]), 0, 0)
        menu.print_text("Distance: {}".format(distances[i]), 0, 10)
        menu.display.update()
        sleep(2)

        command = "./move {} {}".format(angles[i], distances[i])
        os.system(command)
       

    # command = "./move "
    # for i in len(angles):
    #     command += "{} {}".format(angles[i], distances[i])
    # # call the command and stop the python script
    # os.system(command)