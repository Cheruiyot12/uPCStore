#ifndef QACRYPT_H
#define QACRYPT_H

#include "qacrypt_global.h"
#include <QByteArray>

class QACRYPTSHARED_EXPORT QACrypt
{

public:
    QACrypt();
    static QByteArray sha256(const QByteArray &text);
    static QByteArray encrypt2(const QByteArray &data, const QByteArray &password, const QByteArray &iv);
    static QByteArray decrypt2(const QByteArray &data, const QByteArray &password, const QByteArray &iv);
    static QByteArray genAesKey(int kl);
};

#endif // QACRYPT_H
