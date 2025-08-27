#pragma once

#include "node/NodeBase.hpp"

class InputNode : public NodeBase {
private:
// skip

protected:
    void openDialog();
    void NodeContent() override;
    bool ShouldDisplayText() const override;

public:
    InputNode();
    ~InputNode();
};
