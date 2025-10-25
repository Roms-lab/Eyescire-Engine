###################################
# PROPERTY OF THE EYESCIRE ENGINE #
###################################

import dearpygui.dearpygui as dpg
""" Wrapper for dearpygui """

_CONTEXT_CREATED = False

def CreateWindow(title, width, height, content_function=None, is_primary=True):
    """
    Creates and manages a Dear PyGui application for a single window.
    """
    global _CONTEXT_CREATED
    if not _CONTEXT_CREATED:
        dpg.create_context()
        dpg.create_viewport(title=title, width=width, height=height)
        dpg.setup_dearpygui()
        _CONTEXT_CREATED = True

    tag = dpg.generate_uuid()
    with dpg.window(tag=tag):
        if content_function:
            content_function()
    
    if is_primary:
        dpg.set_primary_window(tag, True)
    
    return tag

def StartApp():
    """Starts the Dear PyGui application loop."""
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

def AddText(text):
    dpg.add_text(text)

def AddButton(button):
    dpg.add_button(button)