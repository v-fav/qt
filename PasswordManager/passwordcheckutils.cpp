#include "passwordcheckutils.h"

#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>

static QByteArray sha1Hex(const QString &password)
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1)
    .toHex()
        .toUpper();
}

CheckResult checkPasswordSync(const QString &password)
{
    CheckResult result;

    if (password.trimmed().isEmpty()) {
        result.error = "Empty password";
        return result;
    }

    QByteArray hash = sha1Hex(password);
    QByteArray prefix = hash.left(5);
    QByteArray suffix = hash.mid(5);

    QNetworkAccessManager manager;

    QUrl url(QString("https://api.pwnedpasswords.com/range/%1").arg(QString(prefix)));
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "PasswordManager");

    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        result.error = reply->errorString();
        reply->deleteLater();
        return result;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    const auto lines = data.split('\n');

    for (auto line : lines)
    {
        auto parts = line.split(':');
        if (parts.size() != 2)
            continue;

        if (parts[0].trimmed().compare(suffix, Qt::CaseInsensitive) == 0)
        {
            result.found = true;
            result.count = parts[1].trimmed().toInt();
            return result;
        }
    }

    result.found = false;
    return result;
}
