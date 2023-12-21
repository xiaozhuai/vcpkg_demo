//
// Copyright (c) 2023 xiaozhuai
//

#pragma once
#ifndef VCPKG_DEMO_DUK_CONSOLE_H
#define VCPKG_DEMO_DUK_CONSOLE_H

#include "duktape.h"
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> duk_get_logger();

void duk_console_init(duk_context *ctx);

#define DUK_LOGD(fmt, ...) duk_get_logger()->debug(fmt, ##__VA_ARGS__)
#define DUK_LOGI(fmt, ...) duk_get_logger()->info(fmt, ##__VA_ARGS__)
#define DUK_LOGW(fmt, ...) duk_get_logger()->warn(fmt, ##__VA_ARGS__)
#define DUK_LOGE(fmt, ...) duk_get_logger()->error(fmt, ##__VA_ARGS__)

#endif  // VCPKG_DEMO_DUK_CONSOLE_H
