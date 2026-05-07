#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

class MainWindow;

class NodeItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    NodeItem(int id, QString name);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QPointF get_port_scene_pos(int port_id, bool is_input) const;
    int get_clicked_port(QPointF local_pos, bool& is_output);

    inline const int getID() const { return m_id; }
    inline const QString getName() const { return m_name; }

    inline void set_name(const QString& name)  {
        m_name = QString(name);
        update();
    }

signals:
    void startWireDrag(QPointF start_pos);
    void dragWire(QPointF current_scene_pos);
    void endWireDrag(QPointF drop_scene_pos, int source_node, int source_port, bool is_output);

private:
    int m_id;
    QString m_name;

    bool m_isHoveredOnNode;
    bool m_isHoveredOnPort;
    bool m_isDraggingWire = false;
    int m_draggedPortId = -1;
    bool m_draggedPortIsOutput = false;
};

#endif // NODEITEM_H
