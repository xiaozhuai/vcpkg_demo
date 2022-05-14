/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>

#include "yoga/YGNode.h"
#include "yoga/YGNodeLayout.h"
#include "yoga/Yoga.h"

int main() {
    auto root = YGNodeNew();
    YGNodeStyleSetWidth(root, 500.0f);
    YGNodeStyleSetHeight(root, 300.0f);
    YGNodeStyleSetJustifyContent(root, YGJustifyCenter);
    YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);

    auto child1 = YGNodeNew();
    YGNodeStyleSetWidth(child1, 100.0f);
    YGNodeStyleSetHeight(child1, 100.0f);

    auto child2 = YGNodeNew();
    YGNodeStyleSetWidth(child2, 100.0f);
    YGNodeStyleSetHeight(child2, 100.0f);

    YGNodeInsertChild(root, child1, 0);
    YGNodeInsertChild(root, child2, 1);
    YGNodeCalculateLayout(root, 1000.0f, 1000.0f, YGDirectionLTR);

    printf("root: {left: %.0f, top: %.0f, width: %.0f, height: %.0f}\n", YGNodeLayoutGetLeft(root),
           YGNodeLayoutGetTop(root), YGNodeLayoutGetWidth(root), YGNodeLayoutGetHeight(root));

    printf("child1: {left: %.0f, top: %.0f, width: %.0f, height: %.0f}\n", YGNodeLayoutGetLeft(child1),
           YGNodeLayoutGetTop(child1), YGNodeLayoutGetWidth(child1), YGNodeLayoutGetHeight(child1));

    printf("child2: {left: %.0f, top: %.0f, width: %.0f, height: %.0f}\n", YGNodeLayoutGetLeft(child2),
           YGNodeLayoutGetTop(child2), YGNodeLayoutGetWidth(child2), YGNodeLayoutGetHeight(child2));

    YGNodeFreeRecursive(root);
    return 0;
}
