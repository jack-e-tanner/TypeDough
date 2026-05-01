#ifndef ADDNODE_H
#define ADDNODE_H

#include "node.h"

class AddNode : public Node
{
public:
    AddNode(int id, const std::string& name);

    void evaluate(FlowContext& ctx) override;

    std::string type_id() const override;
};

#endif // ADDNODE_H
