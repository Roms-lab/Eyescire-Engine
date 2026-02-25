/////////////////////////////////
// PROPERTY OF EYESCIRE ENGINE //
/////////////////////////////////

#include <string>
#include <iostream>
#include <fstream>

std::string NewScene3D = R"([Project Settings]
Rendering = '3D'

[Project])";

std::string NewScene2D = R"([Project Settings]
Rendering = '2D'

[Project])";

void CreateNewScene3D(std::string FileName) {
    std::ofstream Scene(FileName + ".eys");
    Scene << NewScene3D;
    Scene.close();
}

void CreateNewScene2D(std::string FileName) {
    std::ofstream Scene(FileName + ".eys");
    Scene << NewScene2D;
    Scene.close();
}
