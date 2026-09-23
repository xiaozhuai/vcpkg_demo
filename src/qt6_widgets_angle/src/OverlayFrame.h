//
// Copyright (c) 2026 xiaozhuai
//

#pragma once

#include <QFrame>

QT_BEGIN_NAMESPACE
namespace Ui {
class OverlayFrame;
}
QT_END_NAMESPACE

class OverlayFrame final : public QFrame {
    Q_OBJECT

public:
    explicit OverlayFrame(QWidget *parent = nullptr);
    ~OverlayFrame() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::OverlayFrame *ui;
    void scheduleSync();
    bool syncPending_ = false;
};
