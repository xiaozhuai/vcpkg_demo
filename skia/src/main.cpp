#include <vector>
#include <cassert>
#include <cmath>
#include <MiniFB_cpp.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkSurface.h>
#include <skia/core/SkPath.h>
#include <skia/core/SkScalar.h>
#include <skia/core/SkImageInfo.h>
#include <skia/core/SkData.h>
#include <skia/core/SkImage.h>

sk_sp<SkImage> loadSkImageFromFile(const std::string &file) {
    sk_sp<SkData> data = SkData::MakeFromFileName(file.c_str());
    return SkImage::MakeFromEncoded(data);
}

void draw(SkCanvas *canvas, int width, int height, const sk_sp<SkImage> &image) {
    canvas->clear(SK_ColorWHITE);

    SkSamplingOptions samplingOptions(SkFilterMode::kNearest, SkMipmapMode::kNone);
    canvas->drawImageRect(
            image,
            SkRect::MakeXYWH(0.0f, 0.0f, float(width), float(height)),
            samplingOptions
    );

    const SkScalar R = 0.45f * float(width < height ? width : height);
    const SkScalar TAU = 6.2831853f;
    const int N = 8;
    SkPath path;
    path.moveTo(R, 0.0f);
    for (int i = 1; i < N; ++i) {
        SkScalar theta = 3.0f * float(i) * TAU / float(N);
        path.lineTo(R * cos(theta), R * sin(theta));
    }
    path.close();

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor4f({1.0f, 0.0f, 0.0f, 0.5f});

    canvas->translate(0.5f * float(width), 0.5f * float(height));
    canvas->drawPath(path, paint);
    canvas->translate(-0.5f * float(width), -0.5f * float(height));
}

sk_sp<SkSurface> createSurface(int width, int height, uint8_t *framebuffer) {
    SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
    size_t rowBytes = info.minRowBytes();
    assert(info.computeMinByteSize() == width * height * 4);
    sk_sp<SkSurface> surface = SkSurface::MakeRasterDirect(info, framebuffer, rowBytes);
    return surface;
}

int main() {
    constexpr int width = 960;
    constexpr int height = 540;

    auto image = loadSkImageFromFile("assets/test.jpg");

    std::vector<uint8_t> framebuffer(width * height * 4);
    auto surface = createSurface(width, height, framebuffer.data());
    auto canvas = surface->getCanvas();

    auto *window = mfb_open_ex("vcpkg_demo_skia", width, height, 0);
    do {
        draw(canvas, width, height, image);
        int state = mfb_update_ex(window, framebuffer.data(), width, height);

        if (state < 0) {
            window = nullptr;
            break;
        }
    } while (mfb_wait_sync(window));

    return 0;
}