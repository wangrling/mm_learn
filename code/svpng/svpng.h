//
// Created by wangrl on 19-3-14.
//

#pragma once

#include <cstdio>
#include <string>

/**
 * Save a RGB/RGBA image in PNG format.
 *
 * @param fp Output stream (by default using file descriptor).
 * @param w Width of the image. (<16383)
 * @param h Height of the image.
 * @param img Image pixel data in 24-bit RGB or 32-bit RGBA format.
 * @param alpha Whether the image contains alpha channel.
 */
inline void svpng(FILE * fp, unsigned w, unsigned h, const unsigned char * img, int alpha) {
    static const unsigned t[] = { 0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
            /* CRC32 Table */    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };
    unsigned a = 1, b = 0, c, p = w * (alpha ? 4 : 3) + 1, x, y, i;   /* ADLER-a, ADLER-b, CRC, pitch */

    // 然后分成三个部分，IHDR, IDAT, IEND.

    /**
     * A valid PNG datastream shall begin with a PNG signature, immediately followed by an IHDR chunk,
     * then one or more IDAT chunks, and shall end with an IEND chunk.
     */


#define SVPNG_PUT(u) fputc(u, fp)
#define SVPNG_U8A(ua, l) for (i = 0; i < l; i++) SVPNG_PUT((ua)[i]);
#define SVPNG_U32(u) do { SVPNG_PUT((u) >> 24); SVPNG_PUT(((u) >> 16) & 255); SVPNG_PUT(((u) >> 8) & 255); SVPNG_PUT((u) & 255); } while(0)
#define SVPNG_U8C(u) do { SVPNG_PUT(u); c ^= (u); c = (c >> 4) ^ t[c & 15]; c = (c >> 4) ^ t[c & 15]; } while(0)
#define SVPNG_U8AC(ua, l) for (i = 0; i < l; i++) SVPNG_U8C((ua)[i])
#define SVPNG_U16LC(u) do { SVPNG_U8C((u) & 255); SVPNG_U8C(((u) >> 8) & 255); } while(0)
#define SVPNG_U32C(u) do { SVPNG_U8C((u) >> 24); SVPNG_U8C(((u) >> 16) & 255); SVPNG_U8C(((u) >> 8) & 255); SVPNG_U8C((u) & 255); } while(0)
#define SVPNG_U8ADLER(u) do { SVPNG_U8C(u); a = (a + (u)) % 65521; b = (b + a) % 65521; } while(0)
#define SVPNG_BEGIN(s, l) do { SVPNG_U32(l); c = ~0U; SVPNG_U8AC(s, 4); } while(0)
#define SVPNG_END() SVPNG_U32(~c)


    // (1) 将8字节magic数据写入到文件中。
    // SVPNG_U8A("\x89PNG\r\n\32\n", 8);           /* Magic */
    // 常规文件的PNG头
    // HEX: 8950 4e47 0d0a 1a0a
    // 和这里的倒数第二位不同。

    const char * magic_header = "\x89PNG\r\n\32\n";
    for (i = 0;i < 8; i++) {
        fputc(magic_header[i], fp);
    }


    /**
     * The IHDR chunk shall be the first chunk in the PNG datastream. It contains:
     * Width                        4 bytes
     * Height                       4 bytes
     * Bit depth                    1 byte
     * Colour type                  1 byte
     * Compression method           1 byte
     * Filter method                1 byte
     * Interlace method             1 byte
     */
    SVPNG_BEGIN("IHDR", 13);                    /* IHDR chunk { */
    SVPNG_U32C(w); SVPNG_U32C(h);               /*   Width & Height (8 bytes) */
    SVPNG_U8C(8); SVPNG_U8C(alpha ? 6 : 2);     /*   Depth=8, Color=True color with/without alpha (2 bytes) */
    SVPNG_U8AC("\0\0\0", 3);                    /*   Compression=Deflate, Filter=No, Interlace=No (3 bytes) */
    SVPNG_END();                                /* } */

    /**
     * The IDAT chunk contains the actual image data which is the output stream of the compression algorithm.
     */
    SVPNG_BEGIN("IDAT", 2 + h * (5 + p) + 4);   /* IDAT chunk { */
    SVPNG_U8AC("\x78\1", 2);                    /*   Deflate block begin (2 bytes) */
    for (y = 0; y < h; y++) {                   /*   Each horizontal line makes a block for simplicity */
        SVPNG_U8C(y == h - 1);                  /*   1 for the last block, 0 for others (1 byte) */
        SVPNG_U16LC(p); SVPNG_U16LC(~p);        /*   Size of block in little endian and its 1's complement (4 bytes) */
        SVPNG_U8ADLER(0);                       /*   No filter prefix (1 byte) */
        for (x = 0; x < p - 1; x++, img++)
            SVPNG_U8ADLER(*img);                /*   Image pixel data */
    }
    SVPNG_U32C((b << 16) | a);                  /*   Deflate block end with adler (4 bytes) */
    SVPNG_END();                                /* } */


    /* IEND chunk {} */
    SVPNG_BEGIN("IEND", 0); SVPNG_END();
}