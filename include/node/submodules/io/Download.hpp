#pragma once

#include "node/NodeBase.hpp"

#include <SDL2/SDL.h>

class DownloadNode : public NodeBase{
private:
// skip

protected:
    void NodeContent() override;
    bool ShouldDisplayText() const override;
    void popStyle();
    void setStyle();

    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_texture = nullptr;
    int m_tex_w = 0;
    int m_tex_h = 0;

public:
    DownloadNode(SDL_Renderer* renderer);
    ~DownloadNode();

    void Description() override;
    void ProcessInternal() override;
    void ClearPreview();
};