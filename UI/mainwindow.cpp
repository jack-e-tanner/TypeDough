#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow())
{
    m_ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);

    setCentralWidget(m_view);

    spawn_node(NodeType::Add, QPointF(-100, 0));

    spawn_node(NodeType::Add, QPointF(150, 50));

    GraphManager::Port out_port = {0, 0};
    GraphManager::Port in_port = {1, 0};

    spawn_wire(out_port, in_port);
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::spawn_node(NodeType type, QPointF scene_pos) {
    auto [id, label] = creation_helper(type);

    if (id == -1) return;
    NodeItem* visual_node = new NodeItem(id, label);
    visual_node->setPos(scene_pos);
    m_scene->addItem(visual_node);

    m_visual_nodes[id] = visual_node;
}

std::pair<int, QString> MainWindow::creation_helper(NodeType type) {
    switch (type) {
    case (NodeType::Add):
    {
        return { m_manager.create_node<AddNode>("Add"), "Addition" };
    }

    default:
        return { -1, "Unknown" };
    }
}

void MainWindow::spawn_wire(GraphManager::Port from, GraphManager::Port to) {
    auto from_it = m_visual_nodes.find(from.node_id);
    auto to_it = m_visual_nodes.find(to.node_id);

    if (from_it == m_visual_nodes.end() || to_it == m_visual_nodes.end()) return;

    if (!m_manager.add_connection(from, to)) return;

    WireItem* visual_wire = new WireItem(
        from_it->second, from.port_id,
        to_it->second, to.port_id
    );

    m_scene->addItem(visual_wire);
}

