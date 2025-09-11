#include "node/NodeBase.hpp"

// private

int NodeBase::next_id = 1; // Initialize the static counter for automatic ID generation

void NodeBase::SetNodeStyle(unsigned int borderColor = IM_COL32(58, 58, 58, 255)) {
  style.SetStyle(borderColor);
}

void NodeBase::ResetStyle() {
  style.PopStyle();
}

// protected

ImFont* NodeBase::GetTitleFont() const {
  return ImGui::GetIO().Fonts->Fonts[12];
}

unsigned int NodeBase::GetBorderColor() const {
  return IM_COL32(58, 58, 58, 255);
}

bool NodeBase::ShouldDisplayText() const { return true; }

void NodeBase::InputText() {
  if (n_pinType == PinType::Input || n_pinType == PinType::Both) {
    ImNodes::BeginInputAttribute(n_input_id, ImNodesPinShape_CircleFilled);
    if (ShouldDisplayText()) ImGui::TextColored(ImVec4(0.702f, 0.702f, 0.702f, 1.f), "input"); else ImGui::Dummy(ImVec2(0, 0));
    ImNodes::EndInputAttribute();
  }
}

void NodeBase::OutputText() {
  if (n_pinType == PinType::Output || n_pinType == PinType::Both) {
    ImNodes::BeginOutputAttribute(n_output_id, ImNodesPinShape_CircleFilled);
    if (ShouldDisplayText()) ImGui::TextColored(ImVec4(0.702f, 0.702f, 0.702f, 1.f), "output"); else ImGui::Dummy(ImVec2(0, 0));
    ImNodes::EndOutputAttribute();
  }
}

// publiq

NodeBase::NodeBase(const std::string& title, PinType pinType, 
                   bool deletable, ImVec4 title_col, std::string tooltip)
    : n_id(next_id++), n_title(title), n_pinType(pinType), 
      deletable(deletable), n_title_col(title_col), tooltip(tooltip) {
    n_input_id = n_id * 10 + 1;
    n_output_id = n_id * 10 + 2;
}

void NodeBase::Draw() {
  SetNodeStyle(GetBorderColor());

  ImNodes::BeginNode(n_id);

  ImNodes::BeginNodeTitleBar();
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[18]);
  ImGui::TextColored(n_title_col, "%s", n_title.c_str());
  ImGui::PopFont();
  ImNodes::EndNodeTitleBar();

  InputText();

  if(n_pinType == PinType::Both){
      ImGui::SameLine();
  }

  OutputText();

  NodeContent(); // this will come from the extended nodes

  ImNodes::EndNode();
  ResetStyle();
}

bool NodeBase::IsProtected() const { return deletable; }
int NodeBase::GetId() const { return n_id; }
bool NodeBase::IsSelected() const { return ImNodes::IsNodeSelected(n_id); }
std::string NodeBase::GetTitle() const { return n_title; }