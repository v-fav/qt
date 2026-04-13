#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tickettablemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateActions();
    void on_actionDeleteTicket_triggered();
    void on_actionNewTicket_triggered();
    void on_actionEditTicket_triggered();

private:
    Ui::MainWindow *ui;
    TicketTableModel* model;
};

#endif
