/* md5.h - MD5哈希算法实现 */
#ifndef MD5_H
#define MD5_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* MD5常量定义 */
#define MD5_BLOCK_SIZE 64
#define MD5_DIGEST_SIZE 16

/* MD5上下文结构体 */
typedef struct {
    uint32_t state[4];    /* 状态变量 */
    uint32_t count[2];    /* 位计数器 */
    uint8_t buffer[MD5_BLOCK_SIZE]; /* 输入缓冲区 */
} MD5_CTX;

/* MD5函数声明 */
void md5_init(MD5_CTX* ctx);
void md5_update(MD5_CTX* ctx, const uint8_t* input, size_t input_len);
void md5_final(MD5_CTX* ctx, uint8_t digest[MD5_DIGEST_SIZE]);

#endif /* MD5_H */