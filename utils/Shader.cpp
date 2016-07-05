#include "Shader.h"

#include <vector>
#include <iostream>

GLuint CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);

    const char* cSource = source.c_str();
    glShaderSource(shader, 1, &cSource, nullptr);
    glCompileShader(shader);

    GLint compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == 0) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        // Info log length includes the null terminator, so 1 means that the info log is an empty
        // string.
        if (infoLogLength > 1) {
            std::vector<GLchar> infoLog(infoLogLength);
            glGetShaderInfoLog(shader, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);
            std::cerr << "shader compilation failed: " << &infoLog[0];
        } else {
            std::cerr << "shader compilation failed. <Empty log message>";
        }

        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

GLuint CompileProgram(const std::string& vsSource, const std::string& fsSource) {
    return CompileProgramStuffBeforeLink(vsSource, fsSource, [](GLuint){});
}

GLuint CompileProgramStuffBeforeLink(const std::string& vsSource, const std::string& fsSource, std::function<void (GLuint)> todo) {
    GLuint program = glCreateProgram();

    GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);

    if (vs == 0 || fs == 0) {
        glDeleteShader(fs);
        glDeleteShader(vs);
        glDeleteProgram(program);
        return 0;
    }

    glAttachShader(program, vs);
    glDeleteShader(vs);

    glAttachShader(program, fs);
    glDeleteShader(fs);

    todo(program);

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == 0) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        // Info log length includes the null terminator, so 1 means that the info log is an empty
        // string.
        if (infoLogLength > 1) {
            std::vector<GLchar> infoLog(infoLogLength);
            glGetProgramInfoLog(program, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);

            std::cerr << "program link failed: " << &infoLog[0];
        } else {
            std::cerr << "program link failed. <Empty log message>";
        }

        glDeleteProgram(program);
        return 0;
    }

    return program;
}
