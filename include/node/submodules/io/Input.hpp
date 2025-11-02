#pragma once

#include "node/NodeBase.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <GL/gl.h>
#endif

#include "stb_image.h"
#include "FilePicker.h"

class InputNode : public NodeBase {
private:
    unsigned char* m_image_data = nullptr;
    GLuint m_texture = 0;
    // FilePicker filePicker;
    int m_tex_w = 0;
    int m_tex_h = 0;

protected:
    void NodeContent() override;
    bool ShouldDisplayText() const override;
    void popStyle();
    void setStyle();

    void Process() override;
    bool CreateTextureFromData(unsigned char* data, int width, int height, int channels);

public:
    InputNode();
    ~InputNode();
    
    void Description() override;
    
    // Public method for loading image from memory (called from JS callback)
    void LoadImageFromMemory(unsigned char* data, int width, int height, int channels);
};