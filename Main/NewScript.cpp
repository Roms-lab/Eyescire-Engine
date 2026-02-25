/////////////////////////////////
// PROPERTY OF EYESCIRE ENGINE //
/////////////////////////////////

#include <string>
#include <iostream>
#include <fstream>
#include "NewScript.hpp"

std::string NewScriptSyntax = R"(#include <EyescireEngine.hpp>

void start()
{

}

void update()
{

})";

std::string FilePath;

void CreateNewScript(std::string ScriptName) {
    std::ofstream Script(ScriptName + ".cpp");
    Script << NewScriptSyntax;
    Script.close();
}
