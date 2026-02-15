/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_7;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *editLengthTop;
    QComboBox *comboLengthTopUnit;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *editLengthBottom;
    QComboBox *comboLengthBottomUnit;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *editMassTop;
    QComboBox *comboMassTopUnit;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *editMassBottom;
    QComboBox *comboMassBottomUnit;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *editTempTop;
    QComboBox *comboTempTopUnit;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *editTempBottom;
    QComboBox *comboTempBottomUnit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(520, 260);
        MainWindow->setMinimumSize(QSize(520, 260));
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_7 = new QVBoxLayout(centralwidget);
        verticalLayout_7->setObjectName("verticalLayout_7");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        tab_3->setEnabled(true);
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(23);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        editLengthTop = new QLineEdit(tab_3);
        editLengthTop->setObjectName("editLengthTop");
        editLengthTop->setMinimumSize(QSize(271, 40));

        horizontalLayout->addWidget(editLengthTop);

        comboLengthTopUnit = new QComboBox(tab_3);
        comboLengthTopUnit->addItem(QString());
        comboLengthTopUnit->addItem(QString());
        comboLengthTopUnit->addItem(QString());
        comboLengthTopUnit->addItem(QString());
        comboLengthTopUnit->addItem(QString());
        comboLengthTopUnit->setObjectName("comboLengthTopUnit");
        comboLengthTopUnit->setMinimumSize(QSize(180, 40));

        horizontalLayout->addWidget(comboLengthTopUnit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        editLengthBottom = new QLineEdit(tab_3);
        editLengthBottom->setObjectName("editLengthBottom");
        editLengthBottom->setMinimumSize(QSize(271, 40));

        horizontalLayout_2->addWidget(editLengthBottom);

        comboLengthBottomUnit = new QComboBox(tab_3);
        comboLengthBottomUnit->addItem(QString());
        comboLengthBottomUnit->addItem(QString());
        comboLengthBottomUnit->addItem(QString());
        comboLengthBottomUnit->addItem(QString());
        comboLengthBottomUnit->addItem(QString());
        comboLengthBottomUnit->setObjectName("comboLengthBottomUnit");
        comboLengthBottomUnit->setMinimumSize(QSize(180, 40));

        horizontalLayout_2->addWidget(comboLengthBottomUnit);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_4->addLayout(verticalLayout);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        verticalLayout_5 = new QVBoxLayout(tab_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(23);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        editMassTop = new QLineEdit(tab_4);
        editMassTop->setObjectName("editMassTop");
        editMassTop->setMinimumSize(QSize(271, 40));

        horizontalLayout_3->addWidget(editMassTop);

        comboMassTopUnit = new QComboBox(tab_4);
        comboMassTopUnit->addItem(QString());
        comboMassTopUnit->addItem(QString());
        comboMassTopUnit->addItem(QString());
        comboMassTopUnit->setObjectName("comboMassTopUnit");
        comboMassTopUnit->setMinimumSize(QSize(180, 40));

        horizontalLayout_3->addWidget(comboMassTopUnit);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        editMassBottom = new QLineEdit(tab_4);
        editMassBottom->setObjectName("editMassBottom");
        editMassBottom->setMinimumSize(QSize(271, 40));

        horizontalLayout_4->addWidget(editMassBottom);

        comboMassBottomUnit = new QComboBox(tab_4);
        comboMassBottomUnit->addItem(QString());
        comboMassBottomUnit->addItem(QString());
        comboMassBottomUnit->addItem(QString());
        comboMassBottomUnit->setObjectName("comboMassBottomUnit");
        comboMassBottomUnit->setMinimumSize(QSize(180, 40));

        horizontalLayout_4->addWidget(comboMassBottomUnit);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout_5->addLayout(verticalLayout_2);

        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        verticalLayout_6 = new QVBoxLayout(tab_5);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(23);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        editTempTop = new QLineEdit(tab_5);
        editTempTop->setObjectName("editTempTop");
        editTempTop->setMinimumSize(QSize(271, 40));

        horizontalLayout_5->addWidget(editTempTop);

        comboTempTopUnit = new QComboBox(tab_5);
        comboTempTopUnit->addItem(QString());
        comboTempTopUnit->addItem(QString());
        comboTempTopUnit->addItem(QString());
        comboTempTopUnit->setObjectName("comboTempTopUnit");
        comboTempTopUnit->setMinimumSize(QSize(180, 40));

        horizontalLayout_5->addWidget(comboTempTopUnit);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        editTempBottom = new QLineEdit(tab_5);
        editTempBottom->setObjectName("editTempBottom");
        editTempBottom->setMinimumSize(QSize(271, 40));

        horizontalLayout_6->addWidget(editTempBottom);

        comboTempBottomUnit = new QComboBox(tab_5);
        comboTempBottomUnit->addItem(QString());
        comboTempBottomUnit->addItem(QString());
        comboTempBottomUnit->addItem(QString());
        comboTempBottomUnit->setObjectName("comboTempBottomUnit");
        comboTempBottomUnit->setMinimumSize(QSize(180, 40));

        horizontalLayout_6->addWidget(comboTempBottomUnit);


        verticalLayout_3->addLayout(horizontalLayout_6);


        verticalLayout_6->addLayout(verticalLayout_3);

        tabWidget->addTab(tab_5, QString());

        verticalLayout_7->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 520, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Converter", nullptr));
        comboLengthTopUnit->setItemText(0, QCoreApplication::translate("MainWindow", "meters (m)", nullptr));
        comboLengthTopUnit->setItemText(1, QCoreApplication::translate("MainWindow", "kilometers (km)", nullptr));
        comboLengthTopUnit->setItemText(2, QCoreApplication::translate("MainWindow", "inches (in)", nullptr));
        comboLengthTopUnit->setItemText(3, QCoreApplication::translate("MainWindow", "feet (ft)", nullptr));
        comboLengthTopUnit->setItemText(4, QCoreApplication::translate("MainWindow", "miles (mi)", nullptr));

        comboLengthBottomUnit->setItemText(0, QCoreApplication::translate("MainWindow", "meters (m)", nullptr));
        comboLengthBottomUnit->setItemText(1, QCoreApplication::translate("MainWindow", "kilometers (km)", nullptr));
        comboLengthBottomUnit->setItemText(2, QCoreApplication::translate("MainWindow", "inches (in)", nullptr));
        comboLengthBottomUnit->setItemText(3, QCoreApplication::translate("MainWindow", "feet (ft)", nullptr));
        comboLengthBottomUnit->setItemText(4, QCoreApplication::translate("MainWindow", "miles (mi)", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Length", nullptr));
        comboMassTopUnit->setItemText(0, QCoreApplication::translate("MainWindow", "kilograms (kg)", nullptr));
        comboMassTopUnit->setItemText(1, QCoreApplication::translate("MainWindow", "pounds (lb)", nullptr));
        comboMassTopUnit->setItemText(2, QCoreApplication::translate("MainWindow", "ounces (oz)", nullptr));

        comboMassBottomUnit->setItemText(0, QCoreApplication::translate("MainWindow", "kilograms (kg)", nullptr));
        comboMassBottomUnit->setItemText(1, QCoreApplication::translate("MainWindow", "pounds (lb)", nullptr));
        comboMassBottomUnit->setItemText(2, QCoreApplication::translate("MainWindow", "ounces (oz)", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "Mass", nullptr));
        comboTempTopUnit->setItemText(0, QCoreApplication::translate("MainWindow", "Celsius (\302\260C)", nullptr));
        comboTempTopUnit->setItemText(1, QCoreApplication::translate("MainWindow", "Fahrenheit (\302\260F)", nullptr));
        comboTempTopUnit->setItemText(2, QCoreApplication::translate("MainWindow", "Kelvin (K)", nullptr));

        comboTempBottomUnit->setItemText(0, QCoreApplication::translate("MainWindow", "Celsius (\302\260C)", nullptr));
        comboTempBottomUnit->setItemText(1, QCoreApplication::translate("MainWindow", "Fahrenheit (\302\260F)", nullptr));
        comboTempBottomUnit->setItemText(2, QCoreApplication::translate("MainWindow", "Kelvin (K)", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainWindow", "Temperature", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
