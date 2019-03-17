//
// Created by wangrl on 19-3-14.
//

/**
 * 地址为0x100的地方分别使用大端小端储存0x01234567数据。
 *
 * Big endian
 * 0x100    0x101   0x102   0x103
 * 01       23      45      67
 *
 * Little endian
 * 0x100    0x101   0x102   0x103
 * 67       45      23      01
 *
 * 也就是说小端将低位放在低地址的地方。
 */

#include <stdio.h>

typedef unsigned char * byte_pointer;

void show_bytes(byte_pointer start ,size_t len) {
    for (int i = 0; i < len; i++)
        printf(" %.2x", start[i]);
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(void *));
}

void test_show_bytes(int val) {
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
}

int main(int argc, char * argv[]) {

    // HEX: 3039
    int val = 12345;

    test_show_bytes(val);
    // 39 30 00 00
    // 低位数据存储在内存的低位，表示机器为小端序。


    return 0;
}

