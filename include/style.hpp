#pragma once

#include "imgui.h"
#include "imnodes.h"

class Style {
private:
// skip

protected:
    virtual void SetStyle(unsigned int);
    virtual void PopStyle();
};

class NodeStyle : public Style {
public:
    void SetStyle(unsigned int borderColor) override;
    void PopStyle() override;
};
