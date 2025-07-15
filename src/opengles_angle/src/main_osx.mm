#if defined(__APPLE__)

#define GLFW_EXPOSE_NATIVE_COCOA

#import <QuartzCore/CAMetalLayer.h>

#include "EGL/egl.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

@interface ScalingMetalLayer : CAMetalLayer
@end

@implementation ScalingMetalLayer

- (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary<NSKeyValueChangeKey, id> *)change
                       context:(void *)context {
    if ([keyPath isEqualToString:@"backingScaleFactor"]) {
        NSWindow *window = (NSWindow *)object;
        self.contentsScale = window.backingScaleFactor;
    }
}

@end

EGLSurface createEGLSurface(EGLDisplay display, EGLConfig config, GLFWwindow *window) {
    @autoreleasepool {
        NSWindow *ns_window = glfwGetCocoaWindow(window);
        NSView *view = [ns_window contentView];
        [view setWantsLayer:YES];
        CAMetalLayer *layer = [ScalingMetalLayer layer];
        [view setLayer:layer];
        [layer setContentsScale:[ns_window backingScaleFactor]];
        [ns_window addObserver:layer forKeyPath:@"backingScaleFactor" options:NSKeyValueObservingOptionNew context:nil];
        return eglCreateWindowSurface(display, config, layer, nullptr);
    }  // autoreleasepool
}

#endif
