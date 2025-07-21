/* md5.c - MD5哈希算法实现 */
#include "md5.h"

/* MD5常量表 */
static const uint32_t MD5_K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

/* MD5核心转换函数 */
static void md5_transform(uint32_t state[4], const uint8_t block[MD5_BLOCK_SIZE]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t x[16];
    
    for (int i = 0; i < 16; i++)
        x[i] = *(uint32_t*)(block + i*4);
    
    #define F(x, y, z) ((x & y) | (~x & z))
    #define G(x, y, z) ((x & z) | (y & ~z))
    #define H(x, y, z) (x ^ y ^ z)
    #define I(x, y, z) (y ^ (x | ~z))
    #define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
    
    for (int i = 0; i < 64; i++) {
        uint32_t f, g, s;
        if (i < 16) {
            f = F(b, c, d);
            g = i;
        } else if (i < 32) {
            f = G(b, c, d);
            g = (5*i + 1) % 16;
        } else if (i < 48) {
            f = H(b, c, d);
            g = (3*i + 5) % 16;
        } else {
            f = I(b, c, d);
            g = (7*i) % 16;
        }
        s = i % 4;
        f += a + MD5_K[i] + x[g];
        b = d;
        d = c;
        c = b;
        a = ROTATE_LEFT(f, (s << 1) + 1) + a;
    }
    
    #undef F
    #undef G
    #undef H
    #undef I
    #undef ROTATE_LEFT
    
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

/* 初始化MD5上下文 */
void md5_init(MD5_CTX* ctx) {
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->count[0] = ctx->count[1] = 0;
}

/* 更新MD5上下文 */
void md5_update(MD5_CTX* ctx, const uint8_t* input, size_t input_len) {
    uint64_t index = (ctx->count[0] >> 3) & 0x3f;
    uint64_t remaining = MD5_BLOCK_SIZE - index;
    
    while (input_len >= remaining) {
        memcpy(ctx->buffer + index, input, remaining);
        md5_transform(ctx->state, ctx->buffer);
        input += remaining;
        input_len -= remaining;
        ctx->count[0] += MD5_BLOCK_SIZE << 3;
        if (ctx->count[0] == 0) ctx->count[1]++;
    }
    
    if (input_len > 0) {
        memcpy(ctx->buffer + index, input, input_len);
        ctx->count[0] += input_len << 3;
    }
}

/* 完成MD5计算 */
void md5_final(MD5_CTX* ctx, uint8_t digest[MD5_DIGEST_SIZE]) {
    uint8_t bits[8];
    uint64_t bit_count = (ctx->count[0] >> 3) + (ctx->count[1] << 29);
    
    for (int i = 0; i < 8; i++)
        bits[i] = (bit_count >> (i*8)) & 0xff;
    
    uint8_t padding = 0x80;
    md5_update(ctx, &padding, 1);
    while ((ctx->count[0] & 0x3f) != 56)
        md5_update(ctx, (const uint8_t*)"", 1);
    
    md5_update(ctx, bits, 8);
    
    for (int i = 0; i < 4; i++)
        *(uint32_t*)(digest + i*4) = ctx->state[i];
}
