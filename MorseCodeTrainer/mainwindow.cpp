#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/aboutdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionSettings,
            &QAction::triggered,
            this,
            [this]()
            {
                SettingsDialog dlg(this);

                if (dlg.exec() == QDialog::Accepted)
                {
                    referenceWidget->reloadData();

                    lessonSelectionWidget->reloadTests();

                    statisticsWidget->reloadStatistics();

                    dashboardWidget->updateProgress();
                }
            });

    connect(ui->actionAbout,
            &QAction::triggered,
            this,
            [this]()
            {
                AboutDialog dlg(this);
                dlg.exec();
            });

    setupPages();
    setupConnections();
}

void MainWindow::setupPages()
{
    dashboardWidget = new DashboardWidget(this);
    lessonSelectionWidget = new LessonSelectionWidget(this);
    trainingWidget = new TrainingWidget(this);
    referenceWidget = new ReferenceWidget(this);
    statisticsWidget = new StatisticsWidget(this);

    ui->stackedWidget->addWidget(dashboardWidget);
    ui->stackedWidget->addWidget(lessonSelectionWidget);
    ui->stackedWidget->addWidget(trainingWidget);
    ui->stackedWidget->addWidget(referenceWidget);
    ui->stackedWidget->addWidget(statisticsWidget);

    ui->stackedWidget->setCurrentWidget(dashboardWidget);
}

void MainWindow::setupConnections()
{
    connect(dashboardWidget,
            &DashboardWidget::startLearnClicked,
            this,
            [this]()
            {
                ui->stackedWidget
                    ->setCurrentWidget(lessonSelectionWidget);
            });

    connect(dashboardWidget,
            &DashboardWidget::referenceClicked,
            this,
            [this]()
            {
                ui->stackedWidget
                    ->setCurrentWidget(referenceWidget);
            });

    connect(dashboardWidget,
            &DashboardWidget::statisticsClicked,
            this,
            [this]()
            {
                statisticsWidget->reloadStatistics();

                ui->stackedWidget
                    ->setCurrentWidget(statisticsWidget);
            });

    connect(lessonSelectionWidget,
            &LessonSelectionWidget::backRequested,
            this,
            [this]()
            {
                ui->stackedWidget
                    ->setCurrentWidget(dashboardWidget);
            });

    connect(lessonSelectionWidget,
            &LessonSelectionWidget::startLessonRequested,
            this,
            [this]()
            {
                trainingWidget->setTest(
                    lessonSelectionWidget->selectedTest());

                ui->stackedWidget
                    ->setCurrentWidget(trainingWidget);
            });

    connect(trainingWidget,
            &TrainingWidget::exitRequested,
            this,
            [this]()
            {
                ui->stackedWidget
                    ->setCurrentWidget(dashboardWidget);
            });

    connect(trainingWidget,
            &TrainingWidget::dashboardRequested,
            this,
            [this]()
            {
                dashboardWidget->updateProgress();

                ui->stackedWidget->setCurrentWidget(
                    dashboardWidget);
            });

    connect(referenceWidget,
            &ReferenceWidget::backRequested,
            this,
            [this]()
            {
                ui->stackedWidget->setCurrentWidget(
                    dashboardWidget);
            });

    connect(statisticsWidget,
            &StatisticsWidget::backRequested,
            this,
            [this]()
            {
                ui->stackedWidget
                    ->setCurrentWidget(dashboardWidget);
            });

    connect(dashboardWidget,
            &DashboardWidget::settingsClicked,
            this,
            [this]()
            {
                SettingsDialog dlg(this);

                if (dlg.exec() == QDialog::Accepted)
                {
                    referenceWidget->reloadData();

                    lessonSelectionWidget->reloadTests();

                    statisticsWidget->reloadStatistics();

                    dashboardWidget->updateProgress();
                }
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}
