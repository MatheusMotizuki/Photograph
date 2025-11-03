#pragma once

#include "node/NodeBase.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class PreviewNode : public NodeBase{
private:
    GLuint m_texture = 0;
    int m_tex_w = 0;
    int m_tex_h = 0;

protected:
    void NodeContent() override;
    unsigned int GetBorderColor() const override;
    bool ShouldDisplayText() const override;
    void popStyle();
    void setStyle();
    
    bool CreateTextureFromData(unsigned char* data, int width, int height, int channels);

public:
    PreviewNode();
    ~PreviewNode();

    void Description() override;
    void ProcessInternal() override;
    void ClearPreview();
};