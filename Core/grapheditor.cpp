#include "grapheditor.h"

#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGraphicsItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QLineEdit>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QIcon>
#include <QPainterPath>
#include <QRectF>
#include <QTransform>

#include "../UI/graphview.h"
#include "../UI/Items/nodeitem.h"
#include "../UI/Items/portitem.h"
#include "../UI/Items/wireitem.h"
#include "../UI/Items/pinpointitem.h"
#include "../UI/pinpoint.h"
#include "../UI/Dialog/pinpointdialog.h"
#include "../UI/Dialog/errorutility.h"
#include "Nodes/AllNodes.h"

static QIcon makeColorIcon(const QColor& color) {
    QPixmap pix(16, 16);
    pix.fill(color);
    return QIcon(pix);
}

GraphEditor::GraphEditor(QWidget* parent)
    : QObject(parent)
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(-10000, -10000, 20000, 20000);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    m_view = new GraphView(m_scene, parent);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setDragMode(QGraphicsView::NoDrag);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);

    m_pp_list = new QListWidget(parent);

    connect(m_view, &QGraphicsView::customContextMenuRequested,
            this, &GraphEditor::show_context_menu);
    connect(m_pp_list, &QListWidget::itemClicked,
            this, &GraphEditor::on_pp_clicked);
    connect(m_view, &GraphView::deleteRequested,
            this, &GraphEditor::handle_delete);
}

void GraphEditor::spawn_node(NodeType type, QPointF scene_pos) {
    auto [id, label] = creation_helper(type);

    if (id == -1) {
        ErrorPopup::show(m_view, QString("Failed to make node"), QString("The node type doesn't exist"));
        return;
    }

    NodeItem* visual_node = new NodeItem(id, label);

    for (PortItem* port : visual_node->ports()) {
        connect(port, &PortItem::startWireDrag, this, &GraphEditor::on_startWireDrag);
        connect(port, &PortItem::dragWire, this, &GraphEditor::on_dragWire);
        connect(port, &PortItem::endWireDrag, this, &GraphEditor::on_endWireDrag);
        connect(port, &PortItem::hoverStateChanged, this, &GraphEditor::on_hoverStateChanged);
    }

    visual_node->setPos(scene_pos);
    m_scene->addItem(visual_node);

    m_visual_nodes[id] = visual_node;
}

void GraphEditor::spawn_wire(GraphManager::Port from, GraphManager::Port to) {
    auto from_it = m_visual_nodes.find(from.node_id);
    auto to_it = m_visual_nodes.find(to.node_id);

    if (from_it == m_visual_nodes.end() || to_it == m_visual_nodes.end()) {
        ErrorPopup::show(m_view, QString("Failed to make wire"), QString("The nodes doesn't exist"));
        return;
    }

    if (!m_manager.add_connection(from, to)) {
        ErrorPopup::show(m_view, QString("Failed to make wire"), QString("The nodes doesn't exist"));
        return;
    }

    WireItem* visual_wire = new WireItem(
        from_it->second, from.port_id,
        to_it->second, to.port_id
    );

    m_wires.push_back(visual_wire);
    m_scene->addItem(visual_wire);
}

