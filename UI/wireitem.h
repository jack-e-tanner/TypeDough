#ifndef WIREITEM_H
#define WIREITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include "nodeitem.h"

class WireItem : public QGraphicsItem
{
public:
    WireItem(NodeItem* startNode, int startPort, NodeItem* endNode, int endPort);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    NodeItem *m_startNode, *m_endNode;
    int m_startPort, m_endPort;
};

#endif // WIREITEM_H
