#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include "Nodes/node.h"
#include <unordered_map>
#include <memory>
#include <vector>

class GraphManager
{
    // The FlowContext needs access to the graph's nodes to evaluate them
    friend FlowContext;

public:
    GraphManager();
    ~GraphManager() = default;

    // Create a node
    template<typename T>
    T* create_node(const std::string& name) {
        int id = m_next_id++;

        auto node = std::make_unique<T>(id, name);
        T* ptr = node.get();
        m_nodes[id] = std::move(node);

        return ptr;
    }

    void print_nodes();

private:
    std::unordered_map<int, std::unique_ptr<Node>> m_nodes;

    struct Connection {
        int source_node, source_port;
        int to_node, to_port;
    };

    std::vector<Connection> m_connections;

    int m_next_id = 0; // for auto-incrementing node IDs
};

#endif // GRAPHMANAGER_H
