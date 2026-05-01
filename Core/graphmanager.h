#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include "Nodes/node.h"
#include <unordered_map>
#include <memory>
#include <vector>

class GraphManager
{
public:
    GraphManager() = default;
    ~GraphManager() = default;

    // Create a node
    template<typename T>
    T* create_node(const std::string& name) {
        int id = m_next_id++;

        auto node = std::make_unique<T>(id, name);
        T* ptr = node.get();

        NodeEntry entry {
            .node = std::move(node),
            .incoming_connections = {},
            .output = {}
        };

        m_nodes[id] = std::move(entry);

        return ptr;
    }

    DoughValue get_node_output(int id, int port);

    void set_node_output(int id, int port, DoughValue value);

    void add_connection(int source_node, int source_port, int this_node, int this_port);

    void print_nodes();

private:
    struct Connection {
        int source_id; // ID of the node providing data
        int source_port; // Id in sources output vector
        int this_port; // Index of the input port on THIS node
    };

    struct NodeEntry {
        std::unique_ptr<Node> node;
        std::vector<Connection> incoming_connections;
        std::vector<DoughValue> output;
    };

    std::unordered_map<int, NodeEntry> m_nodes;

    int m_next_id = 0; // for auto-incrementing node IDs
};

#endif // GRAPHMANAGER_H
