#include "wireitem.h"
#include <QPointF>

WireItem::WireItem(NodeItem* startNode, int startPort, NodeItem* endNode, int endPort)
    : QGraphicsItem(nullptr),
      m_startNode(startNode), m_endNode(endNode),
      m_startPort(startPort), m_endPort(endPort)
{
    setZValue(-1);
}

QRectF WireItem::boundingRect() const {
    return m_startNode->sceneBoundingRect().united(m_endNode->sceneBoundingRect());
}

void WireItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QPointF p1 = m_startNode->get_port_scene_pos(m_startPort, false);
    QPointF p2 = m_endNode->get_port_scene_pos(m_endPort, true);

    QPointF local_p1 = mapFromScene(p1);
    QPointF local_p2 = mapFromScene(p2);

    QPainterPath path(local_p1);
    qreal dx = local_p2.x() - local_p1.x();
    qreal ctrlX = local_p1.x() + dx * 0.5;

    path.cubicTo(ctrlX, local_p1.y(), ctrlX, local_p2.y(), local_p2.x(), local_p2.y());

    painter->setPen(QPen(Qt::yellow, 2));
    painter->drawPath(path);
}
