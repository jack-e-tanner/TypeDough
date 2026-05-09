#include "portitem.h"
#include "nodeitem.h"

PortItem::PortItem(int port_id, int node_id, bool is_output, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_port_id(port_id), m_is_output(is_output),
    m_node_id(node_id) {
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF PortItem::boundingRect() const {
    return QRectF(-5, -5, 10, 10);
}

void PortItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(m_is_hovering ? Qt::cyan : Qt::red);
    painter->setPen(m_is_hovering ? Qt::cyan : Qt::red);
    painter->drawEllipse(boundingRect());
}

void PortItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_is_dragging = true;

        emit startWireDrag(m_node_id, m_port_id, m_is_output, event->scenePos());

        event->accept();
        return;
    }

    QGraphicsItem::mousePressEvent(event);
}

void PortItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_is_dragging) {
        emit dragWire(event->scenePos());

        event->accept();
        return;
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void PortItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (m_is_dragging) {
        m_is_dragging = false;
        emit endWireDrag(event->scenePos(), m_node_id, m_port_id, m_is_output);

        event->accept();
        return;
    }

    QGraphicsItem::mouseReleaseEvent(event);
}

void PortItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    m_is_hovering = true;    
    update();

    if (auto* node = static_cast<NodeItem*>(parentItem())) {
        node->set_isHovering(false);
    }

    emit hoverStateChanged(this, true);
    QGraphicsItem::hoverEnterEvent(event);
}

void PortItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_is_hovering = false;
    update();

    if (auto* node = static_cast<NodeItem*>(parentItem())) {
        node->set_isHovering(true);
    }

    emit hoverStateChanged(this, false);
    QGraphicsItem::hoverLeaveEvent(event);
}
