#include "nodeitem.h"
#include <iostream>

NodeItem::NodeItem(int id, QString name) :
    m_id(id), m_name(name) {
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    add_port(0, false, QPointF(0, 40));
    add_port(0, true, QPointF(120, 40));
}

void NodeItem::add_port(int port_id, bool is_output, QPointF local_pos) {
    PortItem* port = new PortItem(port_id, m_id, is_output, this);
    port->setPos(local_pos);

    connect(port, &PortItem::startWireDrag, this, [this](int node_id, int port_id, bool is_output, QPointF pos) {
        emit startWireDrag(node_id, port_id, is_output, pos);
    });

    connect(port, &PortItem::dragWire, this, &NodeItem::dragWire);

    connect(port, &PortItem::endWireDrag, this, [this](QPointF pos, int p_id, bool is_out) {
        emit endWireDrag(pos, m_id, p_id, is_out);
    });

    connect(port, &PortItem::hoverStateChanged, this, &NodeItem::hoverStateChanged);
}

QRectF NodeItem::boundingRect() const {
    return QRectF(0, 0, 120, 80);
}

void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QPen pen(m_isHovering ? Qt::yellow : Qt::black, m_isHovering ? 3 : 2);
    painter->setPen(pen);

    painter->setBrush(Qt::darkGray);
    painter->drawRoundedRect(boundingRect(), 10, 10);

    painter->setPen(Qt::white);
    painter->drawText(QRectF(0, 0, 120, 30), Qt::AlignCenter, m_name);
}

QPointF NodeItem::get_port_scene_pos(int port_id, bool is_input) const {
    qreal x = is_input ? 0 : 120;
    qreal y = 40 + (port_id * 20);

    return mapToScene(QPointF(x, y));
}

void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    m_isHovering = true;
    update();

    QGraphicsItem::hoverEnterEvent(event);
}

void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_isHovering = false;
    update();

    QGraphicsItem::hoverLeaveEvent(event);
}


