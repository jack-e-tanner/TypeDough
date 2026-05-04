#include "nodeitem.h"

NodeItem::NodeItem(int id, QString name) :
    m_id(id), m_name(name) {
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
}

QRectF NodeItem::boundingRect() const {
    return QRectF(0, 0, 120, 80);
}

void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setBrush(Qt::darkGray);
    painter->drawRoundedRect(boundingRect(), 10, 10);

    painter->setPen(Qt::white);
    painter->drawText(QRectF(0, 0, 120, 30), Qt::AlignCenter, m_name);

    painter->setBrush(Qt::red);
    painter->drawEllipse(-5, 40, 10, 10);
    painter->drawEllipse(115, 40, 10, 10);
}
