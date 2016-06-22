#include "utils/GLFWApp.h"

#include <string>
#include <iostream>
#include "utils/Shader.h"

class UniformForLocalVariable: public GLFWApp {
    public:
        void Init(GLFWwindow*) override {

            const std::string vs =
                R"(#version 450
                attribute vec4 a_position;
                void main(){
                    gl_Position = a_position;
                })";

            const std::string fs =
                R"(#version 450
                layout(location = 0) out vec4 fragColor;
                void main(){
                    float b[1] = float[1](2.0);
                    fragColor = (b[0] == 2.0) ? vec4(0.0, 1.0, 0.0, 1.0) : vec4(1.0, 0.0, 0.0, 1.0);
                })";

            mProgram = CompileProgram(vs, fs);

            GLint nActive = 0;
            glGetProgramiv(mProgram, GL_ACTIVE_UNIFORMS, &nActive);

            // The driver returns a settable uniform for the local b variable (called _main_b_0[0])
            std::cout << "Found " << nActive << " uniforms (0 expected)" <<std::endl;
            for (int i = 0; i < nActive; ++i) {
                char name[1024];
                GLsizei length = 0;
                GLsizei size = 0;
                GLenum type = GL_NONE;
                glGetActiveUniform(mProgram, i, sizeof(name), &length, &size, &type, name);
                std::cout << " - " << std::string(name, length) << std::endl;

                // Could set the uniform to 0 and cause b to become 0
            }

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        }

        void Frame() override {
            GLfloat vertices[] = {
                0.0f,  0.5f, 0.0f,
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
            };

            glViewport(0, 0, 640, 480);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(mProgram);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        void Destroy() override {
        
        }

        GLuint mProgram;
};

int main(int argc, const char** argv) {
    return GLFWApp::Run(new UniformForLocalVariable);
}
