#pragma once

#include "node/NodeBase.hpp"

class BrightnessNode : public NodeBase {
private:
    int m_brightness_amount = 50; // 0-100 range, 50 is neutral

protected:
    void NodeContent() override;
    void ProcessInternal() override;
    unsigned int GetBorderColor() const override;

public:
    BrightnessNode();
    ~BrightnessNode();

    void Description() override;
};