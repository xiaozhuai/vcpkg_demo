/**
 * Copyright 2022 xiaozhuai
 */

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <cassert>
#include <cmath>
#include <vector>

#include "MiniFB_cpp.h"
#include "skia/core/SkCanvas.h"
#include "skia/core/SkData.h"
#include "skia/core/SkImage.h"
#include "skia/core/SkImageInfo.h"
#include "skia/core/SkPath.h"
#include "skia/core/SkScalar.h"
#include "skia/core/SkSurface.h"

void draw(SkCanvas *canvas, int width, int height, const sk_sp<SkImage> &image) {
    // clear
    canvas->clear(SK_ColorWHITE);

    // image
    SkSamplingOptions samplingOptions(SkFilterMode::kNearest, SkMipmapMode::kNone);
    canvas->drawImageRect(image, SkRect::MakeXYWH(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
                          samplingOptions);

    // star
    const SkScalar R = 0.45f * std::min(static_cast<float>(width), static_cast<float>(height));
    constexpr int N = 7;
    static_assert(N >= 5 && N % 2 == 1, "N must be odd and >=5");
    constexpr auto step = (N - 1) / 2;
    SkPath path;
    SkScalar theta = -M_PI_2;
    path.moveTo(R * cos(theta), R * sin(theta));
    for (int i = 1; i < N; ++i) {
        theta += static_cast<float>(step) * M_PI * 2.0f / N;
        path.lineTo(R * cos(theta), R * sin(theta));
    }
    path.close();
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor4f({1.0f, 0.0f, 0.0f, 0.5f});
    canvas->save();
    canvas->translate(0.5f * static_cast<float>(width), 0.5f * static_cast<float>(height));
    canvas->drawPath(path, paint);
    canvas->restore();
}

inline sk_sp<SkImage> loadSkImageFromFile(const std::string &file) {
    auto data = SkData::MakeFromFileName(file.c_str());
    assert(data);
    return SkImages::DeferredFromEncodedData(data);
}

inline sk_sp<SkSurface> createSurface(int width, int height, uint8_t *framebuffer) {
    SkImageInfo info = SkImageInfo::MakeN32(width, height, SkAlphaType::kUnpremul_SkAlphaType);
    sk_sp<SkSurface> surface = SkSurfaces::WrapPixels(info, framebuffer, width * 4);
    return surface;
}

int main() {
    const int windowWidth = 960;
    const int windowHeight = 540;
    auto *window = mfb_open_ex("vcpkg_demo_skia", windowWidth, windowHeight, 0);

    float scaleX, scaleY;
    mfb_get_monitor_scale(window, &scaleX, &scaleY);

    const int width = static_cast<int>(round(static_cast<float>(windowWidth) * scaleX));
    const int height = static_cast<int>(round(static_cast<float>(windowHeight) * scaleY));

    auto image = loadSkImageFromFile("assets/test.jpg");

    std::vector<uint8_t> framebuffer(width * height * 4);
    auto surface = createSurface(width, height, framebuffer.data());
    auto canvas = surface->getCanvas();

    do {
        draw(canvas, width, height, image);
#ifdef __APPLE__
        for (int i = 0; i < width * height; ++i) {
            std::swap(framebuffer[i * 4], framebuffer[i * 4 + 2]);
        }
#endif
        int state = mfb_update_ex(window, framebuffer.data(), width, height);

        if (state < 0) {
            mfb_close(window);
            break;
        }
    } while (mfb_wait_sync(window));

    return 0;
}
