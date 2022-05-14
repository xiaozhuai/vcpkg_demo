//
// Created by xiaozhuai on 2022/5/14.
//

#include "AngleHelper.h"
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

static QString executablePath() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return QString::fromWCharArray(buffer);
}

#endif


static QString executableDirPath() {
    QFileInfo exeFile(executablePath());
    return exeFile.absoluteDir().path();
}


#ifdef _WIN32

#ifdef QT_DEBUG
#define LIB_EGL "libEGLd.dll"
#define LIB_GLESv2 "libGLESv2d.dll"
#else
#define LIB_EGL "libEGL.dll"
#define LIB_GLESv2 "libGLESv2.dll"
#endif

bool isAngleAvailable() {
    QDir exeDir(executableDirPath());
    return QFile::exists(exeDir.filePath(LIB_EGL))
           && QFile::exists(exeDir.filePath(LIB_GLESv2));
}

#endif