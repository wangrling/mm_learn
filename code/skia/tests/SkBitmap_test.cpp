//
// Created by wangrl on 19-3-15.
//

#include <SkBitmap.h>

int main(int argc, char * argv[]) {

    {
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


