#include "utils/GLFWApp.h"

#include <string>
#include <cassert>
#include "utils/Shader.h"

class UniformOrder: public GLFWApp {
    public:
        void Init(GLFWwindow*) override {

            const std::string vs =
                R"(#version 450
                in vec4 a_position;
                void main(){
                    gl_Position = a_position;
                })";

            // Writes a depth gradient going from depthBounds.x to depthBounds.y
            // Section 15.2.3 of the GL 4.5 specification says that conversion is not
            // done but clamping is so the output depth should be in [0.0, 1.0]
            const std::string depthFs =
                R"(#version 450
                layout(location = 0) out vec4 fragColor;
                uniform vec2 depthBounds;
                void main(){
                    float f = (gl_FragCoord.x / 640.0 + gl_FragCoord.y / 480.0) / 2.0;
                    gl_FragDepth = depthBounds[0] + f * (depthBounds[1] - depthBounds[0]);
                    fragColor = vec4(0.0, 1.0, 0.0, 1.0);
                })";

            mDepthProgram = CompileProgram(vs, depthFs);
            mDepthBounds = glGetUniformLocation(mDepthProgram, "depthBounds");
            assert(mDepthBounds != -1);

            glGenTextures(1, &mColorTexture);
            glBindTexture(GL_TEXTURE_2D, mColorTexture);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 640, 480);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glGenTextures(1, &mDepthTexture);
            glBindTexture(GL_TEXTURE_2D, mDepthTexture);
            // Works with DEPTH_COMPONENT24
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 640, 480);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glGenFramebuffers(1, &mFramebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

            assert(glGetError() == GL_NO_ERROR);
        }

        void Frame() override {
            glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
            glUseProgram(mDepthProgram);

            GLfloat vertices[] = {
                -1.0f, -1.0f, 0.0f, 1.0f,
                -1.0f,  1.0f, 0.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f,
                 1.0f,  1.0f, 0.0f, 1.0f,
            };

            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vertices);
            glEnableVertexAttribArray(0);

            glViewport(0, 0, 640, 480);

            glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
            glClearDepth(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_DEPTH_TEST);

            // Render a first depth gradient
            glDepthFunc(GL_ALWAYS);
            glUniform2f(mDepthBounds, -1.0, 2.0);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glClear(GL_COLOR_BUFFER_BIT);

            // Render a different gradient, it should match the depth buffer when it is clamped
            // but for some reason isn't
            glDepthFunc(GL_EQUAL);
            glUniform2f(mDepthBounds, -0.5, 3.0);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBlitFramebuffer(0, 0, 640, 480, 0, 0, 640, 480, GL_COLOR_BUFFER_BIT, GL_NEAREST);
            assert(glGetError() == GL_NO_ERROR);
        }

        void Destroy() override {
        }

        GLuint mDepthProgram;
        GLint mDepthBounds;
        GLuint mFramebuffer;
        GLuint mColorTexture;
        GLuint mDepthTexture;
};

int main(int argc, const char** argv) {
    return GLFWApp::Run(new UniformOrder);
}
