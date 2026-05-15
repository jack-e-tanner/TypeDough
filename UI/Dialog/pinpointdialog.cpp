#include "pinpointdialog.h"
#include <QDialogButtonBox>
#include <QFormLayout>

PinpointDialog::PinpointDialog(QWidget* parent, QString text, QColor color)
    : QDialog(parent) {
    auto addColor = [this](const QString& label, const QColor& col) {
        QPixmap pix(16, 16);
        pix.fill(col);
        m_colorCombo->addItem(QIcon(pix), label, QVariant::fromValue(col));
    };

    setWindowTitle("Add Pinpoint");

    auto* layout = new QFormLayout(this);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setText("Pinpoint");
    m_nameEdit->selectAll();
    m_nameEdit->setPlaceholderText("Pinpoint name");
    layout->addRow(text, m_nameEdit);

    m_colorCombo = new QComboBox(this);
    addColor("White",Qt::white);
    addColor("Black",Qt::black);
    addColor("Red",         Qt::red);
    addColor("Dark Red",Qt::darkRed);
    addColor("Green",Qt::green);
    addColor("Dark Green",  Qt::darkGreen);
    addColor("Blue",Qt::blue);
    addColor("Dark Blue",Qt::darkBlue);
    addColor("Cyan",        Qt::cyan);
    addColor("Dark Cyan",Qt::darkCyan);
    addColor("Magenta",Qt::magenta);
    addColor("Dark Magenta",Qt::darkMagenta);
    addColor("Yellow",Qt::yellow);
    addColor("Dark Yellow",Qt::darkYellow);
    addColor("Light Gray",  Qt::lightGray);
    addColor("Gray",Qt::gray);
    addColor("Dark Gray",Qt::darkGray);
    layout->addRow("Color:", m_colorCombo);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addRow(buttons);
}
