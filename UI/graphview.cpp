#include "graphview.h"
#include <cmath>

GraphView::GraphView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {
    setTransformationAnchor(AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setFocusPolicy(Qt::StrongFocus);
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

void GraphView::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        setDragMode(QGraphicsView::ScrollHandDrag);
        viewport()->setCursor(Qt::OpenHandCursor);
    }

    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        emit deleteRequested();
    }

    QGraphicsView::keyPressEvent(event);
}

void GraphView::keyReleaseEvent(QKeyEvent *event) {
    if (!(event->modifiers() & Qt::ControlModifier)) {
        setDragMode(QGraphicsView::NoDrag);
        viewport()->setCursor(Qt::ArrowCursor);
    }

    QGraphicsView::keyReleaseEvent(event);
}
