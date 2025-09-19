#pragma once

#include "node/NodeBase.hpp"

class BrightnessNode : public NodeBase {
private:
// skip

protected:
    void Process() override;
    void NodeContent() override;
    unsigned int GetBorderColor() const override;

public:
    BrightnessNode();
    ~BrightnessNode();

    void Description() override;
};