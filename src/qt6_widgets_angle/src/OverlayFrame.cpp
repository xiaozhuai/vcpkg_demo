//
// Copyright (c) 2026 xiaozhuai
//

#include "OverlayFrame.h"

#include <QEvent>
#include <QStyle>
#include <QWindow>

#include "ui_OverlayFrame.h"

namespace {

Qt::WindowFlags overlayWindowFlags() {
#if defined(Q_OS_WIN)
    // A native child window does not retain QWidget's per-pixel alpha path on
    // Windows. Keep the overlay as a non-activating owned tool window instead.
    return Qt::Tool | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus;
#else
    return Qt::Window | Qt::FramelessWindowHint;
#endif
}

}  // namespace

OverlayFrame::OverlayFrame(QWidget *parent) : QFrame(parent, overlayWindowFlags()), ui(new Ui::OverlayFrame) {
    // Keep a separate backing store with alpha, above the native rendering surface.
    setAttribute(Qt::WA_TranslucentBackground);
#if defined(Q_OS_WIN)
    setAttribute(Qt::WA_ShowWithoutActivating);
#endif
    ui->setupUi(this);
    setAttribute(Qt::WA_NativeWindow);

    ui->previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    if (parent) {
        parent->installEventFilter(this);
        if (parent->window() != parent) {
            parent->window()->installEventFilter(this);
        }
    }
    scheduleSync();
}

OverlayFrame::~OverlayFrame() { delete ui; }

bool OverlayFrame::eventFilter(QObject *watched, QEvent *event) {
    switch (event->type()) {
        case QEvent::Show:
        case QEvent::Hide:
        case QEvent::Resize:
        case QEvent::Move:
        case QEvent::WindowStateChange:
        case QEvent::WinIdChange:
            scheduleSync();
            break;
        default:
            break;
    }
    return QFrame::eventFilter(watched, event);
}

void OverlayFrame::scheduleSync() {
    if (syncPending_) {
        return;
    }
    syncPending_ = true;
    // Let the owner's native hierarchy and layout settle before attaching the overlay.
    QMetaObject::invokeMethod(this, [this] {
        syncPending_ = false;
        auto *owner = parentWidget();
        if (!owner || !owner->isVisible() || owner->window()->isMinimized()) {
            hide();
            return;
        }
        auto *host = owner->window();
        if (!host->windowHandle()) {
            return;
        }
        // Match the rendering area's width and anchor the overlay's bottom-left
        // corner to its owner. The fixed height is configured in OverlayFrame.ui.
        const QSize overlaySize(owner->width(), height());
        const QPoint anchorOffset(0, owner->height() - overlaySize.height());
#if defined(Q_OS_WIN)
        // Reparenting a translucent QWidget's native HWND turns it into a child
        // window and bypasses Qt's layered-window composition. A transient owner
        // preserves alpha while keeping the overlay above and tied to the host.
        if (windowHandle()->transientParent() != host->windowHandle()) {
            windowHandle()->setTransientParent(host->windowHandle());
        }
        const QPoint topLeft = owner->mapToGlobal(anchorOffset);
#else
        if (windowHandle()->parent() != host->windowHandle()) {
            windowHandle()->setParent(host->windowHandle());
        }
        // Native coordinates are relative to the host window, not the QWidget parent.
        const QPoint topLeft = owner->mapTo(host, anchorOffset);
#endif
        setGeometry(QRect(topLeft, overlaySize));
        show();
        windowHandle()->show();
        windowHandle()->raise();
    }, Qt::QueuedConnection);
}
