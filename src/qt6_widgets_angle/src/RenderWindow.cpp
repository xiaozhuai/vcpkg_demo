//
// Copyright (c) 2026 xiaozhuai
//

#include "RenderWindow.h"

#include <QEvent>
#include <QPlatformSurfaceEvent>

RenderWindow::RenderWindow(QWindow *parent) : QWindow(parent) {}

RenderWindow::~RenderWindow() { stopRendering(); }

bool RenderWindow::event(QEvent *event) {
    if (event->type() == QEvent::PlatformSurface) {
        const auto *surfaceEvent = static_cast<QPlatformSurfaceEvent *>(event);
        if (surfaceEvent->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed) {
            stopRendering();
        }
    }
    if (event->type() == QEvent::DevicePixelRatioChange) {
        emit devicePixelRatioChanged(devicePixelRatio());
    }
    return QWindow::event(event);
}

void RenderWindow::exposeEvent(QExposeEvent *event) {
    QWindow::exposeEvent(event);
    if (isExposed()) {
        startRendering();
    }
}

void RenderWindow::startRendering() {
    if (rendererStarted_) {
        return;
    }

    renderer_.start(this);
    rendererStarted_ = true;
}

void RenderWindow::stopRendering() {
    if (!rendererStarted_) {
        return;
    }

    renderer_.stop();
    rendererStarted_ = false;
}
