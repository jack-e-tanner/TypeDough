#include "wireitem.h"
#include <QPointF>

WireItem::WireItem(NodeItem* startNode, int startPort, NodeItem* endNode, int endPort)
    : m_startNode(startNode), m_endNode(endNode),
      m_startPort(startPort), m_endPort(endPort)
{
    setZValue(-1);

    connect(startNode, &NodeItem::moved, this, &WireItem::on_endpoint_moved);
    connect(endNode, &NodeItem::moved, this, &WireItem::on_endpoint_moved);
}

QRectF WireItem::boundingRect() const {
    QPointF p1 = mapFromScene(m_startNode->get_port_scene_pos(m_startPort, false));
    QPointF p2 = mapFromScene(m_endNode->get_port_scene_pos(m_endPort, true));
    return QRectF(p1, p2).normalized().adjusted(-4, -4, 4, 4);
}

void WireItem::on_endpoint_moved() {
    prepareGeometryChange();
    update();
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

    QPen pen(m_isHovering ? Qt::darkYellow : Qt::yellow, m_isHovering ? 3 : 2);
    painter->setPen(pen);

    painter->drawPath(path);
}

void WireItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    m_isHovering = true;
    update();

    QGraphicsItem::hoverEnterEvent(event);
}

void WireItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_isHovering = false;
    update();

    QGraphicsItem::hoverLeaveEvent(event);
}
