#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Core/grapheditor.h"
#include "graphview.h"

#include <QDockWidget>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow())
{
    m_ui->setupUi(this);

    m_editor = new GraphEditor(this);
    setCentralWidget(m_editor->view());

    m_pp_dock = new QDockWidget("Pinpoints", this);
    m_pp_dock->setWidget(m_editor->pinpoint_list());
    addDockWidget(Qt::LeftDockWidgetArea, m_pp_dock);

    menuBar()->addMenu("View")->addAction(m_pp_dock->toggleViewAction());
}

MainWindow::~MainWindow() {
    delete m_ui;
}
