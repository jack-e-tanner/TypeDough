#include "graphmanager.h"
#include "Core.h"

bool GraphManager::delete_node(int id) {
    if (!m_nodes.contains(id))
        return false;

    // Remove the node
    m_nodes.erase(id);

    // Remove all connections to this node
    for (auto& [node_id, node_entry] : m_nodes) {
        remove_helper(id, node_entry.incoming_connections, &Connection::source_id);
        remove_helper(id, node_entry.outgoing_connections, &Connection::source_id);
    }

    return true;
}

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

void GraphManager::remove_helper(int this_port, std::vector<Connection>& connections, int Connection::*field) {
    std::erase_if(connections, [this_port, field](const Connection& conn) {
        return conn.*field == this_port;
    });
}

void GraphManager::add_connection(Port from, Port to) {
    auto source_it = m_nodes.find(from.node);
    DOUGH_ASSERT(source_it != m_nodes.end(), "Attempted to connect from non-existent source node");

    auto this_it = m_nodes.find(to.node);
    DOUGH_ASSERT(this_it != m_nodes.end(), "Attempted to connect to non-existent target node");

    Connection conn {
        .source_id = from.node,
        .source_port = from.port,
        .this_port = to.port
    };

    // remove any existing connections to the same target port
    auto& connections = this_it->second.incoming_connections;

    remove_helper(to.port, connections, &Connection::this_port);

    connections.push_back(std::move(conn));
}

void GraphManager::remove_connection(int this_node, int this_port) {
    auto this_it = m_nodes.find(this_node);
    DOUGH_ASSERT(this_it != m_nodes.end(), "Attempted to remove connection from non-existent target node");

    auto& connections = this_it->second.incoming_connections;

    remove_helper(this_port, connections, &Connection::this_port);
}

void GraphManager::print_nodes() {
    for (const auto& node : m_nodes) {
        std::cout << "Node ID: " << node.first << ", Name: " << node.second.node->get_name() << std::endl;
    }
}

const std::string& GraphManager::get_node_name(int id) const {
    auto it = m_nodes.find(id);
    DOUGH_ASSERT(it != m_nodes.end(), "Attempted to get name of non-existent node");

    return it->second.node->get_name();
}
