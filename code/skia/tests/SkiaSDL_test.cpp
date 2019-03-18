//
// Created by wangrl on 19-3-18.
//

#include <cstdint>
#include <SkTypes.h>
#include <GL/gl.h>
#include <SkRect.h>
#include <SkTArray.h>
#include <GrContext.h>
#include <SkSurface.h>
#include <SkFont.h>
#include <SkRandom.h>
#include "SDL2/SDL.h"
#include "gl/GrGLInterface.h"
#include "GrBackendSurface.h"
#include "GrContext.h"
#include "gl/GrGLDefines.h"
#include "gl/GrGLUtil.h"

struct ApplicationState {
    ApplicationState() : fQuit(false) {

    }

    // Storage for the user created rectangles. The last one may still be being edited.
    SkTArray<SkRect> fRects;
    bool fQuit;
};

static void handle_error() {
    const char *error = SDL_GetError();
    SkDebugf("SDL Error: %s\n", error);
    SDL_ClearError();
}

// Creates a star type shape using a SkPath
static SkPath create_star() {
    static const int kNumPoints = 5;
    SkPath concavePath;
    SkPoint points[kNumPoints] = {{0, SkIntToScalar(-50)}};
    SkMatrix rot;
    rot.setRotate(SkIntToScalar(360) / kNumPoints);
    for (int i = 1; i < kNumPoints; ++i) {
        rot.mapPoints(points + i, points + i - 1, 1);
    }
    concavePath.moveTo(points[0]);
    for (int i = 0; i < kNumPoints; ++i) {
        concavePath.lineTo(points[(2 * i) % kNumPoints]);
    }
    concavePath.setFillType(SkPath::kEvenOdd_FillType);
    SkASSERT(!concavePath.isConvex());
    concavePath.close();
    return concavePath;
}

static void handle_events(ApplicationState *state, SkCanvas *canvas) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEMOTION:
                if (event.motion.state == SDL_PRESSED) {
                    SkRect &rect = state->fRects.back();
                    rect.fRight = event.motion.x;
                    rect.fBottom = event.motion.y;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.state == SDL_PRESSED) {
                    state->fRects.push_back() = SkRect::MakeLTRB(SkIntToScalar(event.button.x),
                                                                 SkIntToScalar(event.button.y),
                                                                 SkIntToScalar(event.button.x),
                                                                 SkIntToScalar(event.button.y));
                }
                break;
            case SDL_KEYDOWN: {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_ESCAPE) {
                    state->fQuit = true;
                }
                break;
            }
            case SDL_QUIT:
                state->fQuit = true;
                break;
            default:
                break;
        }
    }
}

int main(int argc, char **argv) {

    uint32_t windowFlags = 0;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GLContext glContext = nullptr;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

    static const int kStencilBits = 8;  // Skia needs 8 stencil bits
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, kStencilBits);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    static const int kMsaaSampleCount = 0;

    /**
     * In a real application you might want to initialize more subsystems.
     */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        handle_error();
        return 1;
    }

    /**
     * Setup window
     * This code will create a window with the same resolution as the user's desktop.
     */
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        handle_error();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, dm.w, dm.h, windowFlags);

    if (!window) {
        handle_error();
        return 1;
    }

    /**
     * Try and setup a GL context.
     */
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        handle_error();
        return 1;
    }

    int success = SDL_GL_MakeCurrent(window, glContext);
    if (success != 0) {
        handle_error();
        return success;
    }

    uint32_t windowFormat = SDL_GetWindowPixelFormat(window);
    int contextType;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &contextType);

    int dw, dh;
    SDL_GL_GetDrawableSize(window, &dw, &dh);

    glViewport(0, 0, dw, dh);
    glClearColor(1, 1, 1, 1);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // setup GrContext
    auto interface = GrGLMakeNativeInterface();

    // setup contexts.
    sk_sp<GrContext> grContext(GrContext::MakeGL(interface));
    SkASSERT(grContext);

    /**
     * Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
     * render to it.
     */
    GrGLint buffer;
    GR_GL_GetIntegerv(interface.get(), GR_GL_FRAMEBUFFER_BINDING, &buffer);

    GrGLFramebufferInfo info;
    info.fFBOID = (GrGLuint) buffer;
    SkColorType colorType;

    // SkDebugf("%s", SDL_GetPixelFormatName(windowFormat));
    // TODO: the windowFormat is never any of these?
    if (SDL_PIXELFORMAT_RGBA8888 == windowFormat) {
        info.fFormat = GR_GL_RGBA8;
        colorType = kRGBA_8888_SkColorType;
    } else {
        colorType = kBGRA_8888_SkColorType;
        if (SDL_GL_CONTEXT_PROFILE_ES == contextType) {
            info.fFormat = GR_GL_BGRA8;
        } else {
            // We assume the internal format is RGBA8 on desktop GL
            info.fFormat = GR_GL_RGBA8;
        }
    }

    GrBackendRenderTarget target(dw, dh, kMsaaSampleCount, kStencilBits, info);

    // setup SkSurface
    // To use distance field text, use commented out SkSurfaceProps instead
    // SkSurfaceProps props(SkSurfaceProps::kUseDeviceIndependentFonts_Flag,
    //                      SkSurfaceProps::kLegacyFontHost_InitType);
    SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);

    sk_sp<SkSurface> surface(SkSurface::MakeFromBackendRenderTarget(grContext.get(), target,
                                                                    kBottomLeft_GrSurfaceOrigin,
                                                                    colorType, nullptr, &props));

    SkCanvas *canvas = surface->getCanvas();
    canvas->scale((float) dw / dm.w, (float) dh / dm.h);

    ApplicationState state;

    const char *helpMessage = "Click and drag to create rects.  Press esc to quit.";

    SkPaint paint;

    // create a surface for CPU rasterization
    sk_sp<SkSurface> cpuSurface(SkSurface::MakeRaster(canvas->imageInfo()));

    SkCanvas *offscreen = cpuSurface->getCanvas();
    offscreen->save();
    offscreen->translate(50.0f, 50.0f);
    offscreen->drawPath(create_star(), paint);
    offscreen->restore();

    sk_sp<SkImage> image = cpuSurface->makeImageSnapshot();

    int rotation = 0;
    SkFont font;

    while (!state.fQuit) {
        SkRandom rand;
        canvas->clear(SK_ColorWHITE);
        handle_events(&state, canvas);

        paint.setColor(SK_ColorBLACK);
        canvas->drawString(helpMessage, 100.0f, 100.0f, font, paint);
        for (int i = 0; i < state.fRects.count(); i++) {
            paint.setColor(rand.nextU() | 0x44808080);
            canvas->drawRect(state.fRects[i], paint);
        }

        // draw offscreen canvas
        canvas->save();
        canvas->translate(dm.w / 2.0, dm.h / 2.0);
        canvas->rotate(rotation++);
        canvas->drawImage(image, -50.0f, -50.0f);
        canvas->restore();

        canvas->flush();
        SDL_GL_SwapWindow(window);
    }

    if (glContext) {
        SDL_GL_DeleteContext(glContext);
    }

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

