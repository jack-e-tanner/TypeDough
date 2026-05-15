#ifndef DOUGHVALUE_H
#define DOUGHVALUE_H

#include <variant>
#include <string>
#include <vector>

class DoughValue;

struct LuaVector {
    double x, y, z;
};

struct LuaMapEntry;

struct LuaTable {
    std::vector<DoughValue> array;

    // luau map, compile only, todo: may remove
    std::vector<LuaMapEntry> map;
};

class DoughValue {
public:
    using Storage = std::variant<std::monostate, int, float, std::string, bool>;

    DoughValue() = default; // nil

    template <typename T>
    DoughValue(T&& v) : m_value(std::forward<T>(v)) {}

    template <typename Visitor>
    constexpr decltype(auto) visit(Visitor&& v) const {
        return std::visit(std::forward<Visitor>(v), m_value);
    }

    template <typename T>
    bool holds() const { return std::holds_alternative<T>(m_value); }

    template <typename T>
    const T& get() const { return std::get<T>(m_value); }

private:
    Storage m_value;
};

struct LuaMapEntry {
    DoughValue key;
    DoughValue value;
};

#endif // DOUGHVALUE_H
