#include "noderuntime.h"
#include <iostream>

DoughValue NodeRuntime::get_input_from_port(int port) const {
    return 3;
}

void NodeRuntime::set_output_from_port(int port, const DoughValue& value) {
    return;
}
