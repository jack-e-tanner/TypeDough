#include "Window/mainwindow.h"

#include <QApplication>
#include "Core/Nodes/addnode.h"
#include "Core/Context/noderuntime.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    AddNode addNode;
    NodeRuntime runtime;

    return QCoreApplication::exec();
}
