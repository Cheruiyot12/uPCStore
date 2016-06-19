#ifndef QACRYPT_H
#define QACRYPT_H

#include "qacrypt_global.h"
#include <QByteArray>
#include <openssl/dh.h>
#include <openssl/bn.h>
#include <openssl/err.h>

class QACRYPTSHARED_EXPORT QACrypt
{

public:
    QACrypt();
    static QByteArray sha256(const QByteArray &text);
    static QByteArray encrypt2(const QByteArray &data, const QByteArray &password, const QByteArray &iv);
    static QByteArray decrypt2(const QByteArray &data, const QByteArray &password, const QByteArray &iv);
    static QByteArray genAesKey(int kl);

    //dh

    static QByteArray bn2binArr(const BIGNUM *num);
    static bool binArr2bn(const QByteArray& arr, BIGNUM **num);
};

#endif // QACRYPT_H
