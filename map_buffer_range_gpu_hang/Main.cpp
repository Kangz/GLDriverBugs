#include "utils/GLFWApp.h"

#include <string>

class MapBufferRangeGPUHang : public GLFWApp {
    public:
        void Init(GLFWwindow*) override {
            // This code basically makes a texture, a buffer, reads the texture in the buffer and maps it.
            // This sequence causes any GPU operation afterwards to crash?
            static char blob[131072];

            // Make a texture
            GLuint tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

            // Make a buffer
            GLuint buf;
            glGenBuffers(1, &buf);
            glBindBuffer(GL_ARRAY_BUFFER, buf);
            // This has to be null otherwise there is no hang
            glBufferData(GL_ARRAY_BUFFER, 131072, NULL, GL_STATIC_DRAW);
            // Hang disappear if BufferSubData is removed
            glBufferSubData(GL_ARRAY_BUFFER, 0, 131072, blob);

            // Make the FBO for reading the texture in the buffer
            GLuint fbo;
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
            glBindTexture(GL_TEXTURE_2D, tex);
            glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

            // Do the read and map
            glBindBuffer(GL_PIXEL_PACK_BUFFER, buf);
            glReadPixels(0, 0, 256, 128, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, 131072, GL_MAP_READ_BIT);

            // Clear an unrelated framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void Frame() override {
        }

        void Destroy() override {
        
        }
};

int main(int argc, const char** argv) {
    return GLFWApp::Run(new MapBufferRangeGPUHang, GLFWApp::InitParams{4, 1, true});
}
