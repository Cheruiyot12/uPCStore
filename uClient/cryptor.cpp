#include "cryptor.h"
#include "algorithm"

Cryptor::Cryptor(QObject *parent) : QObject(parent)
{

}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

QByteArray Cryptor::encryptAes(QString plain, QByteArray kkey, QByteArray ivv)
{
    char *key = new  char[32];
    char* iv = new  char[16];
    strcpy(key, kkey.data());
    strcpy(iv, ivv.data());

    QByteArray plainn = QByteArray::fromStdString(plain.toStdString());
    char *plaintext = new char[plainn.size()*2];
    strcpy(plaintext, plainn.data());

    char *ciphertext = new char[(plainn.size()*2)];
    int ciphertext_len;

    ciphertext_len = encrypt((unsigned char*)plainn.data(), plainn.size(), (unsigned char*)key, (unsigned char*)iv, (unsigned char*)ciphertext);

    QByteArray ciphArr = QByteArray::fromRawData(ciphertext, ciphertext_len);

    //delete key;
    //delete iv;
    //delete plaintext;
    //delete ciphertext;

    return ciphArr;
}

QString Cryptor::decryptAes(QByteArray ciph, QByteArray kkey, QByteArray ivv)
{
    char *key = new  char[32];
    char* iv = new  char[16];
    strcpy(key, kkey.data());
    strcpy(iv, ivv.data());

    char* decryptedtext = new char[ciph.size()*2];
    int decryptedtext_len;

    decryptedtext_len = decrypt((unsigned char*)ciph.data(), ciph.size(), (unsigned char*)key, (unsigned char*)iv,
                                (unsigned char*)decryptedtext);

    //decryptedtext[decryptedtext_len] = '\0';

    QByteArray decArr = QByteArray::fromRawData(decryptedtext, decryptedtext_len);
    QString decStr = QString::fromStdString(decArr.toStdString());

    //delete [] key ;
    //delete [] iv;

    //delete [] decryptedtext;

    return decStr;

}

QByteArray Cryptor::genAesKey(int kl)
{
        QByteArray k;
        int kll = kl/8;
        char* kk = new char[kll];
        RAND_bytes((unsigned char*)kk, kll);
        k = QByteArray::fromRawData(kk, kll);
        return k;
}

RSA* Cryptor::constructEncrypt(pubKey pk)
{
    RSA* encrpt_rsa = RSA_new();
    encrpt_rsa->n = BN_bin2bn((unsigned char*)pk.n_b, pk.n_size, NULL);
    encrpt_rsa->e = BN_bin2bn((unsigned char*)pk.e_b, pk.b_size, NULL);

    return encrpt_rsa;

}

QByteArray Cryptor::encr(QString inp, RSA *alg)
{
    char* encrypt_string = (char*) calloc(RSA_size(alg), sizeof(char));
    char plain[inp.size()];
    QByteArray arr = QByteArray::fromStdString(inp.toStdString());
    strcpy(plain, arr.data());
    int encrypt_size = RSA_public_encrypt(arr.size()+1, (unsigned char*)plain, (unsigned char*)encrypt_string, alg,
                                          RSA_PKCS1_OAEP_PADDING);

    QByteArray ret = QByteArray::fromRawData(encrypt_string, encrypt_size);
    return ret;

    /**/

}

QByteArray Cryptor::encr(QByteArray inp, RSA *alg)
{
    char* encrypt_string = (char*) calloc(RSA_size(alg), sizeof(char));
    char plain[inp.size()];
    strcpy(plain, inp.data());
    int encrypt_size = RSA_public_encrypt(inp.size()+1, (unsigned char*)plain, (unsigned char*)encrypt_string, alg,
                                          RSA_PKCS1_OAEP_PADDING);

    QByteArray ret = QByteArray::fromRawData(encrypt_string, encrypt_size);
    return ret;

    /**/

}

pubKey Cryptor::genpk(RSA *rsa){

    pubKey pk;

    pk.n_b = (char*)calloc(RSA_size(rsa), sizeof(char));
    pk.e_b = (char*)calloc(RSA_size(rsa), sizeof(char));

    pk.n_size = BN_bn2bin(rsa->n, (unsigned char*)pk.n_b);
    pk.b_size = BN_bn2bin(rsa->e, (unsigned char*)pk.e_b);

    return pk;
}

QString Cryptor::decr(QByteArray cip, RSA *alg)
{
    char* decrypt_string = (char*) calloc(RSA_size(alg), sizeof(char));
    int decrypt_size = RSA_private_decrypt(cip.size(), (unsigned char*)cip.data(), (unsigned char*)decrypt_string,
                                           alg, RSA_PKCS1_OAEP_PADDING);

    QByteArray arr = QByteArray::fromRawData(decrypt_string, decrypt_size);

    return arr;
}

