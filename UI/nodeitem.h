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

    QPointF get_port_scene_pos(int port_id, bool is_input) const;
    inline const int getID() const { return m_id; }
    inline const QString getName() const { return m_name; }
    inline void set_name(const QString& name)  {
        m_name = QString(name);
        update();
    }
private:
    int m_id;
    QString m_name;
    bool m_isHovered;
};

#endif // NODEITEM_H
