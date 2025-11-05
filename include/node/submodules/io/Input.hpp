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

    // Setters
    void SetImageData(unsigned char* image_data) { m_image_data = image_data; }
    void SetRenderer(SDL_Renderer* renderer) { m_renderer = renderer; }
    void SetTexture(SDL_Texture* texture) { m_texture = texture; }
    void SetImageLoaded(bool loaded) { ImageLoaded = loaded; }
    void SetNewImageUploaded(bool uploaded) { NewImageUploaded = uploaded; }
    void SetTexWidth(int w) { m_tex_w = w; }
    void SetTexHeight(int h) { m_tex_h = h; }

    SDL_Texture* GetTexture() const { return m_texture; }
    unsigned char* GetImageData() const { return m_image_data; }
    SDL_Renderer* GetRenderer() const { return m_renderer; }

    bool ImageLoaded = false;
    bool NewImageUploaded = false;
};
