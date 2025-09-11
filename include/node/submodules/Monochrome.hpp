#pragma once

#include "node/NodeBase.hpp"

class MonochromeNode : public NodeBase {
private:
// skip

protected:
    void Process() override;
    void NodeContent() override;
    unsigned int GetBorderColor() const override;

public:
    MonochromeNode();
    ~MonochromeNode();
};