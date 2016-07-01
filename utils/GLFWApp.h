#pragma once

#include "utils/GLFW.h"

class GLFWApp {
    public:
        virtual void Init(GLFWwindow* window) = 0;
        virtual void Frame() = 0;
        virtual void Destroy() = 0;

        struct InitParams {
            int glMajor = 0;
            int glMinor = 0;
            int coreProfile = false;
        };
        static int Run(GLFWApp* app);
        static int Run(GLFWApp* app, const InitParams& params);
};
