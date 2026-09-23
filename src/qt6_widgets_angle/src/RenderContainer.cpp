//
// Copyright (c) 2026 xiaozhuai
//

#include "RenderContainer.h"

#include <QVBoxLayout>

#include "RenderWindow.h"

RenderContainer::RenderContainer(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // The window container takes ownership of the rendering window.
    auto *renderContainer = createWindowContainer(new RenderWindow, this);
    renderContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(renderContainer);
}
