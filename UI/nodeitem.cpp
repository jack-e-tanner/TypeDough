#include "nodeitem.h"

NodeItem::NodeItem(int id, QString name) :
    m_id(id), m_name(name) {
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF NodeItem::boundingRect() const {
    return QRectF(0, 0, 120, 80);
}

void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    QPen pen(m_isHoveredOnNode ? Qt::yellow : Qt::black, m_isHoveredOnNode ? 3 : 1);
    painter->setPen(pen);

    painter->setBrush(Qt::darkGray);
    painter->drawRoundedRect(boundingRect(), 10, 10);

    painter->setPen(Qt::white);
    painter->drawText(QRectF(0, 0, 120, 30), Qt::AlignCenter, m_name);

    painter->setBrush(m_isHoveredOnPort ? Qt::cyan : Qt::red);
    painter->drawEllipse(-5, 40, 10, 10);
    painter->drawEllipse(115, 40, 10, 10);
}

QPointF NodeItem::get_port_scene_pos(int port_id, bool is_input) const {
    qreal x = is_input ? 0 : 120;
    qreal y = 40 + (port_id * 20);

    return mapToScene(QPointF(x, y));
}

void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    m_isHoveredOnNode = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    m_isHoveredOnNode = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

int NodeItem::get_clicked_port(QPointF local_pos, bool& is_output) {
    QRectF input_rect(-10, 35, 20, 20);
    if (input_rect.contains(local_pos)) {
        is_output = false;
        m_isHoveredOnPort = true;
        return 0;
    }

    QRectF output_rect(110, 35, 20, 20);
    if (output_rect.contains(local_pos)) {
        is_output = true;
        m_isHoveredOnPort = true;
        return 0;
    }

    return -1;
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        bool is_output;
        int port_id = get_clicked_port(event->pos(), is_output);

        if (port_id != -1) {
            m_isDraggingWire = true;
            m_draggedPortId = port_id;
            m_draggedPortIsOutput = is_output;

            QPointF start_pos = get_port_scene_pos(port_id, !is_output);
            emit startWireDrag(m_id, port_id, is_output, start_pos);

            event->accept();
            return;
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isDraggingWire) {
        emit dragWire(event->scenePos());
        return;
    }

    QGraphicsItem::mouseMoveEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isDraggingWire) {
        m_isDraggingWire = false;
        emit endWireDrag(event->scenePos(), m_id, m_draggedPortId, m_draggedPortIsOutput);
        return;
    }

    QGraphicsItem::mouseReleaseEvent(event);
}


















