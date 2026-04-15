#include "passwordleakchecker.h"

#include <QCryptographicHash>
#include <QNetworkReply>
#include <QPointer>
#include <QTimer>
#include <QUrl>

PasswordLeakChecker::PasswordLeakChecker(QObject *parent)
    : QObject(parent)
{
}

QByteArray PasswordLeakChecker::sha1Hex(const QString &password)
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1)
    .toHex()
        .toUpper();
}

QNetworkRequest PasswordLeakChecker::buildRequest(const QByteArray &prefix) const
{
    const QString url = QStringLiteral("https://api.pwnedpasswords.com/range/%1")
    .arg(QString::fromLatin1(prefix));

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("User-Agent", "PasswordManager/1.0");
    request.setRawHeader("Add-Padding", "true");
    return request;
}

bool PasswordLeakChecker::parseResponse(const QByteArray &payload, const QByteArray &suffix, int *count)
{
    const QList<QByteArray> lines = payload.split('\n');

    for (QByteArray line : lines) {
        line = line.trimmed();
        if (line.isEmpty()) {
            continue;
        }

        const int colonIndex = line.indexOf(':');
        if (colonIndex <= 0) {
            continue;
        }

        const QByteArray apiSuffix = line.left(colonIndex).trimmed();
        bool ok = false;
        const int value = line.mid(colonIndex + 1).trimmed().toInt(&ok);
        if (!ok) {
            continue;
        }

        if (apiSuffix.compare(suffix, Qt::CaseInsensitive) == 0) {
            if (count) {
                *count = value;
            }
            return true;
        }
    }

    return false;
}

void PasswordLeakChecker::checkPassword(const QString &password)
{
    if (password.trimmed().isEmpty()) {
        emit checkFailed("Password is empty.");
        return;
    }

    const QByteArray fullHash = sha1Hex(password);
    const QByteArray prefix = fullHash.left(5);
    const QByteArray suffix = fullHash.mid(5);

    emit checkStarted();

    QNetworkReply *reply = m_network.get(buildRequest(prefix));
    QPointer<QNetworkReply> safeReply(reply);

    QTimer::singleShot(10000, this, [safeReply]() {
        if (safeReply && !safeReply->isFinished()) {
            safeReply->abort();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [this, reply, suffix]() {
        const auto error = reply->error();
        const QByteArray payload = reply->readAll();

        if (error == QNetworkReply::OperationCanceledError) {
            emit checkFailed("Password check timeout.");
        } else if (error != QNetworkReply::NoError) {
            emit checkFailed(reply->errorString());
        } else {
            int count = 0;
            const bool found = parseResponse(payload, suffix, &count);
            emit checkCompleted(found, found ? count : 0);
        }

        reply->deleteLater();
    });
}
