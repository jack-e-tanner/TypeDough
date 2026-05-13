#ifndef DOUGHVALUE_H
#define DOUGHVALUE_H

#include <variant>
#include <string>
#include <vector>
#include <memory>

/** DoughValue is the base type */
struct DoughValue;
struct LuaMapEntry;
struct LuaTable;
struct LuaVector;
struct DoughValueNull;

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

struct DoughValueNull {};

struct DoughValue {
    std::variant<
        int,
        float,
        std::string,
        bool,
        std::unique_ptr<LuaTable>,
        LuaVector,
        DoughValueNull
    > value;

    DoughValue add(DoughValue other) {
        DoughValue result = DoughValue {
            .value = DoughValueNull {}
        };
        // TODO
        return result;
    }
};

struct LuaMapEntry {
    DoughValue key;
    DoughValue value;
};

#endif // DOUGHVALUE_H
