###################################
# PROPERTY OF THE EYESCIRE ENGINE #
###################################

selected_file = None


# --- IMPORTS ---- #
import Dependencies
import BinaryCompiler
import dearpygui.dearpygui as dpg
from dearpyguisimple import CreateWindow, StartApp

# --- CALLBACKS --- #
def load_template(template_type):
    dpg.delete_item("template_text", children_only=True)
    dpg.add_text(f"Loading {template_type} Template...", parent="template_text")
    print(f"[Eyescire Engine] {template_type} Template Selected")

    if template_type == "2D":
        Load2D()
    elif template_type == "3D":
        LOAD3D()
    elif template_type == "Existing":
        LOADEXISTING()

def handle_file_selection(sender, app_data):
    """
    This callback function is executed when the user selects a file
    and clicks 'OK' in the file dialog.
    """
    global selected_file
    selected_file = app_data['file_path_name']
    
    if selected_file:
        print(f"Selected file: {selected_file}")
        # Add your file-loading and processing logic here
        # E.g., open and parse the .eys file
    else:
        print("No file was selected.")
        
def handle_cancel(sender, app_data):
    """
    This callback function is executed if the user clicks 'Cancel'.
    """
    print("File selection cancelled.")


# --- LOAD EXISTING PROJECT --- #
def LOADEXISTING():
    """
    Creates and shows a file dialog to select a .eys file.
    """
    print("Requesting file with .eys extension...")
    dialog_tag = "load_eys_dialog"
    
    # Create the dialog only if it doesn't exist
    if not dpg.does_item_exist(dialog_tag):
        with dpg.file_dialog(
            directory_selector=False,  # Set to False to select files, not directories
            show=True,                 # Show the dialog immediately after creation
            callback=handle_file_selection,
            cancel_callback=handle_cancel,
            tag=dialog_tag,
            width=500,
            height=400
        ):
            dpg.add_file_extension(".eys", custom_text="[Eyescire Engine Project]")
            dpg.add_file_extension(".*", custom_text="[All Files]")
    else:
        # If the dialog exists, just show it
        dpg.show_item(dialog_tag)

# --- SELECT TEMPLATE --- #
def Select_Template():
    with dpg.group(horizontal=False):
        dpg.add_spacer(height=15)
        dpg.add_text("Eyescire Engine Project Setup", color=(150, 220, 255))
        dpg.add_separator()
        dpg.add_spacer(height=8)
        dpg.add_text("Select a Template", color=(255, 255, 255))
        dpg.add_spacer(height=5)

        with dpg.group(horizontal=False):
            # 2D Template Section
            dpg.add_text("[2D] Best for 2D / 2.5D Projects", color=(200, 200, 200))
            dpg.add_button(
                label="2D - Built-In Render Pipeline",
                width=320,
                height=35,
                callback=lambda: load_template("2D"),
                tag="btn_2d"
            )
            dpg.add_spacer(height=15)

            # 3D Template Section
            dpg.add_text("[3D] Best for Simple / Complex 3D Projects", color=(200, 200, 200))
            dpg.add_button(
                label="3D - Built-In Render Pipeline",
                width=320,
                height=35,
                callback=lambda: load_template("3D"),
                tag="btn_3d"
            )
            dpg.add_spacer(height=10)
            
            # Load From Folder Section
            dpg.add_text("Load From Folder", color=(200,200,200))
            dpg.add_button(
                label="Load from folder",
                width=320,
                height=35,
                callback=lambda: load_template("Existing"),
                tag="btn_exist"
            )

        dpg.add_separator()
        dpg.add_spacer(height=10)
        with dpg.group(tag="template_text"):
            dpg.add_text("Select a template to begin...", color=(150, 150, 150))

# --- LOAD 2D TEMPLATE --- #
def Load2D():
    filename = "new_2d_project.eys"
    
    # 1. Call the conversion function and capture its return value
    binary_data, _ = BinaryCompiler.convert_object_to_binary(
        "2D", "DefaultObject", "DefaultInfo", "default_texture.png", 0, 0, 0
    )
    
    # 2. Pass the captured binary data to the write function
    BinaryCompiler.write_binary_file(filename, binary_data)
    
    print(f"[Eyescire Engine] Created new 2D project file: {filename}")
    
    # Transition to the main editor window
    # dpg.delete_item("template_selection_window_tag") # Replace with your window tag
    # LoadEyescireEngine(filename)

# --- LOAD 3D TEMPLATE --- #
def LOAD3D():
    filename = "new_3d_project.eys"
    
    # 1. Call the conversion function and capture its return value
    binary_data, _ = BinaryCompiler.convert_object_to_binary(
        "3D", "DefaultObject", "DefaultInfo", "default_texture.png", 0, 0, 0
    )
    
    # 2. Pass the captured binary data to the write function
    BinaryCompiler.write_binary_file(filename, binary_data)
    
    print(f"[Eyescire Engine] Created new 3D project file: {filename}")


# --- LOAD ENGINE --- #
def LoadEyescireEngine():
    pass

# --- MAIN WINDOW --- #
if __name__ == "__main__":
    window_tag = CreateWindow(
        title="Eyescire Engine",
        width=1200,
        height=700,
        content_function=Select_Template
    )
    StartApp()
