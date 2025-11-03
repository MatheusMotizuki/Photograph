#pragma once

#include "node/NodeBase.hpp"

class RGBNode : public NodeBase {
private:
    int m_r = 0, m_g = 0, m_b = 0;

protected:
    void NodeContent() override;
    void ProcessInternal() override;
    unsigned int GetBorderColor() const override;

public:
    RGBNode();
    ~RGBNode();

    void Description() override;
};
