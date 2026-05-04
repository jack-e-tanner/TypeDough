#ifndef NODE_H
#define NODE_H

#include "Core/noderuntime.h"
#include "Core/core.h"


/** Here at Jack Technologies Inc, we like to have fun ;)
 *  (potential subsidiary of Purrquinox)
 */
inline constexpr int UNLIMITED_DOUGH = -0xD0E;

/**
 * @brief The base node class that all nodes will inherit from;
 */
class Node
{
public:
    explicit Node(int id, const std::string& name) : m_id(id), m_name(name) {}
    virtual ~Node() = default;

    // Evaluate the node based on the inherited implementation and the provided context
    virtual void evaluate(NodeRuntime& runtime) = 0;

    struct PortCount {
        int num_ports;

        static PortCount fixed(int n) {
            DOUGH_ASSERT(n >= 0, "Not enough of dough for ports");
            return {n};
        }

        static PortCount dynamic()    { return { UNLIMITED_DOUGH }; }

        bool accepts(int port) const {
            if (port < 0) return false;
            return num_ports == UNLIMITED_DOUGH || port < num_ports;
        }

        bool is_dynamic() const { return num_ports == UNLIMITED_DOUGH; }
        int count() const {
            return is_dynamic() ? 0 : num_ports;
        }
    };

    virtual PortCount input_ports() const = 0;
    virtual PortCount output_ports() const = 0;

    // Return a unique string identifier for the node type;
    // used for serialization and deserialization
    virtual std::string_view type_id() const = 0;

    inline int get_id() const { return m_id; }
    inline const std::string& get_name() const { return m_name; }
protected:
    // Unique identifier for the node; auto-incremented by the graph when a node is added
    int m_id;

    // Name of the node; used for debugging and visualization purposes
    std::string m_name;
};
#endif //
