#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "converter.h"

#include <QDoubleValidator>
#include <QSignalBlocker>
#include <QLocale>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupBindings();
    applyValidators();

    recalcUsingLastSource(ui->tabWidget->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupBindings()
{
    for (int i = 0; i < 3; ++i) bindTab(i);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int idx){
        recalcUsingLastSource(idx);
    });
}

static QStringList prefixCandidatesForTab(int tabIndex)
{
    if (tabIndex == 0) return {"Length"};
    if (tabIndex == 1) return {"Mass"};
    return {"Temp", "Temperature"};
}

void MainWindow::bindTab(int tabIndex)
{
    TabBinding& t = tabs_[tabIndex];
    t.page = ui->tabWidget->widget(tabIndex);

    auto tryBind = [&](const QString& prefix) -> bool {
        const QString topEditName    = "edit"  + prefix + "Top";
        const QString bottomEditName = "edit"  + prefix + "Bottom";
        const QString topUnitName    = "combo" + prefix + "TopUnit";
        const QString bottomUnitName = "combo" + prefix + "BottomUnit";

        t.topEdit    = t.page->findChild<QLineEdit*>(topEditName);
        t.bottomEdit = t.page->findChild<QLineEdit*>(bottomEditName);
        t.topUnit    = t.page->findChild<QComboBox*>(topUnitName);
        t.bottomUnit = t.page->findChild<QComboBox*>(bottomUnitName);

        if (t.topEdit && t.bottomEdit && t.topUnit && t.bottomUnit)
            return true;

        const QString topUnitName2    = "combo" + prefix + "Top";
        const QString bottomUnitName2 = "combo" + prefix + "Bottom";

        if (!t.topUnit)    t.topUnit    = t.page->findChild<QComboBox*>(topUnitName2);
        if (!t.bottomUnit) t.bottomUnit = t.page->findChild<QComboBox*>(bottomUnitName2);

        return (t.topEdit && t.bottomEdit && t.topUnit && t.bottomUnit);
    };

    QStringList prefixes;
    if (tabIndex == 0) prefixes = {"Length"};
    else if (tabIndex == 1) prefixes = {"Mass"};
    else prefixes = {"Temp", "Temperature"};

    bool ok = false;
    QString usedPrefix;

    for (int i = 0; i < prefixes.size(); ++i) {
        const QString& p = prefixes.at(i);
        t.topEdit = nullptr;
        t.bottomEdit = nullptr;
        t.topUnit = nullptr;
        t.bottomUnit = nullptr;

        if (tryBind(p)) {
            ok = true;
            usedPrefix = p;
            break;
        }
    }

    if (!ok) {
        qDebug() << "Bind FAILED for tab" << tabIndex
                 << "Tried prefixes:" << prefixes
                 << "Page objectName:" << t.page->objectName();
        return;
    }

    qDebug() << "bindTab OK tabIndex=" << tabIndex
             << "usedPrefix=" << usedPrefix
             << "topEdit=" << t.topEdit->objectName()
             << "bottomEdit=" << t.bottomEdit->objectName()
             << "topUnit=" << t.topUnit->objectName()
             << "bottomUnit=" << t.bottomUnit->objectName();

    connect(t.topEdit, &QLineEdit::textChanged, this, [this, tabIndex](const QString&){
        tabs_[tabIndex].lastEdited = SourceField::Top;
        recalc(tabIndex, SourceField::Top);
    });

    connect(t.bottomEdit, &QLineEdit::textChanged, this, [this, tabIndex](const QString&){
        tabs_[tabIndex].lastEdited = SourceField::Bottom;
        recalc(tabIndex, SourceField::Bottom);
    });

    auto unitChanged = [this, tabIndex]{
        recalcUsingLastSource(tabIndex);
    };

    connect(t.topUnit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, unitChanged);
    connect(t.bottomUnit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, unitChanged);
}

void MainWindow::applyValidators()
{
    for (int i = 0; i < 3; ++i) {
        if (!tabs_[i].topEdit || !tabs_[i].bottomEdit) {
            qDebug() << "applyValidators skipped tab" << i << "(bindings missing)";
            continue;
        }

        auto* v1 = new QDoubleValidator(this);
        v1->setNotation(QDoubleValidator::StandardNotation);

        auto* v2 = new QDoubleValidator(this);
        v2->setNotation(QDoubleValidator::StandardNotation);

        tabs_[i].topEdit->setValidator(v1);
        tabs_[i].bottomEdit->setValidator(v2);
    }
}

void MainWindow::recalcUsingLastSource(int tabIndex)
{
    if (tabIndex < 0 || tabIndex > 2) return;
    recalc(tabIndex, tabs_[tabIndex].lastEdited);
}

void MainWindow::recalc(int tabIndex, SourceField source)
{
    TabBinding& t = tabs_[tabIndex];
    if (!t.topEdit || !t.bottomEdit || !t.topUnit || !t.bottomUnit) return;

    QLineEdit* srcEdit = (source == SourceField::Top) ? t.topEdit : t.bottomEdit;
    QLineEdit* dstEdit = (source == SourceField::Top) ? t.bottomEdit : t.topEdit;

    QComboBox* srcUnit = (source == SourceField::Top) ? t.topUnit : t.bottomUnit;
    QComboBox* dstUnit = (source == SourceField::Top) ? t.bottomUnit : t.topUnit;

    const QString text = srcEdit->text().trimmed();
    if (text.isEmpty()) {
        setError(srcEdit, false);
        return;
    }

    double value = 0.0;
    if (!tryParseDouble(text, value)) {
        setError(srcEdit, true);
        return;
    }
    setError(srcEdit, false);

    Converter::Mode mode = Converter::Mode::Length;
    if (tabIndex == 1) mode = Converter::Mode::Mass;
    if (tabIndex == 2) mode = Converter::Mode::Temperature;

    const QString fromU = unitKeyFromCombo(srcUnit);
    const QString toU   = unitKeyFromCombo(dstUnit);

    double result = 0.0;
    try {
        result = Converter::convert(mode, value, fromU, toU);
    } catch (...) {
        return;
    }

    const QString outText = formatNumber(result);

    QSignalBlocker b(dstEdit);
    dstEdit->setText(outText);
}

bool MainWindow::tryParseDouble(const QString& text, double& out)
{
    QString s = text.trimmed();
    if (s.contains(',')) s.replace(',', '.');

    bool ok = false;
    out = QLocale::c().toDouble(s, &ok);
    return ok;
}

QString MainWindow::unitKeyFromCombo(QComboBox* combo)
{
    const QVariant d = combo->currentData();
    if (d.isValid() && !d.toString().isEmpty())
        return d.toString();

    const QString t = combo->currentText();
    int l = t.lastIndexOf('(');
    int r = t.lastIndexOf(')');
    if (l != -1 && r != -1 && r > l + 1) {
        QString key = t.mid(l + 1, r - l - 1).trimmed();
        key.remove(QChar(0x00B0));
        return key;
    }
    return t.trimmed();
}

QString MainWindow::formatNumber(double v)
{
    return QString::number(v, 'g', 10);
}

void MainWindow::setError(QLineEdit* edit, bool isError)
{
    if (isError) edit->setStyleSheet("border: 1px solid #d9534f;");
    else edit->setStyleSheet("");
}
