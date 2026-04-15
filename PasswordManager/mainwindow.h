#pragma once

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DatabaseManager;
class PasswordRepository;
class PasswordTableModel;
class PasswordFilterProxyModel;
class PasswordLeakChecker;
class QAction;
class QProgressDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewClicked();
    void onEditClicked();
    void onDeleteClicked();
    void onSaveClicked();
    void onExitClicked();

    void onCheckPasswordClicked();

    void applyFilters();
    void clearFilters();
    void updateEmptyState();

private:
    Ui::MainWindow *ui = nullptr;

    DatabaseManager *m_dbManager = nullptr;
    std::unique_ptr<PasswordRepository> m_repository;
    PasswordTableModel *m_model = nullptr;
    PasswordFilterProxyModel *m_proxy = nullptr;

    PasswordLeakChecker *m_passwordLeakChecker = nullptr;
    QAction *m_checkPasswordAction = nullptr;
    QProgressDialog *m_checkPasswordDialog = nullptr;

    bool initDatabase();
    void initModel();
    void initConnections();
    void initSecurityChecker();
    void refreshCategoryFilter();
    void reloadTable();
    void showError(const QString &title, const QString &message);
};
