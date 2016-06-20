#include "utils/GLFWApp.h"

#include <string>
#include "utils/Shader.h"

class UniformOrder: public GLFWApp {
    public:
        void Init(GLFWwindow*) override {

            const std::string vs =
                R"(#version 450
                in vec4 a_position;
                out vec4 vtxOut;
                struct structType{
                    uint n;
                    uvec4 m;
                };
                uniform structType u_var;
                void main(){
                    gl_Position = a_position;
                    vtxOut = vec4(0.0, 0.0, 0.0, 1.0);
                    vtxOut.r = u_var.n == 2u ? 1.0 : 0.0;
                    vtxOut.g = u_var.m == uvec4(3u, 10u, 3u, 10u) ? 1.0 : 0.0;
                })";

            const std::string fs =
                R"(#version 450
                precision highp float;
                in vec4 vtxOut;
                layout(location = 0) out vec4 fragColor;
                void main(){
                    fragColor = vtxOut;
                })";

            mProgram = CompileProgram(vs, fs);
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

            GLint location = glGetUniformLocation(mProgram, "u_var.n");
            glUniform1ui(location, 2);
            GLint location2 = glGetUniformLocation(mProgram, "u_var.m");
            glUniform4ui(location2, 3, 10, 3, 10);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        void Destroy() override {
        
        }

        GLuint mProgram;
};

int main(int argc, const char** argv) {
    return GLFWApp::Run(new UniformOrder);
}
