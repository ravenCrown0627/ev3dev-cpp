#!/usr/bin/env python3
from time import sleep
import math
import os

from ev3dev2.display import Display
import ev3dev2.fonts as fonts
from ev3dev2.button import Button

# Creat a class to handle the display
class InputManager:
    def __init__(self, input_type:str):
        self.display = Display()  # Create a display object from ev3dev2 lib
        self.button = Button()  # Create a button object from ev3dev2 lib
        self.sign = "+"
        self.digit = 1
        self.digit_value = [0,0,0] 
        self.input = 0
        self.input_type = input_type


    # Set the location and format for printing text
    def print_text(self, text:str, x0: int, y0: int):
        self.display.text_pixels(text, clear_screen=False, x=x0, y=y0,
                                text_color='black', font=fonts.load('ncenB10'))


    # Refresh the content of display 
    def refresh_display(self):
        self.display.clear()

        # Print the request for the user to enter the angle or distance
        if self.input_type == "angle":
            self.print_text("Please enter A{}: ".format(request), 0, 64)
        else:
            self.print_text("Please enter D{}: ".format(request), 0, 64)
        
        # Print the sign
        self.print_text(self.sign, 138, 64)
        # Print the number 
        for i in range(3):
            self.print_text(str(self.digit_value[i]), 168 - 10 * i, 64)
        # Print the cursor
        self.print_text("^", 168 - 10 * int(math.log10(self.digit)), 74)    
        self.display.update()
    

    # Calculate the angle or distance based on input values
    def calculate(self):
        if self.input_type == "angle":
            if self.sign == "+":
                return self.digit_value[0] * 1 + self.digit_value[1] * 10 + self.digit_value[2] * 100
            else:
                return -1 * (self.digit_value[0] * 1 + self.digit_value[1] * 10 + self.digit_value[2] * 100)
        else:
            return self.digit_value[0] * 1 + self.digit_value[1] * 10 + self.digit_value[2] * 100


    # Change digit 
    def change_digit(self, state):
        if not state:
            # digit = 1, 10, 100, 1000 (digits, tens, hundreds, sign)
            self.digit *= 10
            if self.digit > 1000:
                self.digit = 1
            self.refresh_display()
        
        return True


    # Plus number or change sign
    def increment(self, state):
        if not state:
            # Change the number on the chosen digit 
            if self.digit < 1000:
                # Set the limit of the input for angle or distance
                limit = int(181) if self.input_type == "angle" else int(100000)
                
                # Aviod the user to enter a number greater than 9 for single bit and over limit
                if self.digit_value[int(math.log10(self.digit))] < 9:
                    # Increment to the chosen digit
                    self.digit_value[int(math.log10(self.digit))] += 1
                    if self.digit_value[0] * 1 + self.digit_value[1] * 10 + self.digit_value[2] * 100 >= limit:
                        self.digit_value[int(math.log10(self.digit))] -= 1

                # Calculate the angle or distance
                self.input = self.calculate()
            
            # Change the sign
            else:
                if self.input_type == "angle":
                    self.sign = "-" if self.sign == "+" else "+"
                else:
                    self.sign = "+"
            
            # Refresh the display after changing number or sign
            self.refresh_display()
        
        return True


    # Minus number or change sign
    def decrement(self, state):
        if not state:
            # Change the number on the chosen digit 
            if self.digit < 1000:
                # Aviod the user to enter a number less than 0
                if self.digit_value[int(math.log10(self.digit))] > 0:
                    # Decrement to the chosen digit
                    self.digit_value[int(math.log10(self.digit))] -= 1
                
                # Calculate the angle or distance
                self.input = self.calculate()
            
            # Change the sign
            else:
                if self.input_type == "angle":
                    self.sign = "-" if self.sign == "+" else "+"
                else:
                    self.sign = "+"
            
            # Refresh the display after changing number or sign
            self.refresh_display()
        
        return True


    # Reset the values on each digit
    def clear_value(self, state):
        if not state:
            self.digit_value = [0,0,0]
            self.sign = "+"
            self.digit = 1
            self.input = 0
            self.refresh_display()
        
        return True
    

    # Set the callback for each button
    def set_callbacks(self):
        # Set the callback for each button
        self.button.on_left = self.change_digit
        self.button.on_right = self.clear_value
        self.button.on_up = self.increment
        self.button.on_down = self.decrement


    # Reset all values for next request
    def reset_values(self):
        self.sign = "+"
        self.digit = 1
        self.digit_value = [0,0,0]
        self.input = 0



if __name__ == '__main__':

    # Initialization
    angles = []
    distances = []

    # Create an object of InputManager class
    input_angle = InputManager("angle")
    input_angle.set_callbacks()
    input_distance = InputManager("distance")
    input_distance.set_callbacks()

    # Inform the user that compile is done
    input_angle.print_text("Compile done!", 0, 0)
    input_angle.display.update()
    sleep(2)

    # Repeat the request for three location points (A,D)
    for request in range(3):
        # 1. Request for the angle
        input_angle.refresh_display()
    
        # Wait for the user to enter
        while not input_angle.button.enter:
            input_angle.button.process()
       
        # Store the angle value and print it out
        angles.append(input_angle.input)
        input_angle.display.clear()
        input_angle.print_text("{} degree as A{} is accepted".format(input_angle.input, request), 0, 0)
        input_angle.display.update()
        sleep(2)

        # 2. Request for the distance
        input_distance.refresh_display()

        # Wait for the user to enter
        while not input_distance.button.enter:
            input_distance.button.process()
        
        # Store the distance value and print it out
        distances.append(input_distance.input)
        input_distance.display.clear()
        input_distance.print_text("{} cm as D{} is accepted".format(input_distance.input, request), 0, 0)
        input_distance.display.update()
        sleep(2)

        # Reset all values for the next request
        input_angle.reset_values()
        input_distance.reset_values()
    command = "./move {} {} {} {} {} {}".format(angles[0], distances[0], angles[1], distances[1], angles[2], distances[2]) 
    os.system(command)
