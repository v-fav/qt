#include "referencewidget.h"
#include "ui_referencewidget.h"
#include "services/settingsmanager.h"
#include <QHeaderView>
#include <QPushButton>

ReferenceWidget::ReferenceWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReferenceWidget)
{
    ui->setupUi(this);

    for (QPushButton* button :
         findChildren<QPushButton*>())
    {
        button->setFocusPolicy(
            Qt::NoFocus);
    }

    ui->symbolTable->verticalHeader()->setVisible(false);

    encoder.loadSymbols("../../data/symbols.txt");

    ui->categoryFilter->addItem("All");
    ui->categoryFilter->addItem("Letters");
    ui->categoryFilter->addItem("Numbers");

    connect(ui->searchField,
            &QLineEdit::textChanged,
            this,
            [this]()
            {
                loadTable();
            });

    connect(ui->categoryFilter,
            &QComboBox::currentTextChanged,
            this,
            [this]()
            {
                loadTable();
            });

    connect(ui->backButton,
            &QPushButton::clicked,
            this,
            [this]()
            {
                emit backRequested();
            });

    loadTable();

    ui->symbolTable->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
}

bool ReferenceWidget::matchesCategory(
    const Symbol& symbol) const
{
    QString appLanguage =
        SettingsManager::language();

    if (appLanguage == "English")
    {
        if (symbol.language != "EN" &&
            symbol.language != "NUM")
        {
            return false;
        }
    }

    if (appLanguage == "Ukrainian")
    {
        if (symbol.language != "UA" &&
            symbol.language != "NUM")
        {
            return false;
        }
    }

    QString category =
        ui->categoryFilter->currentText();

    if (category == "All")
        return true;

    if (category == "Numbers")
        return symbol.language == "NUM";

    if (category == "Letters")
        return symbol.language != "NUM";

    return true;
}

void ReferenceWidget::loadTable()
{
    QList<Symbol> symbols =
        encoder.getSymbols();

    QString searchText =
        ui->searchField->text().trimmed();

    ui->symbolTable->setRowCount(0);

    int row = 0;

    for (const Symbol& symbol : symbols)
    {
        if (!matchesCategory(symbol))
            continue;

        if (!searchText.isEmpty())
        {
            bool found =
                symbol.character.contains(
                    searchText,
                    Qt::CaseInsensitive)
                ||
                symbol.morseCode.contains(
                    searchText);

            if (!found)
                continue;
        }

        ui->symbolTable->insertRow(row);

        QTableWidgetItem* symbolItem =
            new QTableWidgetItem(symbol.character);

        symbolItem->setTextAlignment(Qt::AlignCenter);

        ui->symbolTable->setItem(
            row,
            0,
            symbolItem);

        QTableWidgetItem* morseItem =
            new QTableWidgetItem(symbol.morseCode);

        morseItem->setTextAlignment(Qt::AlignCenter);

        ui->symbolTable->setItem(
            row,
            1,
            morseItem);

        row++;
    }
}

void ReferenceWidget::reloadData()
{
    loadTable();
}

ReferenceWidget::~ReferenceWidget()
{
    delete ui;
}
