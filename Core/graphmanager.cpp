#include "graphmanager.h"
#include "Core.h"

DoughValue GraphManager::get_node_output(int id, int port) {
    auto it = m_nodes.find(id);

    DOUGH_ASSERT(it != m_nodes.end(), "Attempted to pull data from non-existent node");

    const auto& outputs = it->second.output;

    DOUGH_ASSERT(port >= 0 && port < static_cast<int>(outputs.size()), "Attempted to pull data from non-existent output port");

    return outputs[port];
}

void GraphManager::set_node_output(int id, int port, DoughValue value) {
    auto it = m_nodes.find(id);

    DOUGH_ASSERT(it != m_nodes.end(), "Attempted to set data on non-existent node");

    auto& outputs = it->second.output;

    DOUGH_ASSERT(port >= 0, "Output port index cannot be negative");

    if (port >= static_cast<int>(outputs.size())) {
        outputs.resize(port + 1); // Resize to accommodate the new port index
    }

    outputs[port] = std::move(value);
}

void GraphManager::add_connection(int source_id, int source_port, int this_id, int this_port) {
    auto source_it = m_nodes.find(source_id);
    DOUGH_ASSERT(source_it != m_nodes.end(), "Attempted to connect from non-existent source node");

    auto target_it = m_nodes.find(this_id);
    DOUGH_ASSERT(target_it != m_nodes.end(), "Attempted to connect to non-existent target node");

    Connection conn {
        .source_id = source_id,
        .source_port = source_port,
        .this_port = this_port
    };

    // remove any existing connections to the same target port
    auto& connections = target_it->second.incoming_connections;

    std::erase_if(connections, [&](const Connection& conn) {
        return conn.this_port == this_port;
    })

    connections.push_back(std::move(conn));
}


void GraphManager::print_nodes() {
    for (const auto& node : m_nodes) {
        std::cout << "Node ID: " << node.first << ", Name: " << node.second.node->get_name() << std::endl;
    }
}

