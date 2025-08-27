#include "../include/style.hpp"

void Style::SetStyle(unsigned int) { /* pass */ };
void Style::PopStyle() { /* pass */ };

// -- //

void NodeStyle::SetStyle(unsigned int borderColor) {
    ImNodes::PushStyleVar(ImNodesStyleVar_NodeBorderThickness, 1.7f);
    ImNodes::PushColorStyle(ImNodesCol_NodeOutline, borderColor);
    ImNodes::PushColorStyle(ImNodesCol_NodeBackground, IM_COL32(30, 30, 30, 255));
    ImNodes::PushColorStyle(ImNodesCol_NodeBackgroundHovered, IM_COL32(37, 37, 37, 255));
    ImNodes::PushColorStyle(ImNodesCol_NodeBackgroundSelected, IM_COL32(41, 41, 41, 255));
    
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick | ImNodesAttributeFlags_EnableLinkCreationOnSnap);
    ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(41, 41, 41, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(51, 51, 51, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(58, 58, 58, 255));
    
    ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(117, 117, 117, 255));
    ImNodes::PushColorStyle(ImNodesCol_PinHovered, IM_COL32(0, 255, 255, 255));
}

void NodeStyle::PopStyle() {
    ImNodes::PopStyleVar();
    for (int i = 0; i < 8; i++) ImNodes::PopColorStyle();
    ImNodes::PopAttributeFlag();
}