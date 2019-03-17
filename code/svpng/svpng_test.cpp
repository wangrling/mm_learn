//
// Created by wangrl on 19-3-14.
//
#include "svpng.h"
#include <iostream>

void test_rgb(void) {
    // 储存RGB裸数据，256 * 256个数据。
    unsigned char rgb[256 * 256 * 3], *p = rgb;
    unsigned x, y;
    FILE * fp = fopen("rgb.png", "wb");
    for (y = 0; y < 256; y++) {
        for (x = 0; x < 256; x++) {
            *p++ = (unsigned char)x;    /* R */
            *p++ = (unsigned char)y;    /* G */
            *p++ = 128;                 /* B */
        }
    }
    svpng(fp, 256, 256, rgb, 0);
    fclose(fp);
}

void test_rgba(void) {
    unsigned char rgba[256 * 256 * 4], *p = rgba;
    unsigned x, y;

    FILE * fp = fopen("rgba.png", "wb");

    for (y = 0; y < 256; y++) {
        for (x = 0; x < 256; x++) {
            *p++ = (unsigned char)x;                /* R */
            *p++ = (unsigned char)y;                /* G */
            *p++ = 128;                             /* B */
            // 值越小，透明度越高。
            *p++ = (unsigned char)((x + y) / 2);    /* A */
        }
    }
    svpng(fp, 256, 256, rgba, 1);
    fclose(fp);
}

int main(int argc, char * argv[]) {

    test_rgb();

    test_rgba();

    return 0;
}

