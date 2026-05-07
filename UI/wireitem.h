#ifndef WIREITEM_H
#define WIREITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include "nodeitem.h"

class WireItem : public QGraphicsItem
{
public:
    WireItem(NodeItem* startNode, int startPort, NodeItem* endNode, int endPort);
    ~WireItem() = default;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    inline const NodeItem* getStartNode() const { return m_startNode; }
    inline const NodeItem* getEndNode() const { return m_endNode; }
    inline const int getStartPort() const { return m_startPort; }
    inline const int getEndPort() const { return m_endPort; }

private:
    NodeItem *m_startNode, *m_endNode;
    int m_startPort, m_endPort;
};

#endif // WIREITEM_H
