#ifndef PORTITEM_H
#define PORTITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

class PortItem : public QGraphicsObject
{
    Q_OBJECT

public:
    PortItem(int port_id, bool is_output, QGraphicsItem* parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    const int getPortID() const { return m_port_id; }
    const bool isOutput() const { return m_is_output; }

signals:
    void startWireDrag(int port_id, bool is_output, QPointF scene_pos);
    void dragWire(QPointF current_scene_pos);
    void endWireDrag(QPointF drop_scene_pos, int port_id, bool is_output);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    bool m_is_hovering;
    bool m_is_dragging = false;
    bool m_is_output;
    int m_port_id;
};

#endif // PORTITEM_H
