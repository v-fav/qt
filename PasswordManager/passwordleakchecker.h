#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QString>

class PasswordLeakChecker : public QObject
{
    Q_OBJECT

public:
    explicit PasswordLeakChecker(QObject *parent = nullptr);

    void checkPassword(const QString &password);

signals:
    void checkStarted();
    void checkCompleted(bool found, int count);
    void checkFailed(const QString &message);

private:
    static QByteArray sha1Hex(const QString &password);
    static bool parseResponse(const QByteArray &payload, const QByteArray &suffix, int *count);

    QNetworkRequest buildRequest(const QByteArray &prefix) const;

    QNetworkAccessManager m_network;
};
