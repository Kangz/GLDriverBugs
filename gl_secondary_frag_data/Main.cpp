#include "utils/GLFWApp.h"

#include <string>
#include "utils/Shader.h"

class SecondaryFragData: public GLFWApp {
    public:
        void Init(GLFWwindow*) override {

            const std::string vs =
                R"(#version 100
                attribute vec4 a_position;
                void main(){
                    gl_Position = a_position;
                })";

            const std::string fs =
                R"(#version 100
                   #extension GL_EXT_blend_func_extended : require
                   precision highp float;
                   uniform vec4 src;
                   uniform vec4 src1;
                   void main() {
                       // Works
                       // gl_FragColor = src;
                       // gl_SecondaryFragColorEXT = src1;
                       // Doesn't on Intel Mesa
                       gl_FragData[0] = src;
                       gl_SecondaryFragDataEXT[0] = src1;
                   })";

            mProgram = CompileProgram(vs, fs);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        }

        void Frame() override {
            glViewport(0, 0, 640, 480);
            glClearColor(0.5, 0.5, 0.5, 0.5);
            glClear(GL_COLOR_BUFFER_BIT);

            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFuncSeparate(GL_SRC1_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_COLOR, GL_ONE_MINUS_SRC1_ALPHA);

            glUseProgram(mProgram);

            GLint location = glGetUniformLocation(mProgram, "src");
            glUniform4f(location, 1.0, 1.0, 1.0, 1.0);
            GLint location2 = glGetUniformLocation(mProgram, "src1");
            glUniform4f(location2, 0.3, 0.6, 0.9, 0.7);

            GLfloat vertices[] = {
                0.0f,  0.5f, 0.0f,
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
            };

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        void Destroy() override {
        
        }

        GLuint mProgram;
};

int main(int argc, const char** argv) {
    return GLFWApp::Run(new SecondaryFragData);
}
