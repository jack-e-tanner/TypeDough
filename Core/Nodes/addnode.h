#ifndef ADDNODE_H
#define ADDNODE_H

#include "node.h"

class AddNode : public Node
{
public:
    AddNode(int id, const std::string& name);

    void evaluate(NodeRuntime& runtime) override;

    PortCount input_ports() const override;
    PortCount output_ports() const override;

    std::string_view type_id() const override;
};

#endif // ADDNODE_H
