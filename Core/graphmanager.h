#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include "flowcontext.h"
#include <vector>

class GraphManager
{
public:
    GraphManager();

private:
    std::vector<int> m_nodes;
};

#endif // GRAPHMANAGER_H
