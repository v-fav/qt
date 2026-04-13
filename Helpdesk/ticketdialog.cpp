#include "TicketDialog.h"
#include "ui_ticketdialog.h"

TicketDialog::TicketDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TicketDialog)
{
    ui->setupUi(this);

    connect(ui->titleLineEdit, &QLineEdit::textChanged,
            this, &TicketDialog::onFormChanged);

    connect(ui->descriptionTextEdit, &QTextEdit::textChanged,
            this, &TicketDialog::onFormChanged);

    connect(ui->priorityComboBox, &QComboBox::currentTextChanged,
            this, &TicketDialog::onFormChanged);

    connect(ui->statusComboBox, &QComboBox::currentTextChanged,
            this, &TicketDialog::onFormChanged);

    connect(ui->editButton, &QPushButton::clicked,
            this, &TicketDialog::onEditClicked);

    connect(ui->closeButton, &QPushButton::clicked,
            this, &TicketDialog::onCloseClicked);
}

TicketDialog::~TicketDialog()
{
    delete ui;
}

void TicketDialog::setMode(Mode mode)
{
    mode_ = mode;
    updateUiForMode();
}

void TicketDialog::updateUiForMode()
{
    bool editable = (mode_ == Mode::Edit || mode_ == Mode::Create);

    ui->titleLineEdit->setReadOnly(!editable);
    ui->descriptionTextEdit->setReadOnly(!editable);
    ui->priorityComboBox->setEnabled(editable);
    ui->statusComboBox->setEnabled(editable);

    ui->editButton->setVisible(mode_ == Mode::View);

    if (mode_ == Mode::View) {
        ui->closeButton->setText("Close");
    } else {
        ui->closeButton->setText("Save");
    }

    updateButtonsState();
}

bool TicketDialog::isFormValid() const
{
    return !ui->titleLineEdit->text().trimmed().isEmpty();
}

void TicketDialog::updateButtonsState()
{
    if (mode_ == Mode::View)
        return;

    ui->closeButton->setEnabled(isFormValid());
}

void TicketDialog::onFormChanged()
{
    updateButtonsState();
}

void TicketDialog::loadTicket(const Ticket &t)
{
    ui->idValueLabel->setText(QString::number(t.id));
    ui->titleLineEdit->setText(t.title);
    ui->priorityComboBox->setCurrentText(t.priority);
    ui->statusComboBox->setCurrentText(t.status);
    ui->createdValueLabel->setText(t.createdAt);
}

Ticket TicketDialog::collectTicket() const
{
    Ticket t;
    t.id = ui->idValueLabel->text().toInt();
    t.title = ui->titleLineEdit->text();
    t.priority = ui->priorityComboBox->currentText();
    t.status = ui->statusComboBox->currentText();
    t.createdAt = ui->createdValueLabel->text();
    return t;
}

void TicketDialog::onEditClicked()
{
    setMode(Mode::Edit);
}

void TicketDialog::onCloseClicked()
{
    if (mode_ == Mode::View) {
        close();
        return;
    }

    if (!isFormValid())
        return;

    Ticket t = collectTicket();

    if (mode_ == Mode::Create)
        emit createRequested(t);
    else
        emit updateRequested(t);

    close();
}
