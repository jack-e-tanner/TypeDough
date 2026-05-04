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

    w.show();
    return a.exec();
}
