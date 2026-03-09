#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    enum class SourceField { Top, Bottom };

    struct TabBinding {
        QWidget* page = nullptr;
        QLineEdit* topEdit = nullptr;
        QComboBox* topUnit = nullptr;
        QLineEdit* bottomEdit = nullptr;
        QComboBox* bottomUnit = nullptr;
        SourceField lastEdited = SourceField::Top;
    };

    TabBinding tabs_[3];

    void setupBindings();
    void bindTab(int tabIndex);
    void applyValidators();
    void recalc(int tabIndex, SourceField source);
    void recalcUsingLastSource(int tabIndex);

    static bool tryParseDouble(const QString& text, double& out);
    static QString unitKeyFromCombo(QComboBox* combo);
    static QString formatNumber(double v);
    static void setError(QLineEdit* edit, bool isError);
};
#endif // MAINWINDOW_H
