#ifndef DOUGHVALUE_H
#define DOUGHVALUE_H

#include <variant>
#include <string>
#include <vector>

/** DoughValue is the base type */
struct DoughValue;
struct DoughValueMapEntry;
struct DoughValueTable;
struct DoughValueVector;

struct LuaTable {
    std::vector<DoughValue> array;

    // luau map, compile only, todo: may remove
    std::vector<LuaMapEntry> map;
};

// luau vector, , todo: may remove
struct LuaVector {
    double x;
    double y;
    double z;
};

struct DoughValue : std::variant<
    int,
    float,
    std::string,
    bool,
    LuaTable,
    LuaVector
> {
    using variant::variant;
};

struct LuaMapEntry {
    DoughValue key;
    DoughValue value;
};

#endif // DOUGHVALUE_H
