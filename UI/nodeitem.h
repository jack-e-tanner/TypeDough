#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsItem>
#include <QPainter>

class NodeItem : public QGraphicsItem
{
public:
    NodeItem(int id, QString name);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QPointF get_port_scene_pos(int port_id, bool is_input) const;
private:
    int m_id;
    QString m_name;
};

#endif // NODEITEM_H
