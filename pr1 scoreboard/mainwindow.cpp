#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateScores();

    connect(ui->btnAPlus, &QPushButton::clicked, this, [this]{
        scoreA++;
        updateScores();
    });

    connect(ui->btnAMinus, &QPushButton::clicked, this, [this]{
        if (scoreA > 0) scoreA--;
        updateScores();
    });

    connect(ui->btnBPlus, &QPushButton::clicked, this, [this]{
        scoreB++;
        updateScores();
    });

    connect(ui->btnBMinus, &QPushButton::clicked, this, [this]{
        if (scoreB > 0) scoreB--;
        updateScores();
    });

    connect(ui->btnReset, &QPushButton::clicked, this, [this]{
        scoreA = 0;
        scoreB = 0;
        updateScores();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateScores()
{
    ui->labelScoreA->setText(QString::number(scoreA));
    ui->labelScoreB->setText(QString::number(scoreB));
}
