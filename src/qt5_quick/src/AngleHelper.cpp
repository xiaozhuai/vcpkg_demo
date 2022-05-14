/**
 * Copyright 2022 xiaozhuai
 */

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN

#include "AngleHelper.h"

#include <Windows.h>

#include <QDir>
#include <QFile>
#include <QString>

static QString executablePath() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return QString::fromWCharArray(buffer);
}

static QString executableDirPath() {
    QFileInfo exeFile(executablePath());
    return exeFile.absoluteDir().path();
}

#define LIB_EGL    "libEGL.dll"
#define LIB_GLESv2 "libGLESv2.dll"

bool isAngleAvailable() {
    QDir exeDir(executableDirPath());
    return QFile::exists(exeDir.filePath(LIB_EGL)) && QFile::exists(exeDir.filePath(LIB_GLESv2));
}

#endif
