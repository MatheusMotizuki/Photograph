#pragma once

#include "node/NodeBase.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <iostream>

#include "stb_image.h"

class InputNode : public NodeBase {
private:
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_texture = nullptr;
    int m_tex_w = 0;
    int m_tex_h = 0;

protected:
    void openDialog();
    void NodeContent() override;
    bool ShouldDisplayText() const override;

public:
    InputNode(SDL_Renderer* renderer);
    ~InputNode();
};
