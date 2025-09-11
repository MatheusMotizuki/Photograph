#pragma once

#include "node/NodeBase.hpp"

class BlurNode : public NodeBase {
private:
// skip

protected:
    void Process() override;
    void NodeContent() override;
    unsigned int GetBorderColor() const override;

public:
    BlurNode();
    ~BlurNode();
};