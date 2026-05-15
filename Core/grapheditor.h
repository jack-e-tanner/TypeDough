#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <QObject>
#include <QPointF>
#include <QString>
#include <unordered_map>
#include <vector>
#include <utility>
#include "graphmanager.h"
#include "Nodes/NodeTypes.h"

class QGraphicsScene;
class QGraphicsPathItem;
class QListWidget;
class QListWidgetItem;
class QPoint;
class QWidget;
class GraphView;
class NodeItem;
class WireItem;
class PortItem;
class PinpointItem;

class GraphEditor : public QObject
{
    Q_OBJECT

public:
    explicit GraphEditor(QWidget* parent = nullptr);

    GraphView* view() const { return m_view; }
    QListWidget* pinpoint_list() const { return m_pp_list; }

    void spawn_node(NodeType type, QPointF scene_pos);
    void spawn_wire(GraphManager::Port from, GraphManager::Port to);

private slots:
    void show_context_menu(const QPoint& pos);
    void show_bg_context_menu(const QPoint& pos);
    void show_node_options(int node_id, const QPoint& localPos);
    void show_pp_options(int pp_id, const QPoint& pos);

    void delete_node(int node_id);
    void rename_node(int node_id);
    void delete_pp(int pp_id);
    void edit_pp(int pp_id);

    void on_startWireDrag(int node_id, int port_id, bool is_output, QPointF pos);
    void on_dragWire(QPointF current_scene_pos);
    void on_endWireDrag(QPointF drop_scene_pos, int source_node, int source_port, bool is_output);
    void on_hoverStateChanged(PortItem* port, bool hovering);
    void on_pp_clicked(QListWidgetItem* item);
    void handle_delete();

private:
    std::pair<int, QString> creation_helper(NodeType type);
    void delete_selected_wires();
    QListWidgetItem* find_pp_row(int pp_id) const;

    GraphManager m_manager;
    QGraphicsScene* m_scene;
    GraphView* m_view;
    QListWidget* m_pp_list;

    std::unordered_map<int, NodeItem*> m_visual_nodes;
    std::vector<WireItem*> m_wires;
    std::unordered_map<int, PinpointItem*> m_pps;
    int m_next_pp_id = 0;

    QGraphicsPathItem* m_temp_wire = nullptr;
    QPointF m_drag_start_pos;
    PortItem* m_hovered_port = nullptr;
    int m_drag_source_node = -1;
    int m_drag_source_port = -1;
};

#endif // GRAPHEDITOR_H
