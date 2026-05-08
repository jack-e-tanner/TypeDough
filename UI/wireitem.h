#ifndef WIREITEM_H
#define WIREITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include "nodeitem.h"

class WireItem : public QGraphicsObject
{
    Q_OBJECT

public:
    WireItem(NodeItem* startNode, int startPort, NodeItem* endNode, int endPort);
    ~WireItem() = default;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    inline const NodeItem* getStartNode() const { return m_startNode; }
    inline const NodeItem* getEndNode() const { return m_endNode; }
    inline const int getStartPort() const { return m_startPort; }
    inline const int getEndPort() const { return m_endPort; }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private slots:
    void on_endpoint_moved();

private:
    NodeItem *m_startNode = nullptr;
    NodeItem *m_endNode = nullptr;
    int m_startPort, m_endPort;

    bool m_isHovering = false;
};

#endif // WIREITEM_H
