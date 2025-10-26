###################################
# PROPERTY OF THE EYESCIRE ENGINE #
###################################

# --- IMPORTS ---- #
import Dependencies
import dearpygui.dearpygui as dpg
from dearpyguisimple import CreateWindow, StartApp

# --- CALLBACKS --- #
def load_template(template_type):
    dpg.delete_item("template_text", children_only=True)
    dpg.add_text(f"Loading {template_type} Template...", parent="template_text")
    print(f"[Eyescire Engine] {template_type} Template Selected")

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

        dpg.add_separator()
        dpg.add_spacer(height=10)
        with dpg.group(tag="template_text"):
            dpg.add_text("Select a template to begin...", color=(150, 150, 150))

# --- MAIN WINDOW --- #
if __name__ == "__main__":
    window_tag = CreateWindow(
        title="Eyescire Engine",
        width=1200,
        height=700,
        content_function=Select_Template
    )
    StartApp()
