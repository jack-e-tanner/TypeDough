#include "addnode.h"

AddNode::AddNode() : Node(-1, "Add")
{
}

void AddNode::evaluate(FlowContext& ctx) {
    // An addnode should only have two inputs and one output
    auto inputA = ctx.get_input(0);
    auto inputB = ctx.get_input(1);

    float result = 0.0f;

    if (std::holds_alternative<float>(inputA) && std::holds_alternative<float>(inputB)) {
        result = std::get<float>(inputA) + std::get<float>(inputB);
    } else if (std::holds_alternative<int>(inputA) && std::holds_alternative<int>(inputB)) {
        result = static_cast<float>(std::get<int>(inputA) + std::get<int>(inputB));
    } else {
        // Handle type mismatch or unsupported types
        // For simplicity, we can set the result to 0 or throw an error
        result = 0.0f;
    }

    ctx.set_output(0, result);
}

std::string AddNode::type_id() const {
    return "math.add";
}