//
// Created by xiaozhuai on 2022/5/14.
//

#include "AngleHelper.h"
#include <QString>
#include <QFile>
#include <QDir>

#ifdef _WIN32

#define LIB_EGL "libEGL.dll"
#define LIB_GLESv2 "libGLESv2.dll"

bool isAngleAvailable() {
    QDir exeDir(QCoreApplication::applicationDirPath());
    return QFile::exists(exeDir.filePath(LIB_EGL))
           && QFile::exists(exeDir.filePath(LIB_GLESv2));
}

#endif