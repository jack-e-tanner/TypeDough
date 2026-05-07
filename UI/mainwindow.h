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

    bool spawn_node(NodeType type, QPointF scene_pos);
    bool spawn_wire(GraphManager::Port from, GraphManager::Port to);

    void delete_node(int node_id);
    //bool delete_connection();
    //void add_conection();
    //void delete_connection();

    void show_bg_context_menu(const QPoint& pos);

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void show_node_options(int node_id, const QPoint& localPos);
    void show_context_menu(const QPoint& pos);
    void rename_node(int node_id);

private:
    std::pair<int, QString> creation_helper(NodeType type);

    Ui::MainWindow* m_ui;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    GraphManager m_manager;
    std::unordered_map<int, NodeItem*> m_visual_nodes;
    std::vector<WireItem*> m_wires;
};
#endif // MAINWINDOW_H
