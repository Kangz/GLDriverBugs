#pragma once

#include <string>
#include <functional>

#include "utils/GLFW.h"

GLuint CompileProgram(const std::string& vsSource, const std::string& fsSource);
GLuint CompileProgramStuffBeforeLink(const std::string& vsSource, const std::string& fsSource, std::function<void (GLuint)> todo);
