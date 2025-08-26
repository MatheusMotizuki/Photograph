#pragma once

#include "node/NodeBase.hpp"
#include <iostream>

class InputNode : public NodeBase {
private:
    void openDialog();

protected:
    bool ShouldDisplayText() const override;
    void NodeContent() override;

public:
    InputNode();
};
