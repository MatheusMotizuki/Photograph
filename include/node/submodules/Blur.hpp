#pragma once

#include "node/NodeBase.hpp"

class BlurNode : public NodeBase {
private:
    ImVec4 nodeColor = ImVec4(1.0f, 0.37f, 0.0f, 0.15f);
    int m_blur_amount = 0;
    int m_blur_type = 0;

protected:
    void NodeContent() override;
    void ProcessInternal() override;
    unsigned int GetBorderColor() const override;

    void setStyleCombo();
    void popStyleCombo();

    void setStyleSlider();
    void popStyleSlider();

public:
    BlurNode();
    ~BlurNode();

    void Description() override;
};