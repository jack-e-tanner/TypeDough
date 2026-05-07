#include "portitem.h"
#include "Core/core.h"

PortItem::PortItem(int port_id, bool is_output, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_port_id(port_id), m_is_output(is_output) {
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF PortItem::boundingRect() const {
    return QRectF(-10, -10, 10, 10);
}

void PortItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(m_is_hovering ? Qt::cyan : Qt::red);
    painter->setPen(m_is_hovering ? Qt::cyan : Qt::red);
    painter->drawEllipse(boundingRect());
}

void PortItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_is_dragging = true;

        emit startWireDrag(m_port_id, m_is_output, event->scenePos());

        event->accept();
    }

    QGraphicsItem::mousePressEvent(event);
}

void PortItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    DOUGH_COORDS(event->scenePos());

    if (m_is_dragging) {
        emit dragWire(event->scenePos());
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void PortItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (m_is_dragging) {
        m_is_dragging = false;
        emit endWireDrag(event->scenePos(), m_port_id, m_is_output);
    }
}

void PortItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    m_is_hovering = true;
    update();

    QGraphicsItem::hoverEnterEvent(event);
}

void PortItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_is_hovering = false;
    update();

    QGraphicsItem::hoverLeaveEvent(event);
}
