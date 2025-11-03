#pragma once

#include "node/NodeBase.hpp"
#include <cstdint>

class BrightnessNode : public NodeBase {
private:
    int m_brightness_amount = 50;
    int m_last_processed_brightness = -1;
    uint64_t m_last_input_signature = 0;

protected:
    void NodeContent() override;
    void ProcessInternal() override;
    unsigned int GetBorderColor() const override;

public:
    BrightnessNode();
    ~BrightnessNode();

    void Description() override;
};
