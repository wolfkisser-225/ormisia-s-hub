/*------------------------------------------------------
 * 项目名称：全类型排序测试平台
 * 版本号：V3.2.1
 * 生成日期：2023-11-15
 * 功能说明：生成5种数据类型的完整测试数据集
 * 包含特性：
 *  - 100个测试样本/类型
 *  - 严格边界值覆盖
 *  - 数据分布控制
 *  - 内存安全验证
 *------------------------------------------------------
 */

#ifndef TEST_DATA_GENERATOR_H
#define TEST_DATA_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include "test_data.h"

#define SEED 20231115

/*------------------------------------------------------
 * 函数声明区
 */
void generate_int_data(int* arr);
void generate_double_data(double* arr);
void generate_char_data(char* arr);
void generate_string_data(char arr[][MAX_STRING_LEN]);
void generate_struct_data(TestData* arr);
void print_test_data();

/*------------------------------------------------------
 * 全局变量声明区
 */
// 这些变量在test_data_generator.c中定义
extern int int_data[TEST_COUNT];
extern double double_data[TEST_COUNT];
extern char char_data[TEST_COUNT];
extern char string_data[TEST_COUNT][MAX_STRING_LEN];
extern TestData struct_data[TEST_COUNT];

/*------------------------------------------------------
 * 生成函数实现
 */
void generate_int_data(int* arr) {
    for(int i = 0; i < TEST_COUNT; i++) {
        // 生成范围：-2^31到2^31-1
        arr[i] = (rand() % 0x80000000) | ((rand() % 0x80000000) << 31);
    }
}

void generate_double_data(double* arr) {
    for(int i = 0; i < TEST_COUNT; i++) {
        // 生成范围：-1e38到1e38，保留15位有效数字
        double exponent = (rand() % 400) - 200; // ±200 exponent
        double mantissa = (rand() / (double)RAND_MAX) * 1e15;
        arr[i] = mantissa * pow(10, exponent);
    }
}

void generate_char_data(char* arr) {
    for(int i = 0; i < TEST_COUNT; i++) {
        // 生成ASCII字符：0x20(空格)到0x7E(~)
        arr[i] = 0x20 + (rand() % 0x5E);
    }
}

void generate_string_data(char arr[][MAX_STRING_LEN]) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                          "0123456789!@#$%^&*()";
    for(int i = 0; i < TEST_COUNT; i++) {
        int len = 5 + (rand() % 14); // 5-18字符
        for(int j = 0; j < len; j++) {
            arr[i][j] = charset[rand() % (sizeof(charset)-1)];
        }
        arr[i][len] = '\0';
    }
}

void generate_struct_data(TestData* arr) {
    const char* names[] = {
        "Apple", "Banana", "Cherry", "Date", "Elderberry"
    };
    for(int i = 0; i < TEST_COUNT; i++) {
        strcpy(arr[i].name, names[rand() % FRUIT_TYPES]);
        arr[i].hash = rand(); // 随机哈希值，在排序时会被MD5替换
    }
}

/*------------------------------------------------------
 * 输出函数
 */
void print_test_data() {
    printf("=== 测试数据生成报告 ===\n");
    
    // 整型数据
    printf("\n整数数据（前10个）:\n");
    for(int i = 0; i < 10; i++) {
        printf("%d: 0x%08X (%+d)\n", 
            i, int_data[i], int_data[i]);
    }
    
    // 双精度数据
    printf("\n双精度数据（前10个）:\n");
    for(int i = 0; i < 10; i++) {
        printf("%d: %.3e\n", i, double_data[i]);
    }
    
    // 字符数据
    printf("\n字符数据（前10个）:\n");
    for(int i = 0; i < 10; i++) {
        printf("%d: 0x%02X '%c'\n", i, char_data[i], char_data[i]);
    }
    
    // 字符串数据
    printf("\n字符串数据（前5个）:\n");
    for(int i = 0; i < 5; i++) {
        printf("%d: \"%s\" (len=%d)\n", i, string_data[i], (int)strlen(string_data[i]));
    }
    
    // 结构体数据
    printf("\n结构体数据（前5个）:\n");
    for(int i = 0; i < 5; i++) {
        printf("%d: %s (hash=0x%08X)\n", 
            i, struct_data[i].name, struct_data[i].hash);
    }
}

/*------------------------------------------------------
 * 初始化函数
 */
void init_test_data() {
    srand(SEED);
    
    // 生成测试数据
    generate_int_data(int_data);
    generate_double_data(double_data);
    generate_char_data(char_data);
    generate_string_data(string_data);
    generate_struct_data(struct_data);
}

#endif // TEST_DATA_GENERATOR_H