#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Dialog/errorutility.h"
#include <QInputDialog>
#include <QMenu>
#include "Dialog/pinpointdialog.h"
#include "pinpoint.h"
#include "Items/pinpointitem.h"
#include "Core/Nodes/AllNodes.h"
#include <QDockWidget>
#include <QPixmap>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow())
{
    m_ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(-10000, -10000, 20000, 20000);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_view = new GraphView(m_scene, this);

    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::NoDrag);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_view, &QGraphicsView::customContextMenuRequested, this, &MainWindow::show_context_menu);

    auto* dock = new QDockWidget("Pinpoints", this);
    m_pp_list = new QListWidget(dock);
    dock->setWidget(m_pp_list);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(m_pp_list, &QListWidget::itemClicked, this, &MainWindow::on_pp_clicked);

    QMenu* viewMenu = menuBar()->addMenu("View");
    viewMenu->addAction(dock->toggleViewAction());

    setCentralWidget(m_view);
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::show_context_menu(const QPoint& pos) {
    QGraphicsItem* item = m_view->itemAt(pos);

    if (!item) {
        show_bg_context_menu(pos);
        return;
    }

    switch (item->type()) {
        case NodeItem::Type: {
            auto* node = qgraphicsitem_cast<NodeItem*>(item);
            show_node_options(node->getID(), m_view->viewport()->mapToGlobal(pos));
            break;
        }
        case PinpointItem::Type: {
            auto* pp = qgraphicsitem_cast<PinpointItem*>(item);
            show_pp_options(pp->data()->id(), m_view->viewport()->mapToGlobal(pos));
            break;
        }
        default: {
            show_bg_context_menu(pos);
        }
    }
}


void MainWindow::spawn_node(NodeType type, QPointF scene_pos) {
    auto [id, label] = creation_helper(type);

    if (id == -1) {
        ErrorPopup::show(this, QString("Failed to make node"), QString("The node type doesn't exist"));
        return;
    }

    NodeItem* visual_node = new NodeItem(id, label);

    for (PortItem* port : visual_node->ports()) {
        connect(port, &PortItem::startWireDrag, this, &MainWindow::on_startWireDrag);
        connect(port, &PortItem::dragWire, this, &MainWindow::on_dragWire);
        connect(port, &PortItem::endWireDrag, this, &MainWindow::on_endWireDrag);
        connect(port, &PortItem::hoverStateChanged, this, &MainWindow::on_hoverStateChanged);
    }

    visual_node->setPos(scene_pos);
    m_scene->addItem(visual_node);

    m_visual_nodes[id] = visual_node;
}

void MainWindow::on_pp_clicked(QListWidgetItem* item) {
    int id = item->data(Qt::UserRole).toInt();
    auto it = m_pps.find(id);
    if (it == m_pps.end()) return;
    const QPointF p = it->second->pos();
    const qreal box = 200.0;
    QRectF target(p.x() - box/2, p.y() - box/2, box, box);
    m_view->fitInView(target, Qt::KeepAspectRatio);
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        m_view->setDragMode(QGraphicsView::ScrollHandDrag);
        m_view->viewport()->setCursor(Qt::OpenHandCursor);
    }

    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        delete_selected_wires();
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

void MainWindow::delete_selected_wires() {
    auto selected = m_scene->selectedItems();
    for (QGraphicsItem* item : selected) {
        auto* wire = dynamic_cast<WireItem*>(item);
        if (!wire) continue;

        int dst_node = wire->getEndNode()->getID();
        int dst_port = wire->getEndPort();
        m_manager.remove_connection(dst_node, dst_port);

        m_scene->removeItem(wire);
        wire->deleteLater();
        std::erase(m_wires, wire);
    }
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

    if (from_it == m_visual_nodes.end() || to_it == m_visual_nodes.end()) {
        ErrorPopup::show(this, QString("Failed to make wire"), QString("The nodes doesn't exist"));
        return;
    }

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

static QIcon makeColorIcon(const QColor& color) {
    QPixmap pix(16, 16);
    pix.fill(color);
    return QIcon(pix);
}

void MainWindow::show_bg_context_menu(const QPoint& pos) {
    const QPointF scenePos = m_view->mapToScene(pos);

    QMenu menu(this);

    QMenu* addNodeMenu = menu.addMenu("Add Node");

    QAction* addAction = addNodeMenu->addAction("Addition Node");
    connect(addAction, &QAction::triggered, this, [this, scenePos]() {
        spawn_node(NodeType::Add, scenePos);
    });

    QAction* subAction = addNodeMenu->addAction("Subtraction Node");
    connect(subAction, &QAction::triggered, this, [this, scenePos]() {
        spawn_node(NodeType::Subtract, scenePos);
    });

    QAction* addPosAction = menu.addAction("Add Pinpoint");
    connect(addPosAction, &QAction::triggered, this, [this, scenePos]() {
        PinpointDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            Pinpoint* pp = new Pinpoint(m_next_pp_id++, dlg.name(), dlg.color(), scenePos);
            PinpointItem* pp_item = new PinpointItem(pp);
            pp_item->setPos(scenePos);
            m_scene->addItem(pp_item);
            m_pps[pp->id()] = pp_item;

            auto* row = new QListWidgetItem(makeColorIcon(dlg.color()), dlg.name());
            row->setData(Qt::UserRole, pp->id());
            m_pp_list->addItem(row);
        }
    });

    menu.exec(m_view->viewport()->mapToGlobal(pos));
}

