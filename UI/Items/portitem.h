#ifndef PORTITEM_H
#define PORTITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

class PortItem : public QGraphicsObject
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };
    int type() const override { return Type; }

    PortItem(int port_id, int node_id, bool is_output, QGraphicsItem* parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    const int getPortID() const { return m_port_id; }
    const int getNodeID() const { return m_node_id; }
    const bool isOutput() const { return m_is_output; }
    void setHovering(bool hovering) {
        m_is_hovering = hovering;
        update();
    }

signals:
    void startWireDrag(int node_id, int port_id, bool is_output, QPointF pos);
    void dragWire(QPointF current_scene_pos);
    void endWireDrag(QPointF drop_scene_pos, int source_node, int source_port, bool is_output);
    void hoverStateChanged(PortItem* port, bool hovering);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    bool m_is_hovering = false;
    bool m_is_dragging = false;
    bool m_is_output;
    int m_port_id;
    int m_node_id;
};

#endif // PORTITEM_H
