#include "GLFWApp.h"

#include <iostream>
#include <memory>

namespace {

    void OnGLFWError(int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    }

}

int GLFWApp::Run(GLFWApp* app_) {
    InitParams params;
    Run(app_, params);
}

int GLFWApp::Run(GLFWApp* app_, const InitParams& params) {
    std::unique_ptr<GLFWApp> app(app_);

    glfwSetErrorCallback(OnGLFWError);
    
    if (!glfwInit()) {
        return 1;
    }

    if (params.glMajor != 0) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.glMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.glMinor);
        if (params.coreProfile) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "", nullptr, nullptr);
    if (!window) {
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(!gladLoadGL()) {
        return 1;
    }

    app->Init(window);

    while(!glfwWindowShouldClose(window)) {
        app->Frame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    app->Destroy();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
