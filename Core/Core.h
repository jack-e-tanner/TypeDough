#ifndef CORE_H
#define CORE_H

#include <iostream>

#define DOUGH_ASSERT(x, msg) { if (!(x)) { \
    std::cerr << "--- [DOUGH ASSERTION FAILED] ---" << std::endl; \
    std::cerr << "Message:   " << msg << std::endl; \
    std::cerr << "Condition: " << #x << std::endl; \
    std::cerr << "File:      " << __FILE__ << std::endl; \
    std::cerr << "Line:      " << __LINE__ << std::endl; \
    std::cerr << "--------------------------------" << std::endl; \
    std::abort(); \
} }

#endif // CORE_H
