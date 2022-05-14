/**
 * Copyright 2022 xiaozhuai
 */

#include <QDebug>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QQmlApplicationEngine>

#if defined(_WIN32)
#include "AngleHelper.h"
#endif

int main(int argc, char *argv[]) {
#if defined(_WIN32)
    if (isAngleAvailable()) {
        QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    }

    // Force win7 use d3d9 as angle backend
    if (QSysInfo::productType() == "windows" && QSysInfo::productVersion() == "7") {
        qputenv("QT_ANGLE_PLATFORM", "d3d9");
    }
#endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [](QObject *obj, const QUrl &objUrl) {
            if (!obj) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    qDebug() << "OpenGL Module Type:" << QOpenGLContext::openGLModuleType();
    qDebug() << "OpenGL Module Type is OpenGL ES:" << (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGLES);

    return app.exec();
}
