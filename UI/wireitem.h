#ifndef WIREITEM_H
#define WIREITEM_H

#include <QGraphicsItem>
#include <QPainter>

class WireItem : QGraphicsItem
{
public:
    WireItem(QGraphicsItem* startItem, QGraphicsItem* endItem);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QGraphicsItem *m_startItem, *m_endItem;
};

#endif // WIREITEM_H
