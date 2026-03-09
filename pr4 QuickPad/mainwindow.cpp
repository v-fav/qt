#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusBar()->showMessage("Ready");

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNew);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAs);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);

    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::onCut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::onCopy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::onPaste);
    connect(ui->actionSelectAll, &QAction::triggered, this, &MainWindow::onSelectAll);

    ui->actionNew->setStatusTip("Create a new document");
    ui->actionOpen->setStatusTip("Open a document (stub)");
    ui->actionSave->setStatusTip("Save (stub)");
    ui->actionSaveAs->setStatusTip("Save as (stub)");
    ui->actionExit->setStatusTip("Exit QuickPad");

    ui->actionCut->setStatusTip("Cut selection");
    ui->actionCopy->setStatusTip("Copy selection");
    ui->actionPaste->setStatusTip("Paste from clipboard");
    ui->actionSelectAll->setStatusTip("Select all text");

    ui->actionAbout->setStatusTip("About QuickPad");

    setWindowTitle("QuickPad");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNew()
{
    bool ok = false;
    QString name = QInputDialog::getText(
        this,
        "New document",
        "Enter document name:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok)
    {
        statusBar()->showMessage("New: canceled", 2000);
        return;
    }

    name = name.trimmed();
    if (name.isEmpty())
    {
        statusBar()->showMessage("New: empty name", 2000);
        return;
    }

    m_documentName = name;
    ui->editor->clear();
    setWindowTitle("QuickPad - " + m_documentName);
    statusBar()->showMessage("Created: " + m_documentName, 2000);
}

void MainWindow::onOpen()
{
    QMessageBox::information(this, "Open",
                             "Open dialog stub.\n(Real file opening will be added later.)");
    statusBar()->showMessage("Open: stub", 2000);
}

void MainWindow::onSave()
{
    QMessageBox::information(this, "Save",
                             "Save stub.\n(Real saving will be added later.)");
    statusBar()->showMessage("Save: stub", 2000);
}

void MainWindow::onSaveAs()
{
    QMessageBox::information(this, "Save As",
                             "Save As stub.\n(Real saving will be added later.)");
    statusBar()->showMessage("Save As: stub", 2000);
}

void MainWindow::onExit()
{
    QMessageBox::StandardButton r = QMessageBox::question(
        this,
        "Exit",
        "Exit QuickPad?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

    if (r == QMessageBox::Yes)
    {
        statusBar()->showMessage("Exiting...");
        close();
        return;
    }

    statusBar()->showMessage("Exit canceled", 2000);
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About QuickPad",
                       "QuickPad\n\nUI practice project (Qt Widgets).\nMenus, toolbar, dialogs, layouts.");
    statusBar()->showMessage("About shown", 2000);
}

void MainWindow::onCut()
{
    ui->editor->cut();
    statusBar()->showMessage("Cut", 1000);
}

void MainWindow::onCopy()
{
    ui->editor->copy();
    statusBar()->showMessage("Copy", 1000);
}

void MainWindow::onPaste()
{
    ui->editor->paste();
    statusBar()->showMessage("Paste", 1000);
}

void MainWindow::onSelectAll()
{
    ui->editor->selectAll();
    statusBar()->showMessage("Select All", 1000);
}
