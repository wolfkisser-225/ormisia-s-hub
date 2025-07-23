#include <stdio.h>
#include <string.h>
#include "md5.h"

int main() {
    MD5_CTX ctx;
    uint8_t digest[MD5_DIGEST_SIZE];
    char* test_string = "hello world";
    size_t len = strlen(test_string);
    
    // 初始化MD5上下文
    md5_init(&ctx);
    
    // 更新数据
    md5_update(&ctx, (uint8_t*)test_string, len);
    
    // 完成计算
    md5_final(&ctx, digest);
    
    // 打印结果
    printf("MD5(\"%s\") = ", test_string);
    for (int i = 0; i < MD5_DIGEST_SIZE; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");
    
    // 验证结果
    printf("Expected MD5: 5eb63bbbe01eeed093cb22bb8f5acdc3\n");
    
    return 0;
}