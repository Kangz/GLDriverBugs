#include "utils/GLFWApp.h"

#include <string>
#include <iostream>
#include "utils/Shader.h"

class UniformForLocalVariable: public GLFWApp {
    public:
        void Init(GLFWwindow*) override {
            char buf[1000];

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            GLuint fbo;
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

            std::cout << "For RGBA16F" << std::endl;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1, 1, 0, GL_RGBA, GL_HALF_FLOAT, 0);
            glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &buf);
            std::cout << "Framebuffer status is " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            std::cout << "GLError is " << glGetError() << std::endl;

            glCheckFramebufferStatus(GL_FRAMEBUFFER);

            std::cout << "For RGB16F" << std::endl;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1, 1, 0, GL_RGB, GL_HALF_FLOAT, 0);
            // Uncommenting this line fixes the test.
            // std::cout << "Framebuffer status is " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, &buf);
            // On Mesa 11.2 Intel, the framebuffer is complete but glGetError is GL_INVALID_FRAMEBUFFER_OPERATION
            std::cout << "Framebuffer status is " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            std::cout << "GLError is " << glGetError() << std::endl;

            // Ugly early out, otherwise looping on glXSwapBuffers freezes the system???
            throw 0;
        }

        void Frame() override {
        }

        void Destroy() override {
        }
};

int main(int argc, const char** argv) {
    return GLFWApp::Run(new UniformForLocalVariable);
}
