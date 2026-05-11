#include "pinpointitem.h"
#include "UI/pinpoint.h"
#include <QPainter>

PinpointItem::~PinpointItem() {
    delete m_pp;
}

QRectF PinpointItem::boundingRect() const {
    constexpr qreal pad = 2.0;
    return QRectF(
        -HEAD_RADIUS - pad,
        -HEAD_RADIUS - pad,
        2.0 * HEAD_RADIUS + 2.0 * pad,
        HEAD_RADIUS + TIP_OFFSET + 2.0 * pad
        );
}

QPainterPath PinpointItem::shape() const {
    QPainterPath head;
    head.addEllipse(QPointF(0, 0), HEAD_RADIUS, HEAD_RADIUS);

    QPainterPath tail;
    tail.moveTo(0, TIP_OFFSET);
    tail.lineTo(-HEAD_RADIUS * 0.7, 0);
    tail.lineTo(HEAD_RADIUS * 0.7, 0);
    tail.closeSubpath();

    return head.united(tail);
}

void PinpointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    const QColor fill = m_pp->color();
    const QColor outline = fill.darker(150);

    painter->setBrush(fill);
    painter->setPen(QPen(outline, 1.5));
    painter->drawPath(shape());

    painter->setBrush(Qt::transparent);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(0, 0), HOLE_RADIUS, HOLE_RADIUS);
}
