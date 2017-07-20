#include "GLFWApp.h"

#include <iostream>
#include <memory>

namespace {

    void OnGLFWError(int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    }

    void APIENTRY OnKHRDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        std::cerr << "KHR_debug says: " << message << std::endl;
    }

    bool ExtensionAvailable(const GLFWApp::InitParams& params, const std::string& extension) {
        if (params.coreProfile) {
            GLint nExtensions = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
            for (GLint i = 0; i < nExtensions; i++) {
                if (std::string(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i))) == extension) {
                    return true;
                }
            }
            return false;
        } else {
            std::string extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
            return extensions.find("GL_KHR_debug") != std::string::npos;
        }
    }

}

int GLFWApp::Run(GLFWApp* app_) {
    InitParams params;
    return Run(app_, params);
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
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
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

    if (ExtensionAvailable(params, "GL_KHR_debug")) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OnKHRDebug, nullptr);
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
