//
// Copyright (c) 2023 xiaozhuai
//

#include "duk_bridge.h"

void duk_bridge_init(duk_context *ctx) {
    duk_push_object(ctx);
    duk_push_object(ctx);
    duk_put_prop_string(ctx, -2, "native");
    duk_push_object(ctx);
    duk_put_prop_string(ctx, -2, "js");
    duk_put_global_string(ctx, "bridge");
}

void duk_bridge_register_native_function(duk_context *ctx, const char *name, duk_c_function func, duk_idx_t nargs) {
    if (!duk_get_global_string(ctx, "bridge")) {
        duk_fatal(ctx, "bridge not initialized");
    }
    duk_get_prop_string(ctx, -1, "native");
    duk_push_c_function(ctx, func, nargs);
    duk_put_prop_string(ctx, -2, name);
    duk_pop_n(ctx, 2);
}
