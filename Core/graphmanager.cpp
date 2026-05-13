#include "graphmanager.h"
#include "core.h"

bool GraphManager::delete_node(int id) {
    if (!m_nodes.contains(id))
        return false;

    // Remove the node
    m_nodes.erase(id);

    // Remove all connections to and from this node
    for (auto& [node_id, node_entry] : m_nodes) {
        remove_helper(id, node_entry.incoming_connections, &Connection::source_id);
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

bool GraphManager::add_connection(Port from, Port to) {
    if (!can_connect(from, to)) return false;

    Connection conn {
        .source_id = from.node_id,
        .source_port = from.port_id,
        .this_port = to.port_id
    };

    // remove any existing connections to the same target port
    auto& connections = m_nodes.at(to.node_id).incoming_connections;

    remove_helper(to.port_id, connections, &Connection::this_port);

    connections.push_back(std::move(conn));

    return true;
}

bool GraphManager::can_connect(Port from, Port to) const {
    auto source_it = m_nodes.find(from.node_id);
    if (source_it == m_nodes.end()) return false;

    auto this_it = m_nodes.find(to.node_id);
    if (this_it == m_nodes.end()) return false;

    if (!(source_it->second.node->output_ports().accepts(from.port_id))) return false;
    if (!(this_it->second.node->input_ports().accepts(to.port_id))) return false;

    if (from.node_id == to.node_id) return false;

    return true;
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

        std::cout << "Connections:\n";
        for (const auto& conns : node.second.incoming_connections) {
            std::cout << "\tFrom node: " << conns.source_id << " , port: " << conns.source_port << std::endl;
        }
    }
}

const std::string& GraphManager::get_node_name(int id) const {
    auto it = m_nodes.find(id);
    DOUGH_ASSERT(it != m_nodes.end(), "Attempted to get name of non-existent node");

    return it->second.node->get_name();
}

void GraphManager::set_node_name(int node_id, const std::string& name) {
    auto it = m_nodes.find(node_id);

    DOUGH_ASSERT(it != m_nodes.end(), "Attempted to get name of non-existent node");
    it->second.node->set_name(name);
}

