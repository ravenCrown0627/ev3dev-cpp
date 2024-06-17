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
        # self.inputs = {
        #     "x": 0,
        #     "y": 0,
        #     # 1 is manhattan distance, 2 is euclidean distance
        #     "mode": 1
        # }
        # self.input_mode = "x" # set the initial input mode to x
        # self.input_mode_list = ["x", "y", "mode"]
        # Set 1 as initial input value
        self.value = 1

        #initialize the button
        self.button.on_up = self.increase
        self.button.on_down = self.decrease
           
    # a method to print text on the display
    def print_text(self, text:str, x0: int, y0: int):
        self.display.text_pixels(text, clear_screen=False, x=x0, y=y0,
                                text_color='black', font=fonts.load('ncenB10'))
        
    def refresh_display(self):
        self.display.clear()

        self.print_text("Please enter an exit: ", 0, 64)
        self.print_text(str(self.value), 168, 64)
        self.display.update()
        pass

        # if self.input_mode == "x":
        #     self.print_text("Please enter x: ", 0, 64)
        # elif self.input_mode == "y":
        #     self.print_text("Please enter y: ", 0, 64)
        # elif self.input_mode == "mode":
        #     self.print_text("Please enter mode: ", 0, 64)
        #     self.print_text("0: Manhattan", 0, 74)
        #     self.print_text("1: Euclidean", 0, 84)

    def increase(self, state):
        if not state:
            self.value += 1
            # Only allow input from 1-5
            self.value = self.value % 6
            self.refresh_display()

            # self.value += 1
            # if self.input_mode == "x":
            #     # x from 0-4
            #     self.value = self.value % 5
            # elif self.input_mode == "y":
            #     # y from 0-3
            #     self.value = self.value % 4
            # elif self.input_mode == "mode":
            #     # mode from 0-1
            #     self.value = self.value % 2                     
        return

    def decrease(self, state):
        if not state:
            self.value -= 1
            # Only allow input from 1-5
            self.value = self.value % 6
            self.refresh_display()

            # if self.input_mode == "x":
            #     # x from 0-4
            #     self.value = self.value % 5
            # elif self.input_mode == "y":
            #     # y from 0-3
            #     self.value = self.value % 4
            # elif self.input_mode == "mode":
            #     # mode from 0-1
            #     self.value = self.value % 2
        return
    



if __name__ == "__main__":
    
    # Request a input
    menu = InputManager()
    menu.print_text("Compile Done!", 0, 0)
    sleep(2)    

    # Save the input
    while not menu.button.enter: 
        menu.button.procee()
        sleep(0.01)
    menu.print_text("Input Done!", 0, 0)
    menu.print_text("Robot will leave at exit {}".format(menu.value), 0, 10)
    sleep(1)

    # # Call command to move robot
    # command = 
    # os.system(command)