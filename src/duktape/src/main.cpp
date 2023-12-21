/**
 * Copyright 2022 xiaozhuai
 */

#include <fstream>
#include <sstream>

#include "duk_bridge.h"
#include "duk_console.h"
#include "duktape.h"

static void duk_error_callback(void *user_data, const char *msg) {
    DUK_LOGE("Fatal: {}", msg);
    abort();
}

static duk_context *duk_create_custom_heap() {
    duk_context *ctx = duk_create_heap(nullptr, nullptr, nullptr, nullptr, duk_error_callback);
    duk_console_init(ctx);
    duk_bridge_init(ctx);
    return ctx;
}

static inline std::string read_script(const std::string &name) {
    std::ifstream file("src/duktape/duktape_compile/lib/" + name);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

static inline void duk_eval_script_file(duk_context *ctx, const std::string &name) {
    std::string script = read_script(name);
    duk_eval_string(ctx, script.c_str());
}

static duk_ret_t native_callback_value(duk_context *ctx) {
    int v = duk_to_int(ctx, 0);
    assert(v == 3);
    return 0;
}

int main() {
    duk_context *ctx = duk_create_custom_heap();
    duk_bridge_register_native_function(ctx, "callbackValue", native_callback_value, 1);
    duk_eval_script_file(ctx, "index.js");
    duk_eval_string(ctx, "bridge.js.pow(2, 3)");
    assert(duk_get_int(ctx, -1) == pow(2, 3));
    duk_destroy_heap(ctx);
    return 0;
}
