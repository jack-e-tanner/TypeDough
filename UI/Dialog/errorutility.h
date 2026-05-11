#ifndef ERRORUTILITY_H
#define ERRORUTILITY_H

#include <QMessageBox>
#include <QString>

class ErrorPopup {
public:
    static void show(QWidget* parent, const QString& title, const QString& text) {
        QMessageBox msg_box(parent);

        msg_box.setWindowTitle(QString(title));
        msg_box.setText(text);
        msg_box.setIcon(QMessageBox::Critical);

        msg_box.setStandardButtons(QMessageBox::Ok);

        msg_box.exec();
    }
};

#endif // ERRORUTILITY_H
