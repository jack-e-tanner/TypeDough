#ifndef ADDNODE_H
#define ADDNODE_H

#include "node.h"

class AddNode : public Node
{
public:
    AddNode(int id, const std::string& name);

    void evaluate(NodeRuntime& runtime) override;

    PortCount set_num_inputs() const override;
    PortCount set_num_outputs() const override;

    const std::string& type_id() const override;
};

#endif // ADDNODE_H
