/////////////////////////////////
// PROPERTY OF EYESCIRE ENGINE //
/////////////////////////////////

#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <string>

// Exported Variables
extern std::string NewScene3D;
extern std::string NewScene2D;

// Exported Functions
void CreateNewScene3D(std::string FileName = "New3D");
void CreateNewScene2D(std::string Filename = "New2D");

#endif
