from time import sleep
import math

# import ev3dev module
from ev3dev2.display import Display
import ev3dev2.fonts as fonts
from ev3dev2.button import Button

class Menu:
  def __init__(self):
    # Initialize the display and button
    self.display = Display()
    self.button = Button()

    # Set the mode list
    self.mode_list = ["Angle", "Distance", "Plot"]

    #
    self.button.on_up = self.up

  def print_text(self, text:str, x0: int, y0:int):
    """
    @brief  This function will print the text on the display
    @param  text: The text that needs to be printed
    """

    self.display.text_pixels(text,clear_screen=False, x=x0, y=y0, 
                        text_color = 'black', font = fonts.load('ncenB10'))


  def up(self, state):
    self.display.clear()
    self.print_text("{} degree as A{} is accepted".format(5, 1), 0, 0)
    self.display.update()