#pragma once

#include "node/NodeBase.hpp"

#include <emscripten.h>

class DownloadNode : public NodeBase{
private:
    GLuint m_texture = 0;
    int m_tex_w = 0;
    int m_tex_h = 0;
    std::vector<unsigned char> m_last_pixels;

protected:
    void NodeContent() override;
    bool ShouldDisplayText() const override;
    void popStyle();
    void setStyle();
    
    bool CreateTextureFromData(unsigned char* data, int width, int height, int channels);

public:
    DownloadNode();
    ~DownloadNode();

    void Description() override;
    void ProcessInternal() override;
    void ClearPreview();
};
