#pragma once

#include <QAbstractTableModel>
#include <QVector>
#include "passwordrecord.h"

class PasswordRepository;

class PasswordTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        IdColumn = 0,
        TitleColumn,
        UsernameColumn,
        PasswordColumn,
        WebsiteColumn,
        CategoryColumn,
        UpdatedAtColumn,
        ColumnCount
    };

    explicit PasswordTableModel(PasswordRepository *repository, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool load(QString *error = nullptr);
    bool saveAll(QString *error = nullptr);
    bool addEmptyRow(const QString &defaultCategory = QString());
    bool deleteRow(int row, QString *error = nullptr);

    QStringList categories() const;

private:
    QVector<PasswordRecord> m_records;
    PasswordRepository *m_repository = nullptr;

    void touchRow(int row);
};
