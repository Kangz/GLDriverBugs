#pragma once

#include "utils/GLFW.h"

class GLFWApp {
    public:
        virtual void Init(GLFWwindow* window) = 0;
        virtual void Frame() = 0;
        virtual void Destroy() = 0;

        static int Run(GLFWApp* app);
};
