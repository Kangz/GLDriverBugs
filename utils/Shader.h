#pragma once

#include <string>

#include "utils/GLFW.h"

GLuint CompileProgram(const std::string& vsSource, const std::string& fsSource);
