#include "AngleHelper.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QOpenGLContext>
#include <QDebug>

int main(int argc, char *argv[]) {
#ifdef _WIN32
    if (isAngleAvailable()) {
        QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    }
#endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
            &engine, &QQmlApplicationEngine::objectCreated,
            &app,
            [](QObject *obj, const QUrl &objUrl) {
                if (!obj) {
                    QCoreApplication::exit(-1);
                }
            },
            Qt::QueuedConnection
    );
    engine.load(url);

    qDebug() << "OpenGL Module Type:" << QOpenGLContext::openGLModuleType();
    qDebug() << "OpenGL Module Type is OpenGL ES:" << (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGLES);

    return app.exec();
}
