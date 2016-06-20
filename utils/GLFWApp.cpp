#include "GLFWApp.h"

#include <iostream>
#include <memory>

namespace {

    void OnGLFWError(int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    }

}

int GLFWApp::Run(GLFWApp* app_) {
    std::unique_ptr<GLFWApp> app(app_);

    glfwSetErrorCallback(OnGLFWError);
    
    if (!glfwInit()) {
        return 1;
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
