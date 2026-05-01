#ifndef NODERUNTIME_H
#define NODERUNTIME_H

#include "flowcontext.h"

class NodeRuntime : public FlowContext
{
public:
    NodeRuntime();

    DoughValue get_input(int port_index) override;

    void set_output(int port_index, const DoughValue& value) override;
};

#endif // NODERUNTIME_H
