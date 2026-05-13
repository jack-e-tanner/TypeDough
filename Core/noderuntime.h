#ifndef NODERUNTIME_H
#define NODERUNTIME_H

#include "Input/doughvalue.h"

/**
 * Plan is in graphmanager function create an instane of NodeRuntime on stack
 * when evaluating a node, then just delete the noderuntime, so zero storage,
 * however it could be constructed lots of times
 */

class GraphManager;

class NodeRuntime
{
public:
    NodeRuntime(GraphManager& manager, int id) : m_manager(manager), m_self(id) {}

    DoughValue get_input_from_port(int port) const;
    void set_output_from_port(int port, const DoughValue& value);

private:
    GraphManager& m_manager;
    int m_self; // id of the node this runtime is associated with
};

#endif // NODERUNTIME_H
