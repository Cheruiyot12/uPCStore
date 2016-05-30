#ifndef CRYPTOR_H
#define CRYPTOR_H


#include <QObject>
#include <QString>
#include <QDebug>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <string.h>
#include <algorithm>
#include <QCryptographicHash>
#include <openssl/rand.h>

struct pubKey{
    char* n_b;
    int n_size;

    char* e_b;
    int b_size;
};



class Cryptor : public QObject
{
    Q_OBJECT
public:
    explicit Cryptor(QObject *parent = 0);

    enum keyLen{
        key256,
        key128
    };
    static pubKey genpk(RSA *rsa);

signals:

public slots:
    static QByteArray encryptAes(QString plain, QByteArray kkey, QByteArray ivv);
    static QString decryptAes(QByteArray ciph, QByteArray kkey, QByteArray ivv);
    static QByteArray genAesKey(int kl);

    static RSA *constructEncrypt(pubKey pk);
    static QByteArray encr(QString inp, RSA* alg);
    static QByteArray encr(QByteArray inp, RSA* alg);
    static QString decr(QByteArray cip, RSA* alg);




};

#endif // CRYPTOR_H
