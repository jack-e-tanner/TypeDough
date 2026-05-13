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

struct DoughValueTable {
    std::vector<DoughValue> array;

    // luau map, compile only, todo: may remove
    std::vector<DoughValueMapEntry> map;
};

// luau vector, , todo: may remove
struct DoughValueVector {
    double x;
    double y;
    double z;
};

struct DoughValue: std::variant<
    int,
    float,
    std::string,
    bool,
    DoughValueTable,
    DoughValueVector
> {
    using variant::variant;
};

struct DoughValueMapEntry {
    DoughValue key;
    DoughValue value;
};



#endif // DOUGHVALUE_H
