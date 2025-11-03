#pragma once
#include <vector>

struct ImageData {
    std::vector<unsigned char> pixels;
    int width = 0, height = 0, channels = 0;
    bool isValid() const { return !pixels.empty() && width > 0 && height > 0; }
};