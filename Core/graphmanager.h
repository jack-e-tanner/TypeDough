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
    int create_node(const std::string& name) {
        int id = m_next_id++;

        auto node = std::make_unique<T>(id, name);

        m_nodes.emplace(id, NodeEntry {
            .node = std::move(node),
            .incoming_connections = {},
            .outgoing_connections = {} ,
            .output = {}
        });

        return id; // UI can use this id to reference the node
    }

    bool delete_node(int id);

    const std::string& get_node_name(int id) const;
    void set_node_name(int node_id, const std::string& name);

    DoughValue get_node_output(int id, int port);

    void set_node_output(int id, int port, DoughValue value);

    struct Port {
        int node_id;
        int port_id;
    };

    bool add_connection(Port from, Port to);

    // Used for ui to check if you can connect; change wire colors
    bool can_connect(Port from, Port to) const;

    void remove_connection(int this_node, int this_port);

    void print_nodes();

private:
    struct Connection {
        int source_id; // ID of the node providing data
        int source_port; // Id in sources output vector
        int this_port; // Index of the input port on THIS node
    };

    struct NodeEntry {
        std::unique_ptr<Node> node; // instance of the node
        std::vector<Connection> incoming_connections; // connections feeding into this node
        std::vector<Connection> outgoing_connections; // connections feeding out of this node
        std::vector<DoughValue> output; // cached output values for this node, indexed by output port
    };

    std::unordered_map<int, NodeEntry> m_nodes;

    int m_next_id = 0; // for auto-incrementing node IDs

    void remove_helper(int this_port, std::vector<Connection>& connections, int Connection::*field);
};

#endif // GRAPHMANAGER_H
