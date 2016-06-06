#include "qacrypt.h"
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>


QACrypt::QACrypt()
{
}

QByteArray QACrypt::sha256(const QByteArray &text)
{
    unsigned int outLen=0;
    QByteArray dataBuff; dataBuff.resize(EVP_MAX_MD_SIZE);
    EVP_MD_CTX evpMdCtx;
    EVP_DigestInit(&evpMdCtx, EVP_sha256());
    EVP_DigestUpdate(&evpMdCtx, text.data(), text.size());
    EVP_DigestFinal_ex(&evpMdCtx, (unsigned char *)dataBuff.data(), &outLen);
    EVP_MD_CTX_cleanup(&evpMdCtx);
    dataBuff.resize(outLen);
    return dataBuff.toHex();
}

QByteArray QACrypt::encrypt2(const QByteArray &data, const QByteArray &password, const QByteArray &iv)
{
    int outLen=0;
    QByteArray dataBuff;dataBuff.resize(data.size()+AES_BLOCK_SIZE);
    EVP_CIPHER_CTX evpCipherCtx;
    EVP_CIPHER_CTX_init(&evpCipherCtx);
    EVP_EncryptInit(&evpCipherCtx,EVP_aes_256_cbc(),(const unsigned char*)sha256(password).data(),(const unsigned char*)sha256("AEScrypt"+iv).data());
    EVP_EncryptUpdate(&evpCipherCtx,(unsigned char*)dataBuff.data(),&outLen,(const unsigned char*)data.data(),data.size());
    int tempLen=outLen;
    EVP_EncryptFinal(&evpCipherCtx,(unsigned char*)dataBuff.data()+tempLen,&outLen);
    tempLen+=outLen;
    EVP_CIPHER_CTX_cleanup(&evpCipherCtx);
    dataBuff.resize(tempLen);
    return dataBuff;
}

QByteArray QACrypt::decrypt2(const QByteArray &data, const QByteArray &password, const QByteArray &iv)
{
    int outLen=0;
    QByteArray dataBuff;dataBuff.resize(data.size()+AES_BLOCK_SIZE);
    EVP_CIPHER_CTX evpCipherCtx;
    EVP_CIPHER_CTX_init(&evpCipherCtx);
    EVP_DecryptInit(&evpCipherCtx,EVP_aes_256_cbc(),(const unsigned char*)sha256(password).data(),(const unsigned char*)sha256("AEScrypt"+iv).data());
    EVP_DecryptUpdate(&evpCipherCtx,(unsigned char*)dataBuff.data(),&outLen,(const unsigned char*)data.data(),data.size());
    int tempLen=outLen;
    EVP_DecryptFinal(&evpCipherCtx,(unsigned char*)dataBuff.data()+tempLen,&outLen);
    tempLen+=outLen;
    EVP_CIPHER_CTX_cleanup(&evpCipherCtx);
    dataBuff.resize(tempLen);
    return dataBuff;
}

QByteArray QACrypt::genAesKey(int kl)
{
        QByteArray k;
        int kll = kl/8;
        k.resize(kll);
        RAND_bytes((unsigned char*)k.data(), kll);
        return k;
}
