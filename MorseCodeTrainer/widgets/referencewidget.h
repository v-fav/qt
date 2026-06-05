#ifndef REFERENCEWIDGET_H
#define REFERENCEWIDGET_H

#include <QDialog>
#include "services/morseencoder.h"

namespace Ui {
class ReferenceWidget;
}

class ReferenceWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ReferenceWidget(QWidget *parent = nullptr);
    ~ReferenceWidget();
    void reloadData();

signals:
    void backRequested();

private:
    Ui::ReferenceWidget *ui;
    MorseEncoder encoder;

    void loadTable();
    bool matchesCategory(const Symbol& symbol) const;
};

#endif // REFERENCEWIDGET_H
