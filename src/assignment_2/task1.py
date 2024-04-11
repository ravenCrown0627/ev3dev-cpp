#!/usr/bin/env python3
from time import sleep
import math

from ev3dev2.display import Display
import ev3dev2.fonts as fonts
from ev3dev2.button import Button


def print_text(text:str, x0: int, y0:int):
    """
    @brief  This function will print the text on the display
    @param  text: The text that needs to be printed
    """
    global display

    display.text_pixels(text,clear_screen=False, x=x0, y=y0, 
                        text_color = 'black', font = fonts.load('ncenB10'))
    # display.update()


def refresh_display():
    """
    @brief  This function will refresh the display
    """
    global display, request, number_list, digit, input_type

    display.clear()
    # Print the request for the user to enter the angle or distance
    if input_type == "angle":
        print_text("Please enter an angle A{}: ".format(request), 0, 64)
    else:
        print_text("Please enter an distance D{}: ".format(request), 0, 64)

    # Print the number that the user is entering
    for i in range(3):
        print_text(str(number_list[i]), 168 - 10*i, 64)
    print_text("^", 168 - 10*int(math.log10(digit)), 74)
    display.update()


def up(state):
    """
    @brief  This function will be called when the up button is pressed and change digit
    """
    global digit, number_list, display
    if not state:
        # Change the digit to the next digit
        digit *= 10
        if digit > 100:
            digit = 1

        # Print a mark under the digit that is being changed
        refresh_display()

    return True
    

def right(state):
    """
    @brief  This function will be called when the right button is pressed
    """
    global number_list, digit, angle, display, input_type, distance

    if not state:
        # Set the limit of the angle or distance
        limit = int(361) if input_type == "angle" else int(179)

        # Aviod the user to enter a number greater than 9
        if number_list[int(math.log10(digit))] < 9:
            # Change the number on the chosen digit
            number_list[int(math.log10(digit))] += 1
            # Avoid the user to enter a total number greater than 360
            if number_list[0] * 1 + number_list[1] * 10 + number_list[2] * 100 >= limit:
                number_list[int(math.log10(digit))] -= 1

        # Print the changed number on the display
        refresh_display()
        
        # Calculate the angle or distance
        if input_type == "angle":
            angle = number_list[0] * 1 + number_list[1] * 10 + number_list[2] * 100
        else:
            distance = number_list[0] * 1 + number_list[1] * 10 + number_list[2] * 100

    return True


def left(state):
    """
    @brief  This function will be called when the left button is pressed
    """
    global number_list, digit, angle, display, input_type, distance

    if not state:

        # Aviod the user to enter a number less than 0
        if number_list[int(math.log10(digit))] > 0:
            # Change the number on the chosen digit
            number_list[int(math.log10(digit))] -= 1
        
        # Print the changed number on the display
        refresh_display()

        # Calculate the angle
        if input_type == "angle":
            angle = number_list[0] * 1 + number_list[1] * 10 + number_list[2] * 100
        else:
            distance = number_list[0] * 1 + number_list[1] * 10 + number_list[2] * 100
    
    return True


def down(state):
    """
    @brief  This function will be called when the down button is pressed
    """
    global number_list, angle, digit, distance
    if not state:
        # Reset the number_list and angle
        number_list = [0, 0, 0]
        digit = 1
        angle = 0
        distance = 0
        # Print the changed number on the display
        refresh_display()
    
    return True


if __name__ == '__main__':

    # Create the display and button objects
    display = Display()
    button = Button()

    # Initialize the variable
    input_type = None
    digit = 1 # The digit that the user is entering
    angle = 0
    distance = 0
    number_list = [0, 0, 0] # for both angle and distance
    angle_list = []
    distance_list = []


    # Inform the user that compile is done
    print_text("Compile Done", 0, 0)
    display.update()
    sleep(2)

    # Set the button callbacks for the request of angle and distance
    button.on_up = up
    button.on_right = right
    button.on_left = left
    button.on_down = down


    # Repeat the request for three locations points (A, D)
    for request in range(3):
        ############################## Request for the angle ##############################
        input_type = "angle"

        # Initilize the display
        refresh_display()

        # Wait for the user to enter the angle and press the enter button
        while not button.enter:
            button.process()
        
        # Inform the user that the angle is accepted
        angle_list.append(angle)
        display.clear()
        print_text("{} degree as A{} is accepted".format(angle, request), 0, 0)
        display.update()

        # Reset the angle and number_list
        angle = 0
        number_list = [0, 0, 0]
        sleep(2)
        

        ############################## Request for the distance ##############################
        input_type = "distance"

        # Initilize the display
        refresh_display()

        # Wait for the user to enter the distance and press the enter button
        while not button.enter:
            button.process()
        
        # Inform the user that the distance is accepted
        distance_list.append(distance)
        display.clear()
        print_text("{} as D{} is accepted".format(distance, request), 0, 0)
        display.update()

        # Reset the distance and number_list
        distance = 0
        number_list = [0, 0, 0]
        sleep(2)

    # Print out all the angles (Test purpose)
    display.clear()
    print_text("The angles are: ", 0, 0)
    for i in range(3):
        print_text("{}".format(angle_list[i]), 20 + (i-1)*20, 20)
    display.update()
    sleep(2)
    # Print out all the distances
    display.clear()
    print_text("The distances are: ", 0, 0)
    for i in range(3):
        print_text("{}".format(distance_list[i]), 20 + (i-1)*20, 20)
    display.update()
    sleep(2)



