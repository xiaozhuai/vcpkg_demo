#include <vector>
#include <string>
#include <cassert>
#include <MiniFB_cpp.h>
#include <cairomm/cairomm.h>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#else
#include <cmath>
#endif

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#undef STB_IMAGE_IMPLEMENTATION

using ContextPtr = Cairo::RefPtr<Cairo::Context>;
using ImageSurfacePtr = Cairo::RefPtr<Cairo::ImageSurface>;

void draw(const ContextPtr& ctx, int width, int height, const ImageSurfacePtr& image) {
    // clear
    ctx->save();
    ctx->set_source_rgba(0.0, 0.0, 0.0, 0.0);
    ctx->set_operator(Cairo::Context::Operator::CLEAR);
    ctx->paint();
    ctx->restore();

    // image
    ctx->save();
    double imageWidth = image->get_width();
    double imageHeight = image->get_height();
    ctx->scale(double(width) / imageWidth, double(height) / imageHeight);
    ctx->set_source(image, 0, 0);
    ctx->paint();
    ctx->restore();

    // star
    ctx->save();
    ctx->set_antialias(Cairo::Antialias::ANTIALIAS_DEFAULT);
    ctx->translate(0.5f * double(width), 0.5f * double(height));
    const float R = 0.45f * std::min(float(width), float(height));
    constexpr int N = 7;
    static_assert(N >= 5 && N % 2 == 1, "N must be odd and >=5");
    constexpr auto step = (N - 1) / 2;
    float theta = -M_PI_2;
    ctx->move_to(R * cos(theta), R * sin(theta));
    for (int i = 1; i < N; ++i) {
        theta += float(step) * M_PI * 2.0f / N;
        ctx->line_to(R * cos(theta), R * sin(theta));
    }
    ctx->translate(-0.5f * double(width), -0.5f * double(height));
    ctx->set_source_rgba(1.0, 0.0, 0.0, 0.5);
    ctx->fill_preserve();
    ctx->restore();
}

ImageSurfacePtr createSurface(int width, int height, uint8_t *framebuffer = nullptr) {
    if (framebuffer == nullptr) {
        return Cairo::ImageSurface::create(Cairo::Surface::Format::ARGB32, width, height);
    }
    return Cairo::ImageSurface::create(framebuffer, Cairo::Surface::Format::ARGB32, width, height, width * 4);
}

ImageSurfacePtr createSurfaceFromImageFile(const std::string &file) {
    int width, height, channels;
    auto data = stbi_load(file.c_str(), &width, &height, &channels, 4);
    assert(width != 0 && height != 0 && data != nullptr);
    // RGBA 2 BGRA
    for (int i = 0; i < width * height; ++i) std::swap(data[i * 4], data[i * 4 + 2]);
    return createSurface(width, height, data);
}

int main() {
    const int windowWidth = 960;
    const int windowHeight = 540;
    auto *window = mfb_open_ex("vcpkg_demo_cairo", windowWidth, windowHeight, 0);

    float scaleX, scaleY;
    mfb_get_monitor_scale(window, &scaleX, &scaleY);

    const int width = (int) round(float(windowWidth) * scaleX);
    const int height = (int) round(float(windowHeight) * scaleY);

    auto image = createSurfaceFromImageFile("assets/test.jpg");

    std::vector<uint8_t> framebuffer(width * height * 4);
    auto surface = createSurface(width, height, framebuffer.data());
    auto ctx = Cairo::Context::create(surface);

    assert(width == surface->get_width());
    assert(height == surface->get_height());
    assert(width * 4 == surface->get_stride());

    do {
        draw(ctx, width, height, image);
        ctx->show_page();
        int state = mfb_update_ex(window, surface->get_data(), width, height);

        if (state < 0) {
            mfb_close(window);
            break;
        }
    } while (mfb_wait_sync(window));

    stbi_image_free(image->get_data());

    return 0;
}
