/**
 * Copyright 2022 xiaozhuai
 */

#include "mainwindow.h"

#include <QImage>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QPixmap pixmap;
    pixmap.load(QStringLiteral(":/test.jpg"));
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

MainWindow::~MainWindow() { delete ui; }
