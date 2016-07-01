#include "utils/GLFWApp.h"

#include <string>
#include <iostream>
#include "utils/Shader.h"

class BlendFuncExtendedArray : public GLFWApp {
    public:
        void Init(GLFWwindow*) override {

            const std::string vs =
                R"(#version 330
                layout(location = 0) in vec4 a_position;
                void main(){
                    gl_Position = a_position;
                })";

            // Doesn't work with FragData and SecondaryFragData being arrays.
            // Deleting all occurence of [0] and [1] makes it work.
            const std::string fs =
                R"(#version 330
                   uniform vec4 src;
                   uniform vec4 src1;
                   out vec4 FragData[1];
                   out vec4 SecondaryFragData[1];
                   void main() {
                       FragData[0] = src;
                       SecondaryFragData[0] = src1;
                   })";

            mProgram = CompileProgramStuffBeforeLink(vs, fs, [](GLuint program){
                glBindFragDataLocationIndexed(program, 0, 0, "FragData[0]");
                glBindFragDataLocationIndexed(program, 0, 1, "SecondaryFragData[0]");
            });
            std::cout << "Location of FragData, should be 0: " << glGetFragDataLocation(mProgram, "FragData[0]") << std::endl;
            std::cout << "Index of FragData, should be 0: " << glGetFragDataIndex(mProgram, "FragData[0]") << std::endl;
            std::cout << "Location of SecondaryFragData, should be 0: " << glGetFragDataLocation(mProgram, "SecondaryFragData[0]") << std::endl;
            std::cout << "Index of SecondaryFragData, should be 1: " << glGetFragDataIndex(mProgram, "SecondaryFragData[0]") << std::endl;

            GLfloat vertices[] = {
                0.0f,  0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f, 1.0f,
            };

            glGenBuffers(1, &mBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glGenVertexArrays(1, &mVAO);
            glBindVertexArray(mVAO);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
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

            glBindVertexArray(mVAO);
            glDrawArrays(GL_TRIANGLES, 0, 256);
        }

        void Destroy() override {
        
        }

        GLuint mProgram;
        GLuint mBuffer;
        GLuint mVAO;
};

int main(int argc, const char** argv) {
    GLFWApp::InitParams params;
    params.glMajor = 3;
    params.glMinor = 3;
    params.coreProfile = true;
    return GLFWApp::Run(new BlendFuncExtendedArray, params);
}
