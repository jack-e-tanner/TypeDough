#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

class GraphView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphView(QGraphicsScene *scene, QWidget *parent = nullptr);

signals:
    void deleteRequested();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // GRAPHVIEW_H
