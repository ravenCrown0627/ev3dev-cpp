#!/usr/bin/env python3
import os
from time import sleep

from ev3dev2.display import Display
import ev3dev2.fonts as fonts
from ev3dev2.button import Button

class InputManager:
    def __init__(self):   
        self.display = Display()
        self.button = Button()
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

    def increase(self, state):
        if not state:
            self.value += 1
            # Only allow input from 1-5
            if self.value > 5: 
                self.value = 1
            if self.value < 1: 
                self.value = 5
            self.refresh_display()
                    
        return

    def decrease(self, state):
        if not state:
            self.value -= 1
            # Only allow input from 1-5
            if self.value > 5: 
                self.value = 1
            if self.value < 1: 
                self.value = 5
            self.refresh_display()

        return
    



if __name__ == "__main__":
    
    # Request a input
    menu = InputManager()
    menu.print_text("Compile Done!", 0, 0)
    menu.display.update()
    sleep(2)    

    # Save the input
    while not menu.button.enter: 
        menu.refresh_display()
        menu.button.process()
        sleep(0.01)
    exit = menu.value
    menu.display.clear()
    menu.print_text("Input Done!", 0, 0)
    menu.print_text("Robot will leave at exit {}".format(exit), 0, 10)
    menu.display.update()
    sleep(1)

    # # Call command to move robot
    command = "./test_ultrasonic {}".format(exit)
    os.system(command) 