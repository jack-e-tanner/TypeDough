#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

class GraphView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;

};

#endif // GRAPHVIEW_H
