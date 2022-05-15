#include <vector>
#include <string>
#include <cassert>
#include <MiniFB_cpp.h>
#include <cairo/cairo.h>

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif

#include <cmath>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#undef STB_IMAGE_IMPLEMENTATION

void draw(cairo_t *ctx, int width, int height, cairo_surface_t *image) {
    // clear
    cairo_save(ctx);
    cairo_set_source_rgba(ctx, 0.0, 0.0, 0.0, 0.0);
    cairo_set_operator(ctx, CAIRO_OPERATOR_CLEAR);
    cairo_paint(ctx);
    cairo_restore(ctx);

    // image
    cairo_save(ctx);
    double imageWidth = cairo_image_surface_get_width(image);
    double imageHeight = cairo_image_surface_get_height(image);
    cairo_scale(ctx, double(width) / imageWidth, double(height) / imageHeight);
    cairo_set_source_surface(ctx, image, 0, 0);
    cairo_paint(ctx);
    cairo_restore(ctx);

    // star
    cairo_save(ctx);
    cairo_set_antialias(ctx, CAIRO_ANTIALIAS_BEST);
    cairo_translate(ctx, 0.5f * double(width), 0.5f * double(height));
    const float R = 0.45f * float(width < height ? width : height);
    const float TAU = 6.2831853f;
    const int N = 8;
    cairo_move_to(ctx, R, 0.0f);
    for (int i = 1; i < N; ++i) {
        float theta = 3.0f * float(i) * TAU / float(N);
        cairo_line_to(ctx, R * cos(theta), R * sin(theta));
    }
    cairo_translate(ctx, -0.5f * double(width), -0.5f * double(height));
    cairo_set_source_rgba(ctx, 1.0, 0.0, 0.0, 0.5);
    cairo_fill_preserve(ctx);
    cairo_restore(ctx);
}

cairo_surface_t *createSurface(int width, int height, uint8_t *framebuffer = nullptr) {
    if (framebuffer == nullptr) {
        return cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    }
    return cairo_image_surface_create_for_data(framebuffer, CAIRO_FORMAT_ARGB32, width, height, width * 4);
}

cairo_surface_t *createSurfaceFromImageFile(const std::string &file) {
    int width, height, channels;
    auto data = stbi_load(file.c_str(), &width, &height, &channels, 4);
    // RGBA 2 BGRA
    for (int i = 0; i < width * height; ++i) std::swap(data[i * 4], data[i * 4 + 2]);
    assert(width != 0 && height != 0 && data != nullptr);
    return createSurface(width, height, data);
}

int main() {
    const int windowWidth = 960;
    const int windowHeight = 540;
    auto *window = mfb_open_ex("vcpkg_demo_cairo", windowWidth, windowHeight, 0);

    float scaleX, scaleY;
    mfb_get_monitor_scale(window, &scaleX, &scaleY);

    const int width = (int) floor(float(windowWidth) * scaleX);
    const int height = (int) floor(float(windowHeight) * scaleY);

    auto image = createSurfaceFromImageFile("assets/test.jpg");

    std::vector<uint8_t> framebuffer(width * height * 4);
    auto surface = createSurface(width, height, framebuffer.data());
    auto ctx = cairo_create(surface);

    assert(width == cairo_image_surface_get_width(surface));
    assert(height == cairo_image_surface_get_height(surface));
    assert(width * 4 == cairo_image_surface_get_stride(surface));

    do {
        draw(ctx, width, height, image);
        cairo_show_page(ctx);
        int state = mfb_update_ex(window, cairo_image_surface_get_data(surface), width, height);

        if (state < 0) {
            window = nullptr;
            break;
        }
    } while (mfb_wait_sync(window));

    stbi_image_free(cairo_image_surface_get_data(image));
    cairo_destroy(ctx);
//    free(surface);
//    free(image);

    return 0;
}
