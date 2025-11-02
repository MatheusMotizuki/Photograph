#pragma once

#include "node/NodeBase.hpp"

class DownloadNode : public NodeBase{
private:
// skip

protected:
    void NodeContent() override;
    bool ShouldDisplayText() const override;
    void popStyle();
    void setStyle();

public:
    DownloadNode();
    ~DownloadNode();

    void Description() override;
    void Process() override;
};