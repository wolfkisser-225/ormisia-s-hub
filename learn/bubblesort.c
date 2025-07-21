/* bubblesort.c - 全类型排序系统（完整版） */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "md5.h" /* 引入MD5模块 */

/* ===================== 类型定义 ===================== */
typedef struct {
    char name[20];
    uint32_t hash;
} TestData;

/* ===================== 排序核心模块 ===================== */
typedef enum {
    SORT_INT,
    SORT_FLOAT,
    SORT_DOUBLE,
    SORT_STRING,
    SORT_STRUCT
} SortType;

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    SortType type;
    uint8_t (*hash)(const void*);
} SortArray;

static void sort_swap(void* a, void* b, size_t size) {
    char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

static int compare_int(const void* a, const void* b) {
    // 修复整数比较，避免大整数溢出问题
    int val_a = *(const int*)a;
    int val_b = *(const int*)b;
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

static int compare_float(const void* a, const void* b) {
    return (*(const float*)a > *(const float*)b) ? 1 : -1;
}

static int compare_double(const void* a, const void* b) {
    return (*(const double*)a > *(const double*)b) ? 1 : -1;
}

static int compare_string(const void* a, const void* b) {
    const char* str1 = *(const char**)a;
    const char* str2 = *(const char**)b;
    return strcmp(str1, str2);
}

static int compare_struct(const void* a, const void* b) {
    const TestData* s1 = (const TestData*)a;
    const TestData* s2 = (const TestData*)b;
    int name_cmp = strcmp(s1->name, s2->name);
    if (name_cmp != 0) return name_cmp;
    return (s1->hash < s2->hash) ? -1 : (s1->hash > s2->hash);
}

static void sort_bubble(void* base, size_t nmemb, size_t size,
                       int (*compar)(const void*, const void*)) {
    for (size_t i = 0; i < nmemb - 1; i++) {
        for (size_t j = 0; j < nmemb - i - 1; j++) {
            if (compar((char*)base + j*size, (char*)base + (j+1)*size) > 0) {
                sort_swap((char*)base + j*size, (char*)base + (j+1)*size, size);
            }
        }
    }
}

/* ===================== 内存管理模块 ===================== */
SortArray* sort_array_create(SortType type) {
    SortArray* arr = malloc(sizeof(SortArray));
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    arr->type = type;
    
    switch(type) {
        case SORT_STRUCT:
            arr->hash = NULL;  // 我们在插入时计算哈希值
            break;
        default:
            arr->hash = NULL;
    }
    return arr;
}

void sort_array_free(SortArray* arr) {
    free(arr->data);
    free(arr);
}

int sort_array_insert(SortArray* arr, const void* element) {
    size_t element_size;
    switch(arr->type) {
        case SORT_INT: element_size = sizeof(int); break;
        case SORT_FLOAT: element_size = sizeof(float); break;
        case SORT_DOUBLE: element_size = sizeof(double); break;
        case SORT_STRING: element_size = sizeof(char*); break;
        case SORT_STRUCT: element_size = sizeof(TestData); break;
        default: return -1;
    }

    if (arr->size >= arr->capacity) {
        size_t new_cap = arr->capacity ? arr->capacity * 2 : 4;
        void* new_data = realloc(arr->data, new_cap * element_size);
        if (!new_data) return -1;
        arr->data = new_data;
        arr->capacity = new_cap;
    }
    memcpy((char*)arr->data + arr->size * element_size, element, element_size);
    arr->size++;
    return 0;
}

#include "test_data.h"

/* 测试数据生成函数声明 */
void init_test_data();
void print_test_data();

/* ===================== 测试模块 ===================== */
static void test_sort() {
    int type;
    printf("请选择排序类型:\n");
    printf("1. 整数排序\n");
    printf("2. 字符串排序\n");
    printf("3. 结构体排序\n");
    printf("4. 全类型测试数据排序\n");
    scanf("%d", &type);
    getchar(); // 消除换行符

    switch(type) {
        case 1: {
            SortArray* arr = sort_array_create(SORT_INT);
            printf("请输入要排序的整数个数: ");
            int n;
            scanf("%d", &n);
            printf("请输入%d个整数:\n", n);
            
            for(int i = 0; i < n; i++) {
                int num;
                scanf("%d", &num);
                sort_array_insert(arr, &num);
            }
            
            sort_bubble(arr->data, arr->size, sizeof(int), compare_int);
            
            printf("整数排序结果: ");
            for(int i = 0; i < arr->size; i++)
                printf("%d ", ((int*)arr->data)[i]);
            printf("\n");
            
            sort_array_free(arr);
            break;
        }
        case 2: {
            SortArray* arr = sort_array_create(SORT_STRING);
            printf("请输入要排序的字符串个数: ");
            int n;
            scanf("%d", &n);
            getchar(); // 消除换行符
            
            char** strings = malloc(n * sizeof(char*));
            printf("请输入%d个字符串(每行一个):\n", n);
            
            for(int i = 0; i < n; i++) {
                char buffer[256];
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0; // 移除换行符
                strings[i] = strdup(buffer);
                sort_array_insert(arr, &strings[i]);
            }
            
            sort_bubble(arr->data, arr->size, sizeof(char*), compare_string);
            
            printf("字符串排序结果: ");
            for(int i = 0; i < arr->size; i++)
                printf("%s ", *(char**)((char*)arr->data + i * sizeof(char*)));
            printf("\n");
            
            for(int i = 0; i < n; i++)
                free(strings[i]);
            free(strings);
            sort_array_free(arr);
            break;
        }
        case 3: {
            SortArray* arr = sort_array_create(SORT_STRUCT);
            printf("请输入要排序的结构体个数: ");
            int n;
            scanf("%d", &n);
            getchar(); // 消除换行符
            
            TestData* data = malloc(n * sizeof(TestData));
            MD5_CTX ctx;
            uint8_t digest[MD5_DIGEST_SIZE];
            
            printf("请输入%d个名字(每行一个，最长19个字符):\n", n);
            for(int i = 0; i < n; i++) {
                fgets(data[i].name, sizeof(data[i].name), stdin);
                data[i].name[strcspn(data[i].name, "\n")] = 0; // 移除换行符
                
                // 计算MD5哈希
                md5_init(&ctx);
                md5_update(&ctx, (const uint8_t*)data[i].name, strlen(data[i].name));
                md5_final(&ctx, digest);
                data[i].hash = *(uint32_t*)digest;
                
                sort_array_insert(arr, &data[i]);
            }
            
            sort_bubble(arr->data, arr->size, sizeof(TestData), compare_struct);
            
            printf("结构体排序结果:\n");
            for(int i = 0; i < arr->size; i++) {
                const TestData* s = &((TestData*)arr->data)[i];
                printf("%s (hash: 0x%08x)\n", s->name, s->hash);
            }
            
            free(data);
            sort_array_free(arr);
            break;
        }
        case 4: {
            clock_t start_time, end_time;
            double cpu_time_used;
            
            // 整数排序测试
            SortArray* arr_int = sort_array_create(SORT_INT);
            for(int i = 0; i < TEST_COUNT; i++)
                sort_array_insert(arr_int, &int_data[i]);
            
            printf("\n=== 整数排序测试 ===\n");
            printf("排序前(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("%d ", int_data[i]);
                if((i+1) % 10 == 0) printf("\n");
            }
            printf("\n");
            
            // 重复排序多次以获得更准确的时间测量
            start_time = clock();
            for(int repeat = 0; repeat < 10; repeat++) {  // 减少重复次数，因为样本数量已增加
                // 每次排序前重置数据
                for(int i = 0; i < TEST_COUNT; i++)
                    ((int*)arr_int->data)[i] = int_data[i];
                sort_bubble(arr_int->data, arr_int->size, sizeof(int), compare_int);
            }
            end_time = clock();
            cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            
            printf("排序后(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("%d ", ((int*)arr_int->data)[i]);
                if((i+1) % 10 == 0) printf("\n");
            }
            printf("\n整数排序用时: %f 秒\n", cpu_time_used);
            
            sort_array_free(arr_int);
            
            // 双精度浮点数排序测试
            SortArray* arr_double = sort_array_create(SORT_DOUBLE);
            for(int i = 0; i < TEST_COUNT; i++)
                sort_array_insert(arr_double, &double_data[i]);
            
            printf("\n=== 双精度浮点数排序测试 ===\n");
            printf("排序前(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("%.3e ", double_data[i]);
                if((i+1) % 5 == 0) printf("\n");
            }
            printf("\n");
            
            // 重复排序多次以获得更准确的时间测量
            start_time = clock();
            for(int repeat = 0; repeat < 10; repeat++) {  // 减少重复次数，因为样本数量已增加
                // 每次排序前重置数据
                for(int i = 0; i < TEST_COUNT; i++)
                    ((double*)arr_double->data)[i] = double_data[i];
                sort_bubble(arr_double->data, arr_double->size, sizeof(double), compare_double);
            }
            end_time = clock();
            cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            
            printf("排序后(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("%.3e ", ((double*)arr_double->data)[i]);
                if((i+1) % 5 == 0) printf("\n");
            }
            printf("\n双精度浮点数排序用时: %f 秒\n", cpu_time_used);
            
            sort_array_free(arr_double);
            
            // 字符排序测试
            SortArray* arr_char = sort_array_create(SORT_INT); // 用INT类型存储char
            for(int i = 0; i < TEST_COUNT; i++) {
                int char_val = (int)char_data[i];
                sort_array_insert(arr_char, &char_val);
            }
            
            printf("\n=== 字符排序测试 ===\n");
            printf("排序前(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("'%c' ", char_data[i]);
                if((i+1) % 10 == 0) printf("\n");
            }
            printf("\n");
            
            // 重复排序多次以获得更准确的时间测量
            start_time = clock();
            for(int repeat = 0; repeat < 10; repeat++) {  // 减少重复次数，因为样本数量已增加
                // 每次排序前重置数据
                for(int i = 0; i < TEST_COUNT; i++) {
                    int char_val = (int)char_data[i];
                    ((int*)arr_char->data)[i] = char_val;
                }
                sort_bubble(arr_char->data, arr_char->size, sizeof(int), compare_int);
            }
            end_time = clock();
            cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            
            printf("排序后(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("'%c' ", (char)((int*)arr_char->data)[i]);
                if((i+1) % 10 == 0) printf("\n");
            }
            printf("\n字符排序用时: %f 秒\n", cpu_time_used);
            
            sort_array_free(arr_char);
            
            // 字符串排序测试
            SortArray* arr_str = sort_array_create(SORT_STRING);
            char** str_ptrs = malloc(TEST_COUNT * sizeof(char*));
            for(int i = 0; i < TEST_COUNT; i++) {
                str_ptrs[i] = strdup(string_data[i]);
                sort_array_insert(arr_str, &str_ptrs[i]);
            }
            
            printf("\n=== 字符串排序测试 ===\n");
            printf("排序前(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("\"%s\" ", string_data[i]);
                if((i+1) % 3 == 0) printf("\n");
            }
            printf("\n");
            
            // 重复排序多次以获得更准确的时间测量
            start_time = clock();
            for(int repeat = 0; repeat < 5; repeat++) { // 字符串操作较慢，且样本数量已增加
                // 每次排序前重置数据
                for(int i = 0; i < TEST_COUNT; i++) {
                    // 释放旧字符串
                    free(*(char**)((char*)arr_str->data + i * sizeof(char*)));
                    // 分配新字符串
                    char* new_str = strdup(string_data[i]);
                    *(char**)((char*)arr_str->data + i * sizeof(char*)) = new_str;
                }
                sort_bubble(arr_str->data, arr_str->size, sizeof(char*), compare_string);
            }
            end_time = clock();
            cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            
            printf("排序后(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("\"%s\" ", *(char**)((char*)arr_str->data + i * sizeof(char*)));
                if((i+1) % 3 == 0) printf("\n");
            }
            printf("\n字符串排序用时: %f 秒\n", cpu_time_used);
            
            for(int i = 0; i < TEST_COUNT; i++)
                free(str_ptrs[i]);
            free(str_ptrs);
            sort_array_free(arr_str);
            
            // 结构体排序测试
            SortArray* arr_struct = sort_array_create(SORT_STRUCT);
            MD5_CTX ctx;
            uint8_t digest[MD5_DIGEST_SIZE];
            TestData* struct_copy = malloc(TEST_COUNT * sizeof(TestData));
            
            printf("\n=== 结构体排序测试 ===\n");
            printf("排序前(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                printf("%s (原始hash: 0x%08X) ", struct_data[i].name, struct_data[i].hash);
                if((i+1) % 3 == 0) printf("\n");
            }
            printf("\n");
            
            // 复制并计算新的MD5哈希
            for(int i = 0; i < TEST_COUNT; i++) {
#ifdef __STDC_LIB_EXT1__
                strncpy_s(struct_copy[i].name, sizeof(struct_copy[i].name), struct_data[i].name, sizeof(struct_copy[i].name) - 1);
#else
                strncpy(struct_copy[i].name, struct_data[i].name, sizeof(struct_copy[i].name) - 1);
                struct_copy[i].name[sizeof(struct_copy[i].name) - 1] = '\0';
#endif
                md5_init(&ctx);
                md5_update(&ctx, (const uint8_t*)struct_copy[i].name, strlen(struct_copy[i].name));
                md5_final(&ctx, digest);
                struct_copy[i].hash = *(uint32_t*)digest;
                sort_array_insert(arr_struct, &struct_copy[i]);
            }
            
            // 重复排序多次以获得更准确的时间测量
            start_time = clock();
            for(int repeat = 0; repeat < 5; repeat++) { // 结构体操作较慢，且样本数量已增加
                // 每次排序前重置数据
                for(int i = 0; i < TEST_COUNT; i++) {
                    TestData* target = &((TestData*)arr_struct->data)[i];
#ifdef __STDC_LIB_EXT1__
                    strncpy_s(target->name, sizeof(target->name), struct_data[i].name, sizeof(target->name) - 1);
#else
                    strncpy(target->name, struct_data[i].name, sizeof(target->name) - 1);
                    target->name[sizeof(target->name) - 1] = '\0';
#endif
                    // 为每个结构体计算唯一的哈希值
                    md5_init(&ctx);
                    // 添加一些随机性，确保每个结构体的哈希值不同
                    char buffer[256];
                    snprintf(buffer, sizeof(buffer), "%s_%d_%d", target->name, i, repeat);
                    md5_update(&ctx, (const uint8_t*)buffer, strlen(buffer));
                    md5_final(&ctx, digest);
                    target->hash = *(uint32_t*)digest;
                }
                sort_bubble(arr_struct->data, arr_struct->size, sizeof(TestData), compare_struct);
            }
            end_time = clock();
            cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
            
            printf("排序后(全部%d个):\n", TEST_COUNT);
            for(int i = 0; i < TEST_COUNT; i++) {
                const TestData* s = &((TestData*)arr_struct->data)[i];
                printf("%s (MD5 hash: 0x%08X) ", s->name, s->hash);
                if((i+1) % 3 == 0) printf("\n");
            }
            printf("\n结构体排序用时: %f 秒\n", cpu_time_used);
            
            free(struct_copy);
            sort_array_free(arr_struct);
            break;
        }
        default:
            printf("无效的选择\n");
    }
}

/* ===================== 主函数 ===================== */
int main() {
    printf("MD5测试结果: ");
    MD5_CTX ctx;
    uint8_t digest[MD5_DIGEST_SIZE];
    
    md5_init(&ctx);
    md5_update(&ctx, (const uint8_t*)"hello world", 11);
    md5_final(&ctx, digest);
    
    for (int i = 0; i < MD5_DIGEST_SIZE; i++)
        printf("%02x", digest[i]);
    printf("\n\n");
    
    // 初始化测试数据
    init_test_data();
    
    // 打印测试数据报告
    print_test_data();
    
    // 执行排序测试
    test_sort();
    
    return 0;
}
