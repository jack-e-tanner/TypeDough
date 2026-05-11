#ifndef PINPOINTITEM_H
#define PINPOINTITEM_H

#include <QGraphicsObject>

class Pinpoint;

class PinpointItem : public QGraphicsObject
{
    Q_OBJECT

public:
    enum { Type = UserType + 2 };
    int type() const override { return Type; }

    static constexpr qreal HEAD_RADIUS = 15.0;
    static constexpr qreal TIP_OFFSET = 35.0;
    static constexpr qreal HOLE_RADIUS = 5.0;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    explicit PinpointItem(Pinpoint* pp) : m_pp(pp) {}
    ~PinpointItem() override;

    Pinpoint* data() const { return m_pp; }
private:
    Pinpoint* m_pp;
};

#endif // PINPOINTITEM_H
