#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QImage>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QPixmap pixmap;
    pixmap.load(QStringLiteral(":/test.jpg"));
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);
    ui->label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

MainWindow::~MainWindow() {
    delete ui;
}

