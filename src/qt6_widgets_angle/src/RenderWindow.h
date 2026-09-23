//
// Copyright (c) 2026 xiaozhuai
//

#pragma once

#include <QWindow>

#include "Renderer.h"

class RenderWindow final : public QWindow {
    Q_OBJECT

public:
    explicit RenderWindow(QWindow *parent = nullptr);
    ~RenderWindow() override;

protected:
    bool event(QEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;

signals:
    void devicePixelRatioChanged(qreal ratio);

private:
    void startRendering();
    void stopRendering();

    Renderer renderer_;
    bool rendererStarted_ = false;
};
