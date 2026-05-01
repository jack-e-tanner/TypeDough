#ifndef FLOWCONTEXT_H
#define FLOWCONTEXT_H

#include <variant>
#include <string>

/**
 * @brief The base context class that will be passed to Nodes during evaluation
 *
 */

using DoughValue = std::variant<int, float, std::string, bool>;

class FlowContext
{
public:
    virtual ~FlowContext() = default;

    // This function will be used by nodes to get the value of
    // their input ports during evaluation
    virtual DoughValue get_input(int port_index) = 0;

    // This function will be used by nodes to set the value of
    // their output ports during evaluation
    virtual void set_output(int port_index, const DoughValue& value) = 0;
};

#endif // FLOWCONTEXT_H
