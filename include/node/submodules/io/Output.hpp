#pragma once

#include "node/NodeBase.hpp"

class OutputNode : public NodeBase{
private:
// skip

protected:
    void openDialog();
    void NodeContent() override;
    bool ShouldDisplayText() const override;
    void popStyle();
    void setStyle();

    void Process() override;

public:
    OutputNode();
    ~OutputNode();
};