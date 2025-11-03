#pragma once

#include "node/NodeBase.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <iostream>

#include "stb_image.h"

#include "FilePicker.h"

class PreviewNode : public NodeBase{
private:
    unsigned char* m_image_data = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_texture = nullptr;
    FilePicker filePicker;
    int m_tex_w = 0;
    int m_tex_h = 0;

protected:
    void NodeContent() override;
    unsigned int GetBorderColor() const override;
    bool ShouldDisplayText() const override;
    void popStyle();
    void setStyle();

public:
    PreviewNode(SDL_Renderer* renderer);
    ~PreviewNode();

    void Description() override;
    void ProcessInternal() override;
    void ClearPreview();
};