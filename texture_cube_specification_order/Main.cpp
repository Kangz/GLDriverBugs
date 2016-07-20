#include "utils/GLFWApp.h"

#include <cassert>
#include <iostream>
#include <vector>
#include "utils/Shader.h"

struct LayerData {
    LayerData(int level, int face, uint16_t color): level(level), face(face), color(color) {}
    int level;
    int face;
    uint16_t color;
};

GLuint CreateTextureCube(const std::vector<LayerData>& layers, int maxLevel) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    for (const auto& layer: layers) {
        GLint level = layer.level;
        GLenum face = layer.face;
        uint16_t color = layer.color;

        GLint levelSize = 1 << (maxLevel - level);
        std::vector<uint16_t> colorData(levelSize * levelSize, color);

        glTexImage2D(face + GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, GL_RG8I, levelSize, levelSize, 0, GL_RG_INTEGER, GL_BYTE, colorData.data());
    }

    return texture;
}

class TextureCubeSpecificationOrder : public GLFWApp {
    public:
        void Init(GLFWwindow*) override {
            const int kMaxLevel = 4;
            const uint16_t black = 0x0000;
            const uint16_t green = 0x7F00;
            const uint16_t red = 0x007F;

            // Create two integer cubemaps with green on level 0 and red on the others.

            // The "regular" cube map is filled in a sensible manner.
            std::vector<LayerData> regularLayers;
            for (int level = 0; level <= kMaxLevel; level ++) {
                for (int face = 0; face < 6; face ++) {
                    if (level == 0) {
                        regularLayers.push_back(LayerData(level, face, green));
                    } else {
                        regularLayers.push_back(LayerData(level, face, red));
                    }
                }
            }

            // The "weird" cube map has all its >= 1 levels filled, then the first level,
            // then the >= 1 levels again but for one face. This replicates what the
            // Chromium command buffer "cubemap completeness" workaround is doing.
            std::vector<LayerData> weirdLayers;
            for (int level = 1; level <= kMaxLevel; level ++) {
                for (int face = 0; face < 5; face ++) {
                    weirdLayers.push_back(LayerData(level, face, black));
                }
                weirdLayers.push_back(LayerData(level, 5, red));
            }
            for (int level = 0; level <= kMaxLevel; level ++) {
                for (int face = 0; face < 6; face ++) {
                    if (level == 0) {
                        weirdLayers.push_back(LayerData(level, face, green));
                    } else {
                        if (face != 5) {
                            weirdLayers.push_back(LayerData(level, face, red));
                        }
                    }
                }
            }

            mRegularTex = CreateTextureCube(regularLayers, kMaxLevel);
            mWeirdTex = CreateTextureCube(weirdLayers, kMaxLevel);

            // At this point both textures should be the same.

            const std::string vs =
                R"(#version 450
                in vec4 a_position;
                void main(){
                    gl_Position = a_position;
                })";

            // This program samples both textures at varying Lod and exposes
            // both of them, the regular on top, and the weird on the bottom.
            // On the NVIDIA driver the top is both green and red while the bottom
            // is only green. This is a driver bug because both textures should be
            // exactly the same.
            // I think that because of our level specification order weirdness, the
            // driver gets confused as to which levels are complete and thinks only
            // level 0 is complete. Then when sampling the texture it clamps the sampling
            // to only level 0, resulting in green for all Lod values.
            const std::string fs =
                R"(#version 450
                layout(location = 0) out vec4 fragColor;
                uniform isamplerCube texRegular;
                uniform isamplerCube texWeird;
                void main() {
                    vec2 fragPos = gl_FragCoord.xy / vec2(640.0, 480.0);
                    float lod = (fragPos.x - 0.5) * 10.0;

                    ivec4 colorRegular = textureLod(texRegular, vec3(1.0, 0.0, 0.0), lod);
                    ivec4 colorWeird = textureLod(texWeird, vec3(1.0, 0.0, 0.0), lod);

                    if (fragPos.y > 0.51) {
                        fragColor = vec4(vec2(colorRegular.rg), 0.0, 1.0);
                    } else if (fragPos.y < 0.49){
                        fragColor = vec4(vec2(colorWeird.rg), 0.0, 1.0);
                    } else {
                        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
                    }
                })";

            mSampleProgram = CompileProgram(vs, fs);
            mRegularLocation = glGetUniformLocation(mSampleProgram, "texRegular");
            mWeirdLocation = glGetUniformLocation(mSampleProgram, "texWeird");
            assert(mRegularLocation != -1);
            assert(mWeirdLocation != -1);
        }

        void Frame() override {
            glUseProgram(mSampleProgram);

            GLfloat vertices[] = {
                -1.0f, -1.0f, 0.0f, 1.0f,
                -1.0f,  1.0f, 0.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f,
                 1.0f,  1.0f, 0.0f, 1.0f,
            };

            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vertices);
            glEnableVertexAttribArray(0);

            glViewport(0, 0, 640, 480);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mRegularTex);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mWeirdTex);

            glUniform1i(mRegularLocation, 0);
            glUniform1i(mWeirdLocation, 1);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        void Destroy() override {
        }

        GLuint mSampleProgram = 0;
        GLint mRegularLocation = -1;
        GLint mWeirdLocation = -1;
        GLuint mRegularTex = 0;
        GLuint mWeirdTex = 0;
};

int main(int argc, const char** argv) {
    return GLFWApp::Run(new TextureCubeSpecificationOrder);
}
