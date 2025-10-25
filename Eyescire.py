###################################
# PROPERTY OF THE EYESCIRE ENGINE #
###################################

# --- IMPORTS ---- #
import Dependencies
import dearpygui.dearpygui as dpg
from dearpyguisimple import CreateWindow, StartApp

# -- SELECT TEMPLATE --- #
def Select_Template():
    dpg.add_text("Select A Template")
    dpg.add_text("")
    dpg.add_text("Best for 2D/2.5D")
    dpg.add_button(label="2D-Built In Render Pipeline")
    dpg.add_text("Best for Simple/Complex 3D")
    dpg.add_button(label="3D-Built In Render Pipeline")

# --- MAIN WINDOW --- #
if __name__ == "__main__":
    # Create the window and fill it with content.
    window_tag = CreateWindow(
        title="Eyescire Engine",
        width=1200,
        height=700,
        content_function=Select_Template
    )
    
    # Start the application loop.
    StartApp()