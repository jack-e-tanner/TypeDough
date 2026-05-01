#include "graphmanager.h"
#include <iostream>

GraphManager::GraphManager() : m_next_id(0) {}

void GraphManager::print_nodes() {
    for (const auto& node : m_nodes) {
        std::cout << "Node ID: " << node.first << ", Name: " << node.second->get_name() << std::endl;
    }
}
