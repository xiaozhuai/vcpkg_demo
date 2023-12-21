//
// Copyright (c) 2023 xiaozhuai
//

#include "duk_console.h"

#include "spdlog/sinks/stdout_color_sinks.h"

#define DUK_LOGGER_NAME "duktape"

std::shared_ptr<spdlog::logger> duk_get_logger() {
    auto logger = spdlog::get(DUK_LOGGER_NAME);
    if (!logger) {
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        logger = std::make_shared<spdlog::logger>(DUK_LOGGER_NAME, sink);
        logger->set_pattern("%^[%H:%M:%S] [%n] [Thread:%t] [%L] %v%$");
        logger->set_level(spdlog::level::debug);
        spdlog::register_logger(logger);
    }
    return logger;
}

enum LogLevel {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
};

static duk_ret_t duk_console_log_helper(duk_context *ctx, const char *error_name) {
    auto level = (duk_uint_t)duk_get_current_magic(ctx);
    duk_idx_t n = duk_get_top(ctx);
    duk_idx_t i;

    duk_get_global_string(ctx, "console");
    duk_get_prop_string(ctx, -1, "format");

    for (i = 0; i < n; i++) {
        if (duk_check_type_mask(ctx, i, DUK_TYPE_MASK_OBJECT)) {
            /* Slow path formatting. */
            duk_dup(ctx, -1); /* console.format */
            duk_dup(ctx, i);
            duk_call(ctx, 1);
            duk_replace(ctx, i); /* arg[i] = console.format(arg[i]); */
        }
    }

    duk_pop_2(ctx);

    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, n);

    if (error_name) {
        duk_push_error_object(ctx, DUK_ERR_ERROR, "%s", duk_require_string(ctx, -1));
        duk_push_string(ctx, "name");
        duk_push_string(ctx, error_name);
        duk_def_prop(ctx, -3, DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE); /* to get e.g. 'Trace: 1 2 3' */
        duk_get_prop_string(ctx, -1, "stack");
    }

    const char *log = duk_to_string(ctx, -1);
    switch (level) {
        case LOG_LEVEL_DEBUG:
            DUK_LOGD("{}", log);
            break;
        case LOG_LEVEL_INFO:
            DUK_LOGI("{}", log);
            break;
        case LOG_LEVEL_WARN:
            DUK_LOGW("{}", log);
            break;
        case LOG_LEVEL_ERROR:
            DUK_LOGE("{}", log);
            break;
        default:
            break;
    }
    return 0;
}

static duk_ret_t duk_console_log(duk_context *ctx) { return duk_console_log_helper(ctx, nullptr); }

static duk_ret_t duk_console_trace(duk_context *ctx) { return duk_console_log_helper(ctx, "Trace"); }

/* For now, just share the formatting of .log() */
static duk_ret_t duk_console_dir(duk_context *ctx) { return duk_console_log_helper(ctx, nullptr); }

static duk_ret_t duk_console_info(duk_context *ctx) { return duk_console_log_helper(ctx, nullptr); }

static duk_ret_t duk_console_warn(duk_context *ctx) { return duk_console_log_helper(ctx, nullptr); }

static duk_ret_t duk_console_error(duk_context *ctx) { return duk_console_log_helper(ctx, "Error"); }

static duk_ret_t duk_console_assert(duk_context *ctx) {
    if (duk_to_boolean(ctx, 0)) return 0;
    duk_remove(ctx, 0);
    return duk_console_log_helper(ctx, "Assertion failed");
}

static void duk_console_reg_vararg_func(duk_context *ctx, duk_c_function func, const char *name, LogLevel level) {
    duk_push_c_function(ctx, func, DUK_VARARGS);
    duk_push_string(ctx, "name");
    duk_push_string(ctx, name);
    duk_def_prop(ctx, -3,
                 DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_FORCE); /* Improve stacktraces by displaying function name */
    duk_set_magic(ctx, -1, (duk_int_t)level);
    duk_put_prop_string(ctx, -2, name);
}

void duk_console_init(duk_context *ctx) {
    duk_push_object(ctx);

    /* Custom function to format objects; user can replace.
     * For now, try JX-formatting and if that fails, fall back
     * to ToString(v).
     */
    duk_eval_string(ctx, R"(
(function (E) {
    return function format(v) {
        try {
            return E('jx', v);
        } catch (e) {
            return String(v);
        }
    };
})(Duktape.enc)
)");
    duk_put_prop_string(ctx, -2, "format");

    duk_console_reg_vararg_func(ctx, duk_console_log, "log", LOG_LEVEL_DEBUG);
    duk_console_reg_vararg_func(ctx, duk_console_log, "debug", LOG_LEVEL_DEBUG);
    duk_console_reg_vararg_func(ctx, duk_console_trace, "trace", LOG_LEVEL_DEBUG);
    duk_console_reg_vararg_func(ctx, duk_console_dir, "dir", LOG_LEVEL_DEBUG);
    duk_console_reg_vararg_func(ctx, duk_console_info, "info", LOG_LEVEL_INFO);
    duk_console_reg_vararg_func(ctx, duk_console_warn, "warn", LOG_LEVEL_WARN);
    duk_console_reg_vararg_func(ctx, duk_console_error, "error", LOG_LEVEL_ERROR);
    duk_console_reg_vararg_func(ctx, duk_console_error, "exception", LOG_LEVEL_ERROR);
    duk_console_reg_vararg_func(ctx, duk_console_assert, "assert", LOG_LEVEL_ERROR);

    duk_put_global_string(ctx, "console");
}
