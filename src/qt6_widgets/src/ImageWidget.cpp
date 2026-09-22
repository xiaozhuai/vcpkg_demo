/**
 * Copyright 2022 xiaozhuai
 */

#include "ImageWidget.h"

#include <QPainter>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void ImageWidget::setPixmap(const QPixmap &pixmap) {
    pixmap_ = pixmap;
    updateGeometry();
    update();
}

const QPixmap &ImageWidget::pixmap() const { return pixmap_; }

void ImageWidget::setAspectRatioMode(Qt::AspectRatioMode mode) {
    if (aspectRatioMode_ == mode) {
        return;
    }

    aspectRatioMode_ = mode;
    update();
    emit aspectRatioModeChanged(mode);
}

Qt::AspectRatioMode ImageWidget::aspectRatioMode() const { return aspectRatioMode_; }

void ImageWidget::clear() {
    pixmap_ = QPixmap();
    updateGeometry();
    update();
}

QSize ImageWidget::sizeHint() const { return pixmap_.isNull() ? QWidget::sizeHint() : pixmap_.size(); }

void ImageWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    if (pixmap_.isNull() || size().isEmpty()) {
        return;
    }

    const QRectF widgetRect(rect());
    const QRectF pixmapRect(0, 0, pixmap_.width(), pixmap_.height());
    QRectF targetRect = widgetRect;
    QRectF sourceRect = pixmapRect;

    const qreal widgetRatio = widgetRect.width() / widgetRect.height();
    const qreal pixmapRatio = pixmapRect.width() / pixmapRect.height();

    if (aspectRatioMode_ == Qt::KeepAspectRatio) {
        const qreal scale = pixmapRatio > widgetRatio ? widgetRect.width() / pixmapRect.width()
                                                      : widgetRect.height() / pixmapRect.height();
        targetRect.setSize(pixmapRect.size() * scale);
        targetRect.moveCenter(widgetRect.center());
    } else if (aspectRatioMode_ == Qt::KeepAspectRatioByExpanding) {
        if (pixmapRatio > widgetRatio) {
            const qreal sourceWidth = pixmapRect.height() * widgetRatio;
            sourceRect.setLeft((pixmapRect.width() - sourceWidth) / 2.0);
            sourceRect.setWidth(sourceWidth);
        } else {
            const qreal sourceHeight = pixmapRect.width() / widgetRatio;
            sourceRect.setTop((pixmapRect.height() - sourceHeight) / 2.0);
            sourceRect.setHeight(sourceHeight);
        }
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(targetRect, pixmap_, sourceRect);
}
