#pragma once

#include "node/NodeBase.hpp"

class OutputNode : public NodeBase{
private:
// skip

protected:
    void openDialog();
    void NodeContent() override;
    bool ShouldDisplayText() const override;

public:
    OutputNode();
    ~OutputNode();
};