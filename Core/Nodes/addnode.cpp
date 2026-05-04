#include "addnode.h"

AddNode::AddNode(int id, const std::string& name) : Node(id, name)
{
}

void AddNode::evaluate(NodeRuntime& runtime) {

}

std::string_view AddNode::type_id() const {
    return "Add";
}

AddNode::PortCount AddNode::input_ports() const {
    return AddNode::PortCount::dynamic();
}

AddNode::PortCount AddNode::output_ports() const {
    return AddNode::PortCount::fixed(1);
}
