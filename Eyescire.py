###################################
# PROPERTY OF THE EYESCIRE ENGINE #
###################################

selected_file = None


# --- IMPORTS ---- #
import Dependencies
import BinaryCompiler
import dearpygui.dearpygui as dpg
from dearpyguisimple import CreateWindow, StartApp

# --- GLOBAL VARIABLES --- #
selected_file = None
MAIN_CONTENT_GROUP_TAG = "main_content_group"
window_tag = None # Global variable to hold the window tag

# --- CALLBACKS --- #
def load_template(template_type):
    """
    Handles template loading and screen switching.
    """
    print(f"[Eyescire Engine] {template_type} Template Selected")
    clear_main_content() # Clear the current screen

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
        clear_main_content()
        LoadEyescireEngine(selected_file)
    else:
        print("No file was selected.")
        
def handle_cancel(sender, app_data):
    """
    This callback function is executed if the user clicks 'Cancel'.
    """
    print("File selection cancelled.")

# --- SCREEN MANAGEMENT --- #
def clear_main_content():
    """Deletes all widgets from the main content group."""
    dpg.delete_item(MAIN_CONTENT_GROUP_TAG, children_only=True)

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
            directory_selector=False,
            show=True,
            callback=handle_file_selection,
            cancel_callback=handle_cancel,
            tag=dialog_tag,
            width=500,
            height=400
        ):
            dpg.add_file_extension(".eys", custom_text="[Eyescire Engine Project]")
            dpg.add_file_extension(".*", custom_text="[All Files]")
    else:
        dpg.show_item(dialog_tag)

# --- SELECT TEMPLATE SCREEN --- #
def Select_Template():
    """Builds the widgets for the template selection screen."""
    with dpg.group(horizontal=False, parent=MAIN_CONTENT_GROUP_TAG):
        dpg.add_spacer(height=15)
        dpg.add_text("Eyescire Engine Project Setup", color=(150, 220, 255))
        dpg.add_separator()
        dpg.add_spacer(height=8)
        dpg.add_text("Select a Template", color=(255, 255, 255))
        dpg.add_spacer(height=5)

        with dpg.group(horizontal=False):
            dpg.add_text("[2D] Best for 2D / 2.5D Projects", color=(200, 200, 200))
            dpg.add_button(
                label="2D - Built-In Render Pipeline",
                width=320,
                height=35,
                callback=lambda: load_template("2D"),
                tag="btn_2d"
            )
            dpg.add_spacer(height=15)

            dpg.add_text("[3D] Best for Simple / Complex 3D Projects", color=(200, 200, 200))
            dpg.add_button(
                label="3D - Built-In Render Pipeline",
                width=320,
                height=35,
                callback=lambda: load_template("3D"),
                tag="btn_3d"
            )
            dpg.add_spacer(height=10)
            
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

# --- MAIN EDITOR SCREEN --- #
def MainEditor(dimension):
    """Builds the widgets for the main editor screen."""
    with dpg.group(horizontal=False, parent=MAIN_CONTENT_GROUP_TAG):
        dpg.add_spacer(height=15)
        dpg.add_text(f"Eyescire Engine: {dimension} Editor", color=(150, 220, 255))
        dpg.add_separator()
        dpg.add_spacer(height=8)
        dpg.add_text("Placehold for Main Editor. While you wait 3 BILLION years for this lazy dev to do his job have a nugget! :D", color=(255, 255, 255))

# --- LOAD 2D TEMPLATE --- #
def Load2D():
    filename = "new_2d_project.eys"
    binary_data, _ = BinaryCompiler.convert_object_to_binary(
        "2D", "DefaultObject", "DefaultInfo", "default_texture.png", 0, 0, 0
    )
    BinaryCompiler.write_binary_file(filename, binary_data)
    print(f"[Eyescire Engine] Created new 2D project file: {filename}")
    LoadEyescireEngine("2D")

# --- LOAD 3D TEMPLATE --- #
def LOAD3D():
    filename = "new_3d_project.eys"
    binary_data, _ = BinaryCompiler.convert_object_to_binary(
        "3D", "DefaultObject", "DefaultInfo", "default_texture.png", 0, 0, 0
    )
    BinaryCompiler.write_binary_file(filename, binary_data)
    print(f"[Eyescire Engine] Created new 3D project file: {filename}")
    LoadEyescireEngine("3D")

# --- LOAD ENGINE --- #
def LoadEyescireEngine(project_info):
    Editor_Dimension = project_info
    MainEditor(Editor_Dimension)

# --- MAIN WINDOW --- #
if __name__ == "__main__":
    # The setup function is no longer needed as the content is added later.
    window_tag = CreateWindow(
        title="Eyescire Engine",
        width=1200,
        height=700,
    )
    
    # Add the main content group after the window has been created.
    # The `CreateWindow` function does not require a `content_function` if
    # you add the content manually after it returns.
    dpg.add_group(tag=MAIN_CONTENT_GROUP_TAG, horizontal=False, parent=window_tag)
    Select_Template()

    StartApp()
