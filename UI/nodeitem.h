#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsItem>

class NodeItem : public QGraphicsItem
{
public:
    NodeItem(int id, QString name);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int m_id;
    QString m_name;
};

#endif // NODEITEM_H
