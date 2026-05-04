#include "wireitem.h"
#include <QPointF>

WireItem::WireItem(QGraphicsItem* startItem, QGraphicsItem* endItem)
    : m_startItem(startItem), m_endItem(endItem) {
    setZValue(-1);
}

QRectF WireItem::boundingRect() const {
    return m_startItem->sceneBoundingRect().united(m_endItem->sceneBoundingRect());
}

void WireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPointF p1 = m_startItem->scenePos() + QPointF(120, 45);
    QPointF p2 = m_endItem->scenePos() + QPointF(0, 45);

    QPainterPath path(p1);
    qreal ctrlX = (p1.x() + p2.x()) / 2;
    path.cubicTo(ctrlX, p1.y(), ctrlX, p2.y(), p2.x(), p2.y());

    painter->setPen(QPen(Qt::yellow, 2));
    painter->drawPath(path);
}
