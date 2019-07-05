#ifndef RSA_H
#define RSA_H
#include <cstdint>

class IRSA {
private:
    uint64_t p, q, d;
public:
    uint64_t n, e;

    void init(uint64_t p, uint64_t q);

    uint64_t encryptByPublic(const uint64_t &);     // 公钥加密
    uint64_t decryptByPrivate(const uint64_t &);  // 私钥解密
    uint64_t encryptByPrivate(const uint64_t &);    // 私钥加密
    uint64_t decryptByPublic(const uint64_t &);    // 公钥解密
};
#endif