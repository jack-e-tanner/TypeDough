#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "graphview.h"
#include "Core/graphmanager.h"
#include "UI/Items/nodeitem.h"
#include "UI/Items/portitem.h"
#include "Core/Nodes/NodeTypes.h"
#include "UI/Items/wireitem.h"
#include <QKeyEvent>
#include "UI/Items/pinpointitem.h"
#include <QListWidget>

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
    void delete_pp(int node_id);
    void rename_pp(int node_id);

    void show_bg_context_menu(const QPoint& pos);
    void show_node_options(int node_id, const QPoint& localPos);
    void show_context_menu(const QPoint& pos);
    void show_pp_options(int pp_id, const QPoint& pos);

    void on_startWireDrag(int node_id, int port_id, bool is_output, QPointF pos);
    void on_dragWire(QPointF current_scene_pos);
    void on_endWireDrag(QPointF drop_scene_pos, int source_node, int source_port, bool is_output);
    void on_hoverStateChanged(PortItem* port, bool hovering);

private slots:
    void on_pp_clicked(QListWidgetItem* item);

private:
    std::pair<int, QString> creation_helper(NodeType type);
    void delete_selected_wires();
    QListWidgetItem* find_pp_row(int pp_id) const;

    Ui::MainWindow* m_ui;
    QGraphicsScene* m_scene;
    GraphView* m_view;

    GraphManager m_manager;
    std::unordered_map<int, NodeItem*> m_visual_nodes;
    std::vector<WireItem*> m_wires;

    std::unordered_map<int, PinpointItem*> m_pps;
    int m_next_pp_id = 0;
    QListWidget* m_pp_list;

    QGraphicsPathItem* m_temp_wire = nullptr;
    QPointF m_drag_start_pos;

    PortItem* m_hovered_port = nullptr;
    int m_drag_source_node = -1;
    int m_drag_source_port = -1;
};
#endif // MAINWINDOW_H
