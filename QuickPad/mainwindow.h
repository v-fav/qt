#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onExit();

    void onAbout();

    void onCut();
    void onCopy();
    void onPaste();
    void onSelectAll();

private:
    Ui::MainWindow *ui;

    QString m_documentName;
};
