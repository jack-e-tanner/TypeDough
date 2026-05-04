#include "UI/mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include "Core/Nodes/addnode.h"
#include "Core/graphmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("TypeDough");
    w.resize(600, 600);

    QWidget* central_widget = new QWidget(&w);

    QVBoxLayout* layout = new QVBoxLayout(central_widget);

    QLabel* label = new QLabel("Node Count: 0");
    QPushButton* btn = new QPushButton("Add Node");

    layout->addWidget(label);
    layout->addWidget(btn);

    w.setCentralWidget(central_widget);

    int node_count = 0;

    QObject::connect(btn, &QPushButton::clicked, [&]() {
        node_count++;

        label->setText("Node Count: " + QString::number(node_count));
    });

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

    w.show();
    return a.exec();
}