void MainWindow::show_node_options(int node_id, const QPoint& pos) {
    QMenu menu(this);

    menu.addAction("Rename Node", this, [this, node_id] () { this->rename_node(node_id); });
    menu.addAction("Delete Node", this, [this, node_id] () { this->delete_node(node_id); });

    menu.exec(pos);
}

void MainWindow::show_pp_options(int pp_id, const QPoint& pos) {
    QMenu menu(this);

    menu.addAction("Rename Pinpoint", this, [this, pp_id] () { this->rename_pp(pp_id); });
    menu.addAction("Delete Pinpoint", this, [this, pp_id] () { this->delete_pp(pp_id); });

    menu.exec(pos);
}

QListWidgetItem* MainWindow::find_pp_row(int pp_id) const {
    for (int i = 0; i < m_pp_list->count(); ++i) {
        QListWidgetItem* row = m_pp_list->item(i);
        if (row->data(Qt::UserRole).toInt() == pp_id) return row;
    }
    return nullptr;
}

void MainWindow::delete_pp(int pp_id) {
    auto pp_it = m_pps.find(pp_id);
    if (pp_it == m_pps.end()) {
        ErrorPopup::show(this, QString("Failed to delete pinpoint"), QString("The pinpoint doesn't exist"));
        return;
    }

    PinpointItem* pp_item = pp_it->second;
    m_scene->removeItem(pp_item);
    pp_item->deleteLater();

    if (QListWidgetItem* row = find_pp_row(pp_id)) {
        delete m_pp_list->takeItem(m_pp_list->row(row));
    }

    m_pps.erase(pp_it);
}

void MainWindow::rename_pp(int pp_id) {
    auto pp_it = m_pps.find(pp_id);
    if (pp_it == m_pps.end()) {
        ErrorPopup::show(this, QString("Failed to rename pinpoint"), QString("The pinpoint doesn't exist"));
        return;
    }

    PinpointItem* pp_item = pp_it->second;
    bool ok;
    QString new_name = QInputDialog::getText(this, "Rename Pinpoint", "Enter new name:",
                                             QLineEdit::Normal, pp_item->data()->name(), &ok);

    if (ok && !new_name.trimmed().isEmpty()) {
        pp_item->data()->set_name(new_name);
        pp_item->update();

        if (QListWidgetItem* row = find_pp_row(pp_id)) {
            row->setText(new_name);
        }
    }
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

            (*it)->deleteLater();

            it = m_wires.erase(it);
        } else {
            ++it;
        }
    }

    NodeItem* node_to_kill = node_it->second;

    m_scene->removeItem(node_to_kill);

    node_to_kill->deleteLater();

    m_visual_nodes.erase(node_it);

    m_manager.delete_node(node_id);

    m_scene->update();
}

void MainWindow::on_startWireDrag(int node_id, int port_id, bool is_output, QPointF pos) {
    m_temp_wire = new QGraphicsPathItem();

    QPen pen(Qt::white, 2, Qt::DashLine);
    m_temp_wire->setPen(pen);

    m_scene->addItem(m_temp_wire);
    m_temp_wire->setAcceptedMouseButtons(Qt::NoButton);

    m_drag_start_pos = pos;
    m_drag_source_node = node_id;
    m_drag_source_port = port_id;

    on_dragWire(pos);
}

void MainWindow::on_dragWire(QPointF pos) {
    if (!m_temp_wire) return;

    QPainterPath path;
    path.moveTo(m_drag_start_pos);
    path.lineTo(pos);
    m_temp_wire->setPath(path);

    m_temp_wire->hide();

    QGraphicsItem* item_under_mouse = m_scene->itemAt(pos, QTransform());

    m_temp_wire->show();

    PortItem* port = dynamic_cast<PortItem*>(item_under_mouse);

    if (port && (port->getNodeID() != m_drag_source_node || port->getPortID() != m_drag_source_port)) {
        port->setHovering(true);
        m_hovered_port = port;
    } else {
        m_hovered_port = nullptr;
    }
}

void MainWindow::on_endWireDrag(QPointF drop_scene_pos, int source_node, int source_port, bool is_output) {
    if (m_temp_wire) {
        m_scene->removeItem(m_temp_wire);
        delete m_temp_wire;
        m_temp_wire = nullptr;
    }

    if (m_hovered_port) {
        m_hovered_port->setHovering(false);

        if (m_hovered_port->getNodeID() == source_node && m_hovered_port->getPortID() == source_port) return;
        if (m_hovered_port->isOutput() == is_output) return;

        GraphManager::Port from;
        GraphManager::Port to;

        if (is_output) {
            from = { .node_id = source_node, .port_id = source_port };
            to   = { .node_id = m_hovered_port->getNodeID(), .port_id = m_hovered_port->getPortID() };
        } else {
            from = { .node_id = m_hovered_port->getNodeID(), .port_id = m_hovered_port->getPortID() };
            to   = { .node_id = source_node, .port_id = source_port };
        }

        spawn_wire(from, to);
    }
}

void MainWindow::on_hoverStateChanged(PortItem* port, bool hovering) {
    if (hovering) {
        m_hovered_port = port;
    } else if (m_hovered_port == port) {
        m_hovered_port = nullptr;
    }
}










