#include "Window/mainwindow.h"

#include <QApplication>
#include "Core/Nodes/addnode.h"
#include "Core/Context/noderuntime.h"
#include "Core/graphmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    GraphManager manager;

    manager.create_node<AddNode>("AddNode");

    manager.print_nodes();

    return QCoreApplication::exec();
}
