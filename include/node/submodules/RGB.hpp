#pragma once

#include "node/NodeBase.hpp"

class RGBNode : public NodeBase {
private:
// skip

protected:
    void NodeContent() override;
    unsigned int GetBorderColor() const override;

public:
    RGBNode();
    ~RGBNode();

    void Description() override;
    void Process() override;
};