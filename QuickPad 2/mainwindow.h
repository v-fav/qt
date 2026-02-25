#pragma once

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onActionNew();
    void onActionOpen();
    void onActionSave();
    void onActionSaveAs();
    void onActionExit();
    void onActionAbout();

    void onActionCut();
    void onActionCopy();
    void onActionPaste();
    void onActionSelectAll();

    void onEditorTextChanged();
    void onEditorCopyAvailable(bool available);
    void onClipboardDataChanged();

private:
    void setupShortcuts();
    void setupConnections();
    void setupInitialStates();
    void updateWindowTitle();
    void updateActions();

    bool maybeSave();
    bool saveToPath(const QString &path);
    bool loadFromPath(const QString &path);
    bool doSave();

private:
    Ui::MainWindow *ui;

    QString m_currentFilePath;
    bool m_modified;
};
