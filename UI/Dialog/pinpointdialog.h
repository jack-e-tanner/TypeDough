#ifndef PINPOINTDIALOG_H
#define PINPOINTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QColor>

class PinpointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PinpointDialog(QWidget* parent = nullptr,
                            const QString& title = "Add Pinpoint",
                            const QString& name = "Pinpoint",
                            const QColor& color = Qt::white);

    QString name() const { return m_nameEdit->text(); }
    QColor color() const { return m_colorCombo->currentData().value<QColor>(); }
private:
    QLineEdit* m_nameEdit;
    QComboBox* m_colorCombo;
};

#endif // PINPOINTDIALOG_H
