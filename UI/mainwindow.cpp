#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "errorutility.h"
#include <QInputDialog>
#include "Core/Nodes/AllNodes.h"
#include "Core/core.h"

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
    connect(m_view, &QGraphicsView::customContextMenuRequested, this, &MainWindow::show_context_menu);

    setCentralWidget(m_view);
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::show_context_menu(const QPoint& pos) {
    QGraphicsItem* item = m_view->itemAt(pos);

    if (item) {
        NodeItem* node = dynamic_cast<NodeItem*>(item);
        if (node) {
            QPoint globalPos = m_view->viewport()->mapToGlobal(pos);
            show_node_options(node->getID(), globalPos);
            return;
        }
    } else {
        show_bg_context_menu(pos);
    }
}


void MainWindow::spawn_node(NodeType type, QPointF scene_pos) {
    auto [id, label] = creation_helper(type);

    if (id == -1) {
        ErrorPopup::show(this, QString("Failed to make node"), QString("The node type doesn't exist"));
        return;
    }

    NodeItem* visual_node = new NodeItem(id, label);

    connect(visual_node, &NodeItem::startWireDrag, this, &MainWindow::on_startWireDrag);
    connect(visual_node, &NodeItem::dragWire, this, &MainWindow::on_dragWire);
    connect(visual_node, &NodeItem::endWireDrag, this, &MainWindow::on_endWireDrag);

    visual_node->setPos(scene_pos);
    m_scene->addItem(visual_node);

    m_visual_nodes[id] = visual_node;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
        m_view->viewport()->setCursor(Qt::OpenHandCursor);
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (!(event->modifiers() & Qt::ControlModifier)) {
        m_view->setDragMode(QGraphicsView::NoDrag);
        m_view->viewport()->setCursor(Qt::ArrowCursor);
    }

    QMainWindow::keyReleaseEvent(event);
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

    if (from_it == m_visual_nodes.end() || to_it == m_visual_nodes.end())
        ErrorPopup::show(this, QString("Failed to make wire"), QString("The nodes doesn't exist"));
        return;

    if (!m_manager.add_connection(from, to)) {
        ErrorPopup::show(this, QString("Failed to make wire"), QString("The nodes doesn't exist"));
        return;
    }

    WireItem* visual_wire = new WireItem(
        from_it->second, from.port_id,
        to_it->second, to.port_id
        );

    m_wires.push_back(visual_wire);

    m_scene->addItem(visual_wire);
}

void MainWindow::show_bg_context_menu(const QPoint& pos) {
    QMenu menu(this);

    QMenu* addNodeMenu = menu.addMenu("Add Node");

    QAction* addAction = addNodeMenu->addAction("Addition Node");
    QAction* subAction = addNodeMenu->addAction("Subtraction Node");

    QAction* selectedAction = menu.exec(m_view->mapToGlobal(pos));

    if (selectedAction == addAction) {
        QPointF scenePos = m_view->mapToScene(pos);

        spawn_node(NodeType::Add, scenePos);

    } else if (selectedAction == subAction) {
        QPointF scenePos = m_view->mapToScene(pos);
        spawn_node(NodeType::Subtract, scenePos);
    }
}

void MainWindow::show_node_options(int node_id, const QPoint& pos) {
    QMenu menu(this);

    menu.addAction("Rename Node", this, [this, node_id] () { this->rename_node(node_id); });
    menu.addAction("Delete Node", this, [this, node_id] () { this->delete_node(node_id); });
    // TODO
    //menu.addAction("Add Connection", this, [] () { this->spawn_wire()});

    menu.exec(pos);
}

void MainWindow::rename_node(int node_id) {
    auto node_it = m_visual_nodes.find(node_id);

    if (node_it == m_visual_nodes.end()) {
        ErrorPopup::show(this, QString("Failed to rename node"), QString("The node doesn't exist"));
        return;
    }

    NodeItem* node = node_it->second;

    bool ok;
    QString new_name = QInputDialog::getText(this, "Rename Node", "Enter new name:",
                                             QLineEdit::Normal, node->getName(), &ok);

    if (ok && !new_name.trimmed().isEmpty()) {
        node->set_name(new_name);
        m_manager.set_node_name(node_id, new_name.toStdString());
    }
}

void MainWindow::delete_node(int node_id) {
    auto node_it = m_visual_nodes.find(node_id);

    if (node_it == m_visual_nodes.end()) {
        ErrorPopup::show(this, QString("Failed to remove node"), QString("The node doesn't exist"));
        return;
    }

    auto it = m_wires.begin();
    while (it != m_wires.end()) {
        if ((*it)->getStartNode()->getID() == node_id || (*it)->getEndNode()->getID() == node_id) {
            m_scene->removeItem(*it);

            delete *it;

            it = m_wires.erase(it);
        }
    }

    NodeItem* node_to_kill = node_it->second;

    m_scene->removeItem(node_to_kill);

    delete node_to_kill;

    m_visual_nodes.erase(node_it);

    m_manager.delete_node(node_id);

    m_scene->update();
}

void MainWindow::on_startWireDrag(int node_id, int port_id, bool is_output, QPointF pos) {
    m_temp_wire = new QGraphicsPathItem();

    QPen pen(Qt::white, 2, Qt::DashLine);
    m_temp_wire->setPen(pen);

    m_scene->addItem(m_temp_wire);

    m_drag_start_pos = pos;

    on_dragWire(pos);
}

void MainWindow::on_dragWire(QPointF pos) {
    if (!m_temp_wire) return;

    QPainterPath path;
    path.moveTo(m_drag_start_pos);

    path.lineTo(pos);

    m_temp_wire->setPath(path);
}

void MainWindow::on_endWireDrag(QPointF drop_scene_pos, int source_node, int source_port, bool is_output) {
    if (m_temp_wire) {
        m_scene->removeItem(m_temp_wire);
        delete m_temp_wire;
        m_temp_wire = nullptr;
    }

    QGraphicsItem* top_item = m_scene->itemAt(drop_scene_pos, QTransform());

    NodeItem* target_node = nullptr;

    while (top_item) {
        target_node = dynamic_cast<NodeItem*>(top_item);

        if (target_node) {
            break;
        }

        top_item = top_item->parentItem();
    }

    if (target_node) {
        if (target_node->getID() == source_node) {
            return;
        }

        QPointF local_drop = target_node->mapFromScene(drop_scene_pos);

        bool target_is_output;

    }
}


























