#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "services/settingsmanager.h"
#include "dialogs/confirmresetdialog.h"
#include "database/databasemanager.h"
#include <QPushButton>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    for (QPushButton* button :
         findChildren<QPushButton*>())
    {
        button->setFocusPolicy(
            Qt::NoFocus);
    }

    ui->languageComboBox->setCurrentText(
        SettingsManager::language());

    ui->keyboardInputCheckBox->setChecked(
        SettingsManager::keyboardInputEnabled());

    ui->spaceInputCheckBox->setChecked(
        SettingsManager::spaceInputEnabled());

    connect(ui->saveButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                SettingsManager::setLanguage(
                    ui->languageComboBox->currentText());

                SettingsManager::setKeyboardInputEnabled(
                    ui->keyboardInputCheckBox->isChecked());

                SettingsManager::setSpaceInputEnabled(
                    ui->spaceInputCheckBox->isChecked());

                accept();
            });

    connect(ui->backButton,
            &QPushButton::clicked,
            this,
            &QDialog::reject);

    connect(ui->resetProgressButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                ConfirmResetDialog dialog(this);

                connect(&dialog,
                        &ConfirmResetDialog::confirmed,
                        this,
                        []()
                        {
                            DatabaseManager::resetStatistics();
                        });

                dialog.exec();
            });

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}
