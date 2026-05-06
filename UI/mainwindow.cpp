#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "errorutility.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow())
{
    m_ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene, this);

    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->setDragMode(QGraphicsView::NoDrag);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(m_view, &QGraphicsView::customContextMenuRequested, this, &MainWindow::show_context_menu);

    if(!spawn_node(NodeType::Add, QPointF(-100, 0))) std::cout << "WRONG";

    setCentralWidget(m_view);
}

MainWindow::~MainWindow() {
    delete m_ui;
}

bool MainWindow::spawn_node(NodeType type, QPointF scene_pos) {
    auto [id, label] = creation_helper(type);

    if (id == -1) return false;;
    NodeItem* visual_node = new NodeItem(id, label);

    connect(visual_node, &NodeItem::doubleClick, this, &MainWindow::show_node_options);

    visual_node->setPos(scene_pos);
    m_scene->addItem(visual_node);

    m_visual_nodes[id] = visual_node;

    return true;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
        m_view->viewport()->setCursor(Qt::OpenHandCursor);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (!(event->modifiers() & Qt::ControlModifier)) {
        m_view->setDragMode(QGraphicsView::NoDrag);
        m_view->viewport()->setCursor(Qt::ArrowCursor);
    }

    QMainWindow::keyReleaseEvent(event);
}

void mouseDoubleClickEvent(QMouseEvent *event) {

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

bool MainWindow::spawn_wire(GraphManager::Port from, GraphManager::Port to) {
    auto from_it = m_visual_nodes.find(from.node_id);
    auto to_it = m_visual_nodes.find(to.node_id);

    if (from_it == m_visual_nodes.end() || to_it == m_visual_nodes.end()) return false;

    if (!m_manager.add_connection(from, to)) return false;

    WireItem* visual_wire = new WireItem(
        from_it->second, from.port_id,
        to_it->second, to.port_id
        );

    m_scene->addItem(visual_wire);

    return true;
}

void MainWindow::show_context_menu(const QPoint& pos) {
    QMenu menu(this);

    QMenu* addNodeMenu = menu.addMenu("Add Node");

    QAction* addAction = addNodeMenu->addAction("Addition Node");
    QAction* subAction = addNodeMenu->addAction("Subtraction Node");

    QAction* selectedAction = menu.exec(m_view->mapToGlobal(pos));

    if (selectedAction == addAction) {
        QPointF scenePos = m_view->mapToScene(pos);
        if(!(spawn_node(NodeType::Add, scenePos))) {
            ErrorPopup::show(this, QString("Failed to add node"), QString("The node type is not found"));
        }
    } else if (selectedAction == subAction) {
        QPointF scenePos = m_view->mapToScene(pos);
        if(!(spawn_node(NodeType::Subtract, scenePos))) {
            std::cout << "FAIL\n";
            ErrorPopup::show(this, QString("Failed to add node"), QString("The node type is not found"));
        }
    }
}

void MainWindow::show_node_options(int node_id, const QPoint& screen_pos) {

    std::cout << "RECEIVED EVENT\n\n\n\n\n\n\n\n";
    QMenu menu(this);

    QMenu* renameActionMenu = menu.addMenu("Rename node");
    QMenu* deleteActionMenu = menu.addMenu("Delete Node");
    QMenu* addConnectionActionMenu = menu.addMenu("Add Connection");

    QAction* selectedAction = menu.exec(m_view->mapToGlobal(screen_pos));


}