#include "Window/mainwindow.h"

#include <QApplication>
#include "Core/Nodes/addnode.h"
#include "Core/graphmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    GraphManager manager;

    manager.create_node<AddNode>("AddNode");

    manager.create_node<AddNode>("AddNode2");

    GraphManager::Port p1 {
        .node_id = 0,
        .port_id = 0
    };

    GraphManager::Port p2 {
        .node_id = 1,
        .port_id = 0
    };

    // p2 conn from p1
    manager.add_connection(p1, p2);

    manager.print_nodes();

    return QCoreApplication::exec();
}
