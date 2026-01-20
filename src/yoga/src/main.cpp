/**
 * Copyright 2022 xiaozhuai
 */

#include "MiniFB_cpp.h"
#include "yoga/YGNode.h"
#include "yoga/YGNodeLayout.h"
#include "yoga/Yoga.h"

struct Rect {
    float left = 0;
    float top = 0;
    float width = 0;
    float height = 0;
};

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

Rect get_rect(YGNodeRef node) {
    return {
        YGNodeLayoutGetLeft(node),
        YGNodeLayoutGetTop(node),
        YGNodeLayoutGetWidth(node),
        YGNodeLayoutGetHeight(node),
    };
}

Rect get_abs_rect(YGNodeRef node) {
    Rect rect = get_rect(node);
    YGNodeRef parent = YGNodeGetParent(node);
    while (parent != nullptr) {
        auto parent_rect = get_rect(parent);
        rect.left += parent_rect.left;
        rect.top += parent_rect.top;
        parent = YGNodeGetParent(parent);
    }
    return rect;
}

void draw_node_bg(YGNodeRef node, uint8_t *pixels, int fb_width, int fb_height, Color color) {
    Rect rect = get_abs_rect(node);
    int left = static_cast<int>(rect.left);
    int top = static_cast<int>(rect.top);
    int right = static_cast<int>(rect.left + rect.width);
    int bottom = static_cast<int>(rect.top + rect.height);
    for (int y = top; y < bottom; ++y) {
        for (int x = left; x < right; ++x) {
            if (x < 0 || x >= fb_width || y < 0 || y >= fb_height) {
                continue;
            }
            int index = (y * fb_width + x) * 4;
#ifdef __APPLE__
            pixels[index + 0] = color.b;
            pixels[index + 1] = color.g;
            pixels[index + 2] = color.r;
            pixels[index + 3] = color.a;
#else
            pixels[index + 0] = color.r;
            pixels[index + 1] = color.g;
            pixels[index + 2] = color.b;
            pixels[index + 3] = color.a;
#endif
        }
    }
}

void draw_layout(uint8_t *pixels, int width, int height, float scale) {
    auto root = YGNodeNew();
    YGNodeStyleSetFlex(root, 1.0f);
    YGNodeStyleSetGap(root, YGGutterRow, 10.0f * scale);

    auto header = YGNodeNew();
    YGNodeStyleSetHeight(header, 60.0f * scale);
    YGNodeInsertChild(root, header, 0);

    auto content0 = YGNodeNew();
    YGNodeStyleSetFlex(content0, 1.0f);
    YGNodeStyleSetMargin(content0, YGEdgeStart, 10.0f * scale);
    YGNodeStyleSetMargin(content0, YGEdgeEnd, 10.0f * scale);
    YGNodeInsertChild(root, content0, 1);

    auto content1 = YGNodeNew();
    YGNodeStyleSetFlex(content1, 2.0f);
    YGNodeStyleSetMargin(content1, YGEdgeStart, 10.0f * scale);
    YGNodeStyleSetMargin(content1, YGEdgeEnd, 10.0f * scale);
    YGNodeInsertChild(root, content1, 2);

    auto footer = YGNodeNew();
    YGNodeStyleSetPositionType(footer, YGPositionTypeAbsolute);
    YGNodeStyleSetPosition(footer, YGEdgeBottom, 0.0f);
    YGNodeStyleSetWidthPercent(footer, 100.0f);
    YGNodeStyleSetHeight(footer, 64.0f * scale);
    YGNodeStyleSetFlexDirection(footer, YGFlexDirectionRow);
    YGNodeStyleSetAlignItems(footer, YGAlignCenter);
    YGNodeStyleSetJustifyContent(footer, YGJustifySpaceAround);
    YGNodeInsertChild(root, footer, 1);

    auto icon0 = YGNodeNew();
    YGNodeStyleSetWidth(icon0, 40.0f * scale);
    YGNodeStyleSetHeight(icon0, 40.0f * scale);
    YGNodeInsertChild(footer, icon0, 0);

    auto icon1 = YGNodeNew();
    YGNodeStyleSetWidth(icon1, 40.0f * scale);
    YGNodeStyleSetHeight(icon1, 40.0f * scale);
    YGNodeInsertChild(footer, icon1, 1);

    auto icon2 = YGNodeNew();
    YGNodeStyleSetWidth(icon2, 40.0f * scale);
    YGNodeStyleSetHeight(icon2, 40.0f * scale);
    YGNodeInsertChild(footer, icon2, 2);

    auto icon3 = YGNodeNew();
    YGNodeStyleSetWidth(icon3, 40.0f * scale);
    YGNodeStyleSetHeight(icon3, 40.0f * scale);
    YGNodeInsertChild(footer, icon3, 3);

    YGNodeCalculateLayout(root, width, height, YGDirectionLTR);

    draw_node_bg(root, pixels, width, height, {200, 200, 200, 255});

    draw_node_bg(header, pixels, width, height, {70, 130, 180, 255});
    draw_node_bg(content0, pixels, width, height, {230, 230, 230, 255});
    draw_node_bg(content1, pixels, width, height, {230, 230, 230, 255});

    draw_node_bg(footer, pixels, width, height, {160, 160, 160, 255});

    draw_node_bg(icon0, pixels, width, height, {80, 80, 80, 255});
    draw_node_bg(icon1, pixels, width, height, {80, 80, 80, 255});
    draw_node_bg(icon2, pixels, width, height, {80, 80, 80, 255});
    draw_node_bg(icon3, pixels, width, height, {80, 80, 80, 255});

    YGNodeFreeRecursive(root);
}

int main() {
    const int windowWidth = 800;
    const int windowHeight = 600;
    auto *window = mfb_open_ex("vcpkg_demo_yoga", windowWidth, windowHeight, 0);

    float scaleX, scaleY;
    mfb_get_monitor_scale(window, &scaleX, &scaleY);

    const int width = static_cast<int>(round(static_cast<float>(windowWidth) * scaleX));
    const int height = static_cast<int>(round(static_cast<float>(windowHeight) * scaleY));

    std::vector<uint8_t> framebuffer(width * height * 4);
    draw_layout(framebuffer.data(), width, height, scaleX);

    do {
        int state = mfb_update_ex(window, framebuffer.data(), width, height);
        if (state < 0) {
            mfb_close(window);
            break;
        }
    } while (mfb_wait_sync(window));

    return 0;
}
