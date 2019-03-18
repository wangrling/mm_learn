//
// Created by wangrl on 19-3-15.
//

#include <SkBitmap.h>
#include <SkCanvas.h>
#include <SkFont.h>

int main(int argc, char * argv[]) {

    {
        SkBitmap bitmap;
        for (int rowBytes : { 2, 8 } ) {
            bool result = bitmap.setInfo(SkImageInfo::MakeA8(4, 4), rowBytes);
            SkDebugf("setInfo returned:%s rowBytes:%d\n", result ? "true " : "false", bitmap.rowBytes());
        }
    }

    {
        SkBitmap bitmap;
        for (int w : {0, 8}) {
            for (bool allocate : {false, true}) {
                bitmap.setInfo(SkImageInfo::MakeA8(w, 8));
                allocate ? bitmap.allocPixels() : (void) 0;
                SkDebugf("empty:%s isNull:%s drawsNothing:%s\n", bitmap.empty() ? "true " : "false",
                         bitmap.isNull() ? "true " : "false", bitmap.drawsNothing() ? "true" : "false");
            }
        }
    }

    {
        SkBitmap bitmap;
        SkDebugf("empty bitmap does %shave pixels\n", bitmap.isNull() ? "not " : "");
        bitmap.setInfo(SkImageInfo::MakeA8(8, 8));
        SkDebugf("bitmap with dimensions does %shave pixels\n", bitmap.isNull() ? "not " : "");
        bitmap.allocPixels();
        SkDebugf("allocated bitmap does %shave pixels\n", bitmap.isNull() ? "not " : "");
    }

    {
        // 判断是否为空
        SkBitmap bitmap;
        for (int width : { 0, 2 } ) {
            for (int height : { 0, 2 } ) {
                bitmap.setInfo(SkImageInfo::MakeA8(width, height));
                SkDebugf("width: %d height: %d empty: %s\n", width, height,
                         bitmap.empty() ? "true" : "false");
            }
        }
    }

    {
        const char* colors[] = {"Unknown", "Alpha_8", "RGB_565", "ARGB_4444", "RGBA_8888", "RGB_888x",
                                "BGRA_8888", "RGBA_1010102", "RGB_101010x", "Gray_8", "RGBA_F16"};
        SkImageInfo info = SkImageInfo::MakeA8(1, 1);
        SkBitmap bitmap;
        for (SkColorType colorType : {
                kUnknown_SkColorType, kAlpha_8_SkColorType, kRGB_565_SkColorType,
                kARGB_4444_SkColorType, kRGBA_8888_SkColorType, kRGB_888x_SkColorType,
                kBGRA_8888_SkColorType, kRGBA_1010102_SkColorType, kRGB_101010x_SkColorType,
                kGray_8_SkColorType, kRGBA_F16_SkColorType
        } ) {
            bitmap.setInfo(info.makeColorType(colorType));
            SkDebugf("color: k" "%s" "_SkColorType" "%*s" "shiftPerPixel: %d\n",
                     colors[colorType], 14 - strlen(colors[colorType]), " ",
                     bitmap.shiftPerPixel());
        }
    }

    {
        // rowBytesAsPixels
        // Returns number of pixels that fit on row.
        SkBitmap bitmap;
        for (int rowBytes : { 4, 5, 6, 7, 8} ) {
            bitmap.setInfo(SkImageInfo::MakeN32(1, 1, kPremul_SkAlphaType), rowBytes);
            SkDebugf("rowBytes: %d rowBytesAsPixels: %d\n", rowBytes, bitmap.rowBytesAsPixels());
        }
    }

    {
        const char* colors[] = {"Unknown", "Alpha_8", "RGB_565", "ARGB_4444", "RGBA_8888", "RGB_888x",
                                "BGRA_8888", "RGBA_1010102", "RGB_101010x", "Gray_8", "RGBA_F16"};
        SkImageInfo info = SkImageInfo::MakeA8(1, 1);
        SkBitmap bitmap;
        for (SkColorType colorType : {
                kUnknown_SkColorType, kAlpha_8_SkColorType, kRGB_565_SkColorType,
                kARGB_4444_SkColorType, kRGBA_8888_SkColorType, kRGB_888x_SkColorType,
                kBGRA_8888_SkColorType, kRGBA_1010102_SkColorType, kRGB_101010x_SkColorType,
                kGray_8_SkColorType, kRGBA_F16_SkColorType
        } ) {
            bitmap.setInfo(info.makeColorType(colorType));
            SkDebugf("color: k" "%s" "_SkColorType" "%*s" "bytesPerPixel: %d\n",
                     colors[colorType], 13 - strlen(colors[colorType]), " ",
                     bitmap.bytesPerPixel());
        }
    }

    {
        // 智能指针
        SkBitmap bitmap1, bitmap2;
        bitmap1.setInfo(SkImageInfo::MakeN32(16, 32, kPremul_SkAlphaType,
                                             SkColorSpace::MakeSRGBLinear()));
        bitmap2.setInfo(SkImageInfo::MakeN32(16, 32, kPremul_SkAlphaType,
                bitmap1.refColorSpace()));

        SkColorSpace* colorSpace = bitmap2.colorSpace();
        SkDebugf("gammaCloseToSRGB: %s  gammaIsLinear: %s  isSRGB: %s\n",
                 colorSpace->gammaCloseToSRGB() ? "true" : "false",
                 colorSpace->gammaIsLinear() ? "true" : "false",
                 colorSpace->isSRGB() ? "true" : "false");
    }

    {
        SkBitmap bitmap;
        bitmap.setInfo(SkImageInfo::MakeN32(16, 32, kPremul_SkAlphaType,
                                            SkColorSpace::MakeSRGBLinear()));
        SkColorSpace* colorSpace = bitmap.colorSpace();
        SkDebugf("gammaCloseToSRGB: %s  gammaIsLinear: %s  isSRGB: %s\n",
                 colorSpace->gammaCloseToSRGB() ? "true" : "false",
                 colorSpace->gammaIsLinear() ? "true" : "false",
                 colorSpace->isSRGB() ? "true" : "false");
    }

    {
        const char* alphas[] = {"Unknown", "Opaque", "Premul", "Unpremul"};
        SkPixmap pixmap(SkImageInfo::MakeA8(16, 32), nullptr, 64);
        SkDebugf("alpha type: k" "%s" "_SkAlphaType\n", alphas[pixmap.alphaType()]);
    }

    {
        // colorType
        const char* colors[] = {"Unknown", "Alpha_8", "RGB_565", "ARGB_4444", "RGBA_8888", "RGB_888x",
                                "BGRA_8888", "RGBA_1010102", "RGB_101010x", "Gray_8", "RGBA_F16"};
        SkBitmap bitmap;
        bitmap.setInfo(SkImageInfo::MakeA8(16, 32));
        SkDebugf("color type: k" "%s" "_SkColorType\n", colors[bitmap.colorType()]);
    }

    {
        SkImageInfo info = SkImageInfo::MakeA8(16, 32);
        SkBitmap bitmap;
        bitmap.setInfo(info);
        SkDebugf("bitmap height: %d  info height: %d\n", bitmap.height(), info.height());
    }

    {
        SkImageInfo info = SkImageInfo::MakeA8(16, 32);
        SkBitmap bitmap;
        bitmap.setInfo(info);
        SkDebugf("bitmap width: %d  info width: %d\n", bitmap.width(), info.width());
    }

    {
        // SkImageInfo
        SkBitmap source;
        source.tryAllocPixels(
                SkImageInfo::Make(25, 35, kRGBA_8888_SkColorType, kOpaque_SkAlphaType));
        const SkImageInfo & info = source.info();

        const char* alphas[] = {"Unknown", "Opaque", "Premul", "Unpremul"};
        const char* colors[] = {"Unknown", "Alpha_8", "RGB_565", "ARGB_4444", "RGBA_8888", "RGB_888x",
                                "BGRA_8888", "RGBA_1010102", "RGB_101010x", "Gray_8", "RGBA_F16"};
        SkDebugf("width: %d height: %d color: %s alpha: %s\n", info.width(), info.height(),
                 colors[info.colorType()], alphas[info.alphaType()]);
    }

    {
        // SkPixmap
        SkBitmap bitmap;
        bitmap.allocPixels(SkImageInfo::MakeN32Premul(10, 11));
        SkCanvas offscreen(bitmap);
        offscreen.clear(SK_ColorWHITE);
        SkPaint paint;
        SkFont font;
        offscreen.drawString("&", 0, 10, font, paint);

        const SkPixmap & pixmap = bitmap.pixmap();
        if (pixmap.addr()) {
            SkPMColor  pmWhite = *pixmap.addr32(0, 0);
            for (int y = 0; y < pixmap.height(); ++y) {
                for (int x = 0; x < pixmap.width(); ++x) {
                    SkDebugf("%c", *pixmap.addr32(x, y) == pmWhite ? '-' : 'x');
                }
                SkDebugf("\n");
            }
        }

    }

    {
        // 变量交换
        auto debugster = [](const char* prefix, const SkBitmap& b) -> void {
            const char* alphas[] = {"Unknown", "Opaque", "Premul", "Unpremul"};
            const char* colors[] = {"Unknown", "Alpha_8", "RGB_565", "ARGB_4444", "RGBA_8888", "RGB_888x",
                                    "BGRA_8888", "RGBA_1010102", "RGB_101010x", "Gray_8", "RGBA_F16"};
            SkDebugf("%s width:%d height:%d colorType:k%s_SkColorType alphaType:k%s_SkAlphaType\n",
                     prefix, b.width(), b.height(), colors[b.colorType()], alphas[b.alphaType()]);
        };

        SkBitmap one, two;
        if (!one.tryAllocPixels(
                SkImageInfo::Make(1, 1, kRGBA_8888_SkColorType, kOpaque_SkAlphaType))) {
            return -1;
        }
        if (!two.tryAllocPixels(
                SkImageInfo::Make(2, 2, kBGRA_8888_SkColorType, kPremul_SkAlphaType))) {
            return -1;
        }
        for (int index = 0; index < 2; ++index) {
            debugster("one", one);
            debugster("two", two);
            one.swap(two);
        }
    }

    {
        // 赋值转移
        SkBitmap original;
        if (original.tryAllocPixels(
                SkImageInfo::Make(25, 35, kRGBA_8888_SkColorType, kOpaque_SkAlphaType))) {
            SkDebugf("original has pixels before move: %s\n", original.getPixels() ? "true" : "false");
            SkBitmap copy = std::move(original);
            SkDebugf("original has pixels after move: %s\n", original.getPixels() ? "true" : "false");
            SkDebugf("copy has pixels: %s\n", copy.getPixels() ? "true" : "false");
        }
    }

    {
        // 公用内存
        SkBitmap original;
        if (original.tryAllocPixels(
                SkImageInfo::Make(25, 35, kRGBA_8888_SkColorType, kOpaque_SkAlphaType))) {
            SkDebugf("original has pixels before copy: %s\n", original.getPixels() ? "true" : "false");
            SkBitmap copy = original;
            SkDebugf("original has pixels after copy: %s\n", original.getPixels() ? "true" : "false");
            SkDebugf("copy has pixels: %s\n", copy.getPixels() ? "true" : "false");
        }
    }

    {
        // Copies setting from src to returned SkBitmap.
        // Moves ownership of src pixels to SkBitmap.
        SkBitmap original;
        if (original.tryAllocPixels(
                SkImageInfo::Make(25, 35, kRGBA_8888_SkColorType, kOpaque_SkAlphaType))) {
            SkDebugf("original has pixels before move: %s\n", original.getPixels() ? "true" : "false");
            SkBitmap copy(std::move(original));
            SkDebugf("original has pixels after move: %s\n", original.getPixels() ? "true" : "false");
            SkDebugf("copy has pixels: %s\n", copy.getPixels() ? "true" : "false");
        }
    }

    {
        // 使用引用构造器。
        SkBitmap original;
        if (original.tryAllocPixels(
                SkImageInfo::Make(25, 35, kRGBA_8888_SkColorType, kOpaque_SkAlphaType))) {
            SkDebugf("original has pixels before copy: %s\n", original.getPixels() ? "true" : "false");
            SkBitmap copy(original);
            SkDebugf("original has pixels after copy: %s\n", original.getPixels() ? "true" : "false");
            SkDebugf("copy has pixels: %s\n", copy.getPixels() ? "true" : "false");
        }
    }

    {
        // 默认构造器。
        const char* alphas[] = {"Unknown", "Opaque", "Premul", "Unpremul"};
        const char* colors[] = {"Unknown", "Alpha_8", "RGB_565", "ARGB_4444", "RGBA_8888", "RGB_888x",
                                "BGRA_8888", "RGBA_1010102", "RGB_101010x", "Gray_8", "RGBA_F16"};
        SkBitmap bitmap;
        for (int i = 0; i < 2; ++i) {
            SkDebugf("width: %2d  height: %2d", bitmap.width(), bitmap.height());
            SkDebugf("  color: k%s_SkColorType", colors[bitmap.colorType()]);
            SkDebugf("  alpha: k%s_SkAlphaType\n", alphas[bitmap.alphaType()]);
            bitmap.setInfo(SkImageInfo::Make(25, 35, kRGBA_8888_SkColorType, kOpaque_SkAlphaType),
                           0);
        }
    }

    {
        // HeapAllocator分配内存。
        SkBitmap bitmap;
        bitmap.setInfo(SkImageInfo::MakeN32(16, 16, kPremul_SkAlphaType));
        SkDebugf("pixel address = %p\n", bitmap.getPixels());

        SkBitmap::HeapAllocator stdalloc;
        if (!stdalloc.allocPixelRef(&bitmap)) {
            SkDebugf("pixel allocation failed\n");
        } else {
            SkDebugf("pixel address = %p\n", bitmap.getPixels());
        }
    }

    return 0;
}


