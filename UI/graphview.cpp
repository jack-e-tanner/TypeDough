#include "graphview.h"
#include <cmath>

GraphView::GraphView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {
    setTransformationAnchor(AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void GraphView::wheelEvent(QWheelEvent *event) {
    const int delta = event->angleDelta().y();
    if (delta == 0) {
        QGraphicsView::wheelEvent(event);
        return;
    }

    const qreal sign = (delta > 0) ? 1.0 : -1.0;
    const qreal magnitude = std::sqrt(std::abs(delta));
    const qreal factor = std::pow(1.0069, sign * magnitude);

    scale(factor, factor);
    event->accept();
}