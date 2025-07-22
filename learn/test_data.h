/*------------------------------------------------------
 * 项目名称：全类型排序测试平台
 * 版本号：V3.2.1
 * 生成日期：2023-11-15
 * 功能说明：测试数据常量定义
 *------------------------------------------------------
 */

#ifndef TEST_DATA_H
#define TEST_DATA_H

// 测试数据常量定义
#define TEST_COUNT 100000
#define MAX_STRING_LEN 20
#define FRUIT_TYPES 5

// 测试数据声明（在test_data_generator.h中定义）
extern int int_data[TEST_COUNT];
extern double double_data[TEST_COUNT];
extern char char_data[TEST_COUNT];
extern char string_data[TEST_COUNT][MAX_STRING_LEN];
extern TestData struct_data[TEST_COUNT];

#endif // TEST_DATA_H