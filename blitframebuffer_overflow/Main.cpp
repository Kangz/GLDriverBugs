#include "utils/GLFWApp.h"

#include <string>
#include <iostream>
#include <algorithm>

#include "utils/Shader.h"


class BlitFramebufferOverflow: public GLFWApp {
    public:
        void Init(GLFWwindow*) override {
            // These can be changed and the bug still reproduces.
            int srcWidth = 128;
            int srcHeight = srcWidth;
            int destTextureWidth = 1024;
            int destTextureHeight = destTextureWidth;

            // These are required values to reproduce the bug. Smaller values don't seem to repro.
            int magFactor = 512;
            int minDestSize = 65536 + 1;

            GLuint textures[2];
            glGenTextures(2, textures);

            glBindTexture(GL_TEXTURE_2D, textures[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, srcWidth, srcHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, destTextureWidth, destTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            GLuint fbos[2];
            glGenFramebuffers(2, fbos);

            glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
            std::cout << "Framebuffer status is " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

            glBindFramebuffer(GL_FRAMEBUFFER, fbos[1]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);
            std::cout << "Framebuffer status is " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

            std::cout << "GLError after setup is " << glGetError() << std::endl;

            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbos[0]);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbos[1]);

            glBlitFramebuffer(0, 0, srcWidth, srcHeight,
                0, 0, std::max(srcWidth * magFactor + 1, minDestSize), std::max(srcHeight * magFactor + 1, minDestSize),
                GL_COLOR_BUFFER_BIT, GL_NEAREST);

            std::cout << "GLError after blitFramebuffer is " << glGetError() << std::endl;
        }

        void Frame() override {
            std::cout << "GLError on frame is " << glGetError() << std::endl;
        }

        void Destroy() override {
        }
};

int main(int argc, const char** argv) {
    return GLFWApp::Run(new BlitFramebufferOverflow);
}
