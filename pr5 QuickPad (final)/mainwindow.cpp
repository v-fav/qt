#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QFileInfo>
#include <QSignalBlocker>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_modified(false)
{
    ui->setupUi(this);

    setupShortcuts();
    setupConnections();
    setupInitialStates();

    statusBar()->showMessage("Ready", 2000);
    updateWindowTitle();
    updateActions();

    ui->editor->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupShortcuts()
{

    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionSaveAs->setShortcut(QKeySequence::SaveAs);
    ui->actionExit->setShortcut(QKeySequence::Quit);

    ui->actionCut->setShortcut(QKeySequence::Cut);
    ui->actionCopy->setShortcut(QKeySequence::Copy);
    ui->actionPaste->setShortcut(QKeySequence::Paste);
    ui->actionSelectAll->setShortcut(QKeySequence::SelectAll);

    ui->actionNew->setShortcutContext(Qt::ApplicationShortcut);
    ui->actionOpen->setShortcutContext(Qt::ApplicationShortcut);
    ui->actionSave->setShortcutContext(Qt::ApplicationShortcut);
    ui->actionSaveAs->setShortcutContext(Qt::ApplicationShortcut);
    ui->actionExit->setShortcutContext(Qt::ApplicationShortcut);

    ui->actionCut->setShortcutContext(Qt::ApplicationShortcut);
    ui->actionCopy->setShortcutContext(Qt::ApplicationShortcut);
    ui->actionPaste->setShortcutContext(Qt::ApplicationShortcut);
    ui->actionSelectAll->setShortcutContext(Qt::ApplicationShortcut);

    addAction(ui->actionNew);
    addAction(ui->actionOpen);
    addAction(ui->actionSave);
    addAction(ui->actionSaveAs);
    addAction(ui->actionExit);

    addAction(ui->actionCut);
    addAction(ui->actionCopy);
    addAction(ui->actionPaste);
    addAction(ui->actionSelectAll);
}

void MainWindow::setupConnections()
{
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onActionNew);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onActionOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSave);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onActionSaveAs);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionExit);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);

    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::onActionCut);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::onActionCopy);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::onActionPaste);
    connect(ui->actionSelectAll, &QAction::triggered, this, &MainWindow::onActionSelectAll);

    connect(ui->editor, &QPlainTextEdit::textChanged, this, &MainWindow::onEditorTextChanged);
    connect(ui->editor, &QPlainTextEdit::copyAvailable, this, &MainWindow::onEditorCopyAvailable);

    connect(QApplication::clipboard(), &QClipboard::dataChanged,
            this, &MainWindow::onClipboardDataChanged);
}

void MainWindow::setupInitialStates()
{
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);

    const QMimeData *md = QApplication::clipboard()->mimeData();
    ui->actionPaste->setEnabled(md && md->hasText());

    ui->actionSave->setEnabled(false);
}

void MainWindow::updateWindowTitle()
{
    QString name;

    if (m_currentFilePath.isEmpty())
        name = "Untitled";
    else
        name = QFileInfo(m_currentFilePath).fileName();

    if (m_modified)
        name += "*";

    setWindowTitle("QuickPad - " + name);
}

void MainWindow::updateActions()
{
    ui->actionSave->setEnabled(m_modified);

    const QMimeData *md = QApplication::clipboard()->mimeData();
    ui->actionPaste->setEnabled(md && md->hasText());
}

void MainWindow::onEditorTextChanged()
{
    if (!m_modified)
    {
        m_modified = true;
        updateWindowTitle();
        updateActions();
    }
}

void MainWindow::onEditorCopyAvailable(bool available)
{
    ui->actionCut->setEnabled(available);
    ui->actionCopy->setEnabled(available);
}

void MainWindow::onClipboardDataChanged()
{
    updateActions();
}

bool MainWindow::loadFromPath(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Open error", file.errorString());
        return false;
    }

    QTextStream in(&file);
    const QString text = in.readAll();
    file.close();

    QSignalBlocker blocker(ui->editor);
    ui->editor->setPlainText(text);

    m_currentFilePath = path;
    m_modified = false;

    updateWindowTitle();
    updateActions();

    statusBar()->showMessage("Opened", 2000);
    ui->editor->setFocus();
    return true;
}

bool MainWindow::saveToPath(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Save error", file.errorString());
        return false;
    }

    QTextStream out(&file);
    out << ui->editor->toPlainText();
    file.close();

    m_currentFilePath = path;
    m_modified = false;

    updateWindowTitle();
    updateActions();

    statusBar()->showMessage("Saved", 2000);
    ui->editor->setFocus();
    return true;
}

bool MainWindow::doSave()
{
    if (m_currentFilePath.isEmpty())
    {
        QString path = QFileDialog::getSaveFileName(
            this, "Save file", "", "Text files (*.txt);;All files (*)"
            );
        if (path.isEmpty())
        {
            statusBar()->showMessage("Save canceled", 2000);
            ui->editor->setFocus();
            return false;
        }
        return saveToPath(path);
    }

    return saveToPath(m_currentFilePath);
}

bool MainWindow::maybeSave()
{
    if (!m_modified)
        return true;

    QMessageBox::StandardButton r = QMessageBox::warning(
        this,
        "Unsaved changes",
        "The document has been modified.\nSave changes?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
        );

    if (r == QMessageBox::Cancel)
        return false;

    if (r == QMessageBox::Save)
        return doSave();

    return true;
}

void MainWindow::onActionNew()
{
    if (!maybeSave())
        return;

    QSignalBlocker blocker(ui->editor);
    ui->editor->clear();

    m_currentFilePath.clear();
    m_modified = false;

    updateWindowTitle();
    updateActions();

    statusBar()->showMessage("New document", 2000);
    ui->editor->setFocus();
}

void MainWindow::onActionOpen()
{
    if (!maybeSave())
        return;

    QString path = QFileDialog::getOpenFileName(
        this, "Open file", "", "Text files (*.txt);;All files (*)"
        );

    if (path.isEmpty())
    {
        statusBar()->showMessage("Open canceled", 2000);
        ui->editor->setFocus();
        return;
    }

    loadFromPath(path);
}

void MainWindow::onActionSave()
{
    doSave();
}

void MainWindow::onActionSaveAs()
{
    QString path = QFileDialog::getSaveFileName(
        this, "Save file as", "", "Text files (*.txt);;All files (*)"
        );

    if (path.isEmpty())
    {
        statusBar()->showMessage("Save As canceled", 2000);
        ui->editor->setFocus();
        return;
    }

    saveToPath(path);
}

void MainWindow::onActionExit()
{
    if (maybeSave())
        close();
    else
        statusBar()->showMessage("Exit canceled", 2000);

    ui->editor->setFocus();
}

void MainWindow::onActionAbout()
{
    QMessageBox::about(this, "About QuickPad",
                       "QuickPad\n\nPR5: Actions, shortcuts, Open/Save, dirty state, keyboard-first UX.");
    ui->editor->setFocus();
}

void MainWindow::onActionCut()
{
    ui->editor->cut();
    statusBar()->showMessage("Cut", 1000);
    ui->editor->setFocus();
}

void MainWindow::onActionCopy()
{
    ui->editor->copy();
    statusBar()->showMessage("Copy", 1000);
    ui->editor->setFocus();
}

void MainWindow::onActionPaste()
{
    ui->editor->paste();
    statusBar()->showMessage("Paste", 1000);
    ui->editor->setFocus();
}

void MainWindow::onActionSelectAll()
{
    ui->editor->selectAll();
    statusBar()->showMessage("Select All", 1000);
    ui->editor->setFocus();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}
