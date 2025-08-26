#include "node/NodeBase.hpp"

// Initialize the static counter for automatic ID generation
int NodeBase::next_id = 1;

NodeBase::NodeBase(const std::string& title, PinType pinType, 
                   bool deletable, ImVec4 title_col)
    : n_id(next_id++), n_title(title), n_pinType(pinType), 
      deletable(deletable), n_title_col(title_col) {
    n_input_id = n_id * 10 + 1;
    n_output_id = n_id * 10 + 2;
}