void GraphEditor::show_context_menu(const QPoint& pos) {
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

void GraphEditor::show_bg_context_menu(const QPoint& pos) {
    const QPointF scenePos = m_view->mapToScene(pos);

    QMenu menu(m_view);

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
        PinpointDialog dlg(m_view);
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

void GraphEditor::show_node_options(int node_id, const QPoint& pos) {
    QMenu menu(m_view);

    menu.addAction("Rename Node", this, [this, node_id]() { this->rename_node(node_id); });
    menu.addAction("Delete Node", this, [this, node_id]() { this->delete_node(node_id); });

    menu.exec(pos);
}

void GraphEditor::show_pp_options(int pp_id, const QPoint& pos) {
    QMenu menu(m_view);

    menu.addAction("Edit Pinpoint", this, [this, pp_id]() { this->edit_pp(pp_id); });
    menu.addAction("Delete Pinpoint", this, [this, pp_id]() { this->delete_pp(pp_id); });

    menu.exec(pos);
}

QListWidgetItem* GraphEditor::find_pp_row(int pp_id) const {
    for (int i = 0; i < m_pp_list->count(); ++i) {
        QListWidgetItem* row = m_pp_list->item(i);
        if (row->data(Qt::UserRole).toInt() == pp_id) return row;
    }
    return nullptr;
}

void GraphEditor::delete_pp(int pp_id) {
    auto pp_it = m_pps.find(pp_id);
    if (pp_it == m_pps.end()) {
        ErrorPopup::show(m_view, QString("Failed to delete pinpoint"), QString("The pinpoint doesn't exist"));
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

void GraphEditor::edit_pp(int pp_id) {
    auto pp_it = m_pps.find(pp_id);
    if (pp_it == m_pps.end()) {
        ErrorPopup::show(m_view, QString("Failed to rename pinpoint"), QString("The pinpoint doesn't exist"));
        return;
    }

    PinpointItem* pp_item = pp_it->second;

    PinpointDialog dlg(m_view, "Edit Pinpoint", pp_item->data()->name(), pp_item->data()->color());

    if (dlg.exec() != QDialog::Accepted) return;

    const QString new_name = dlg.name().trimmed();
    if (new_name.isEmpty()) return;

    pp_item->set_name(new_name);
    pp_item->set_color(dlg.color());

    if (QListWidgetItem* row = find_pp_row(pp_id)) {
        row->setText(new_name);
        row->setIcon(makeColorIcon(dlg.color()));
    }
}

void GraphEditor::rename_node(int node_id) {
    auto node_it = m_visual_nodes.find(node_id);

    if (node_it == m_visual_nodes.end()) {
        ErrorPopup::show(m_view, QString("Failed to rename node"), QString("The node doesn't exist"));
        return;
    }

    NodeItem* node = node_it->second;

    bool ok;
    QString new_name = QInputDialog::getText(m_view, "Rename Node", "Enter new name:",
                                             QLineEdit::Normal, node->getName(), &ok);

    if (ok && !new_name.trimmed().isEmpty()) {
        node->set_name(new_name);
        m_manager.set_node_name(node_id, new_name.toStdString());
    }
}

void GraphEditor::delete_node(int node_id) {
    auto node_it = m_visual_nodes.find(node_id);

    if (node_it == m_visual_nodes.end()) {
        ErrorPopup::show(m_view, QString("Failed to remove node"), QString("The node doesn't exist"));
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

void GraphEditor::delete_selected_wires() {
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

void GraphEditor::on_startWireDrag(int node_id, int port_id, bool is_output, QPointF pos) {
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

void GraphEditor::on_dragWire(QPointF pos) {
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

void GraphEditor::on_endWireDrag(QPointF drop_scene_pos, int source_node, int source_port, bool is_output) {
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

void GraphEditor::on_hoverStateChanged(PortItem* port, bool hovering) {
    if (hovering) {
        m_hovered_port = port;
    } else if (m_hovered_port == port) {
        m_hovered_port = nullptr;
    }
}

void GraphEditor::on_pp_clicked(QListWidgetItem* item) {
    int id = item->data(Qt::UserRole).toInt();
    auto it = m_pps.find(id);
    if (it == m_pps.end()) return;
    const QPointF p = it->second->pos();
    const qreal box = 200.0;
    QRectF target(p.x() - box/2, p.y() - box/2, box, box);
    m_view->fitInView(target, Qt::KeepAspectRatio);
}

void GraphEditor::handle_delete() {
    delete_selected_wires();
}

std::pair<int, QString> GraphEditor::creation_helper(NodeType type) {
    switch (type) {
    case (NodeType::Add):
    {
        return { m_manager.create_node<AddNode>("Add"), "Addition" };
    }
    default:
        return { -1, "Unknown" };
    }
}
