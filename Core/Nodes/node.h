#ifndef NODE_H
#define NODE_H

#include <string>
#include "../flowcontext.h"

/**
 * @brief The base node class that all nodes will inherit from;
 */

class Node
{
public:
    Node(int id, std::string name) : m_id(id), m_name(name) {}
    virtual ~Node() = default;

    // Evaluate the node based on the inherited implementation and the provided context
    virtual void evaluate(FlowContext& ctx) = 0;

    // Return a unique string identifier for the node type;
    // used for serialization and deserialization
    virtual std::string type_id() const = 0;

    inline int get_id() const { return m_id; }
    inline const std::string& get_name() const { return m_name; }
protected:
    // Unique identifier for the node; auto-incremented by the graph when a node is added
    int m_id;

    // Name of the node; used for debugging and visualization purposes
    std::string m_name;
};
#endif //
