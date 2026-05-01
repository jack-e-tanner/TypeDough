#include "noderuntime.h"
#include <iostream>

NodeRuntime::NodeRuntime() {}

DoughValue NodeRuntime::get_input(int port_index) {
    return 0;
}

void NodeRuntime::set_output(int port_index, const DoughValue& value) {
    std::cout << "DONE FOR NOW\n";
}
