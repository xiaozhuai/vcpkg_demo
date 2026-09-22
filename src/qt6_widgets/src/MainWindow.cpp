/**
 * Copyright 2022 xiaozhuai
 */

#include "MainWindow.h"

#include "ImageWidget.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->imageWidget->setPixmap(QPixmap(QStringLiteral(":/test.jpg")));
}

MainWindow::~MainWindow() { delete ui; }
