#pragma once

#include "node/NodeBase.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <iostream>

#include "stb_image.h"

#include "FilePicker.h"

class InputNode : public NodeBase {
private:
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_texture = nullptr;
    FilePicker filePicker;
    int m_tex_w = 0;
    int m_tex_h = 0;

protected:
    void NodeContent() override;
    bool ShouldDisplayText() const override;
    void popStyle();
    void setStyle();

    void Process() override;

public:
    InputNode(SDL_Renderer* renderer);
    ~InputNode();
};
