#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Core/graphmanager.h"
#include "nodeitem.h"
#include "Core/Nodes/AllNodes.h"
#include "Core/Nodes/NodeTypes.h"
#include "wireitem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void spawn_node(NodeType type, QPointF scene_pos);
    void spawn_wire(GraphManager::Port from, GraphManager::Port to);

private:
    std::pair<int, QString> creation_helper(NodeType type);

    Ui::MainWindow* m_ui;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    GraphManager m_manager;
    std::unordered_map<int, NodeItem*> m_visual_nodes;

};
#endif // MAINWINDOW_H
