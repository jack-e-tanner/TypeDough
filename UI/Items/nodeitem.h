#ifndef NODEITEM_H
#define NODEITEM_H

#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsObject>
#include "portitem.h"

class MainWindow;

class NodeItem : public QGraphicsObject
{
    Q_OBJECT

public:
    NodeItem(int id, QString name);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPointF get_port_scene_pos(int port_id, bool is_input) const;

    inline const int getID() const { return m_id; }
    inline const QString getName() const { return m_name; }

    QList<PortItem*> ports() const;

    inline void set_name(const QString& name)  {
        m_name = QString(name);
        update();
    }

    inline void set_isHovering(bool hovering) {
        m_isHovering = hovering;
        update();
    }

signals:
    void moved();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    void add_port(int port_id, bool is_output, QPointF local_pos);

    int m_id;
    QString m_name;

    bool m_isHovering = false;
};

#endif // NODEITEM_H
