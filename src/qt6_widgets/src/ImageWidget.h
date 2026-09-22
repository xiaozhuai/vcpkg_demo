/**
 * Copyright 2022 xiaozhuai
 */

#pragma once

#include <QPixmap>
#include <QWidget>

class ImageWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(
        Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode NOTIFY aspectRatioModeChanged)

public:
    explicit ImageWidget(QWidget *parent = nullptr);

    void setPixmap(const QPixmap &pixmap);
    const QPixmap &pixmap() const;
    void clear();

    void setAspectRatioMode(Qt::AspectRatioMode mode);
    Qt::AspectRatioMode aspectRatioMode() const;

    QSize sizeHint() const override;

signals:
    void aspectRatioModeChanged(Qt::AspectRatioMode mode);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap pixmap_;
    Qt::AspectRatioMode aspectRatioMode_ = Qt::IgnoreAspectRatio;
};
