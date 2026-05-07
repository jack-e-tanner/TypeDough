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
#include "QKeyEvent"

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

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

public slots:
    void delete_node(int node_id);
    void rename_node(int node_id);

    void show_bg_context_menu(const QPoint& pos);
    void show_node_options(int node_id, const QPoint& localPos);
    void show_context_menu(const QPoint& pos);

    void on_start_wire_drag(int node_id, int port_id, bool is_output, QPointF start_pos);
    void on_drag_wire(QPointF current_scene_pos);
    void on_end_wire_drag(QPointF drop_scene_pos, int source_node, int source_port, bool is_output);

private:
    std::pair<int, QString> creation_helper(NodeType type);

    Ui::MainWindow* m_ui;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    GraphManager m_manager;
    std::unordered_map<int, NodeItem*> m_visual_nodes;
    std::vector<WireItem*> m_wires;

    QGraphicsPathItem* m_temp_wire = nullptr;
    QPointF m_drag_start_pos;
};
#endif // MAINWINDOW_H
