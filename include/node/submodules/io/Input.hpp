#pragma once

#include "node/NodeBase.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>

#include "stb_image.h"
#include "stb_image_resize2.h"

#include "FilePicker.h"

class InputNode : public NodeBase {
private:
    unsigned char* m_image_data = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_texture = nullptr;
    FilePicker filePicker;
    int m_tex_w = 0;
    int m_tex_h = 0;

protected:
    void NodeContent() override;
    bool ShouldDisplayText() const override;
    void setStyle();
    void popStyle();

public:
    InputNode(SDL_Renderer* renderer);
    ~InputNode();

    void Description() override;
    void ProcessInternal() override;

    // const ImageData& GetImageData() const { return output_image; }
    // void SetImageData(const ImageData& image) { output_image = image; }
    bool ImageLoaded = false;
};
