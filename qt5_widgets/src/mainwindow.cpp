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
}

MainWindow::~MainWindow() {
    delete ui;
}

