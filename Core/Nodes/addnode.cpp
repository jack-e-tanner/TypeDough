#include "addnode.h"

AddNode::AddNode(int id, const std::string& name) : Node(id, name)
{
}

void AddNode::evaluate(NodeRuntime& runtime) {

}

AddNode::PortCount AddNode::set_num_inputs() const {
    return AddNode::PortCount::dynamic();
}

AddNode::PortCount AddNode::set_num_outputs() const {
    return AddNode::PortCount::fixed(1);
}
