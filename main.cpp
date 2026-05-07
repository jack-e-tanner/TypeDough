#include "UI/mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

#ifdef QT_DEBUG
    QList<QScreen*> screens = QApplication::screens();
    if (screens.size() > 1) {
        QRect secondScreenGeo = screens[1]->geometry();

        w.move(secondScreenGeo.center() - w.rect().center());
    }
#endif

    w.show();
    return a.exec();
}
