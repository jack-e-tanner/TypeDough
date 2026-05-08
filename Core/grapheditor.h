#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QObject>
#include "graphmanager.h"

class grapheditor : QObject
{
    Q_OBJECT

public:
    grapheditor();

private:
    GraphManager m_manager;
};

#endif // GRAPHEDITOR_H
