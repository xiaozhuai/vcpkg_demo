//
// Copyright (c) 2023 xiaozhuai
//

#pragma once
#ifndef VCPKG_DEMO_DUK_BRIDGE_H
#define VCPKG_DEMO_DUK_BRIDGE_H

#include "duktape.h"

void duk_bridge_init(duk_context *ctx);

void duk_bridge_register_native_function(duk_context *ctx, const char *name, duk_c_function func, duk_idx_t nargs);

#endif  // VCPKG_DEMO_DUK_BRIDGE_H
