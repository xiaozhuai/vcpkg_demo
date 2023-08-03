/**
 * Copyright 2022 xiaozhuai
 */

// clang-format off
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"
// clang-format on

static lua_State *L;

template <class T>
static T identityCFunction(T value) {
    return value;
}

static void runLua(const std::string &script) {
    if (luaL_loadstring(L, script.c_str()) != 0) {
        throw std::runtime_error(lua_tostring(L, -1));
    }
    if (lua_pcall(L, 0, 0, -2) != 0) {
        throw std::runtime_error(lua_tostring(L, -1));
    }
}

template <class T = luabridge::LuaRef>
static T result() {
    return luabridge::getGlobal(L, "result");
}

class Test {
public:
    Test(int val, int val2) : value(val), value2(val2) { printf("Test(%d, %d)\n", val, val2); }

    ~Test() { printf("~Test()\n"); }

    int add(int a) {
        value += a;
        return value;
    }

    static int sum(int a, int b) { return a + b; }

    int value = 0;
    int value2 = 0;
};

int main() {
    L = luaL_newstate();
    luaL_openlibs(L);
    luabridge::getGlobalNamespace(L)
        .addFunction("boolFn", &identityCFunction<bool>)
        .addFunction("uint8Fn", &identityCFunction<uint8_t>)
        .addFunction("intFn", &identityCFunction<int>)
        .addFunction("uintFn", &identityCFunction<unsigned int>)
        .addFunction("floatFn", &identityCFunction<float>)
        .addFunction("doubleFn", &identityCFunction<double>)
        .addFunction("charFn", &identityCFunction<char>)
        .addFunction("cstringFn", &identityCFunction<const char *>)
        .addFunction("stringFn", &identityCFunction<std::string>)
        .beginClass<Test>("Test")
        .addConstructor<void (*)(int, int)>()
        .addProperty("value", &Test::value, true)
        .addProperty("value2", &Test::value2, true)
        .addFunction("add", &Test::add)
        .addStaticFunction("sum", &Test::sum)
        .endClass();

    {
        runLua("result = boolFn(false)");
        std::cout << "result: " << result() << "\n";
        assert(result().isBool());
        assert(result<bool>() == false);
    }

    {
        runLua("result = uint8Fn(255)");
        std::cout << "result: " << result() << "\n";
        assert(result().isNumber());
        assert(result<uint8_t>() == 255);
    }

    {
        runLua("result = cstringFn('abc')");
        std::cout << "result: " << result() << "\n";
        assert(result().isString());
        assert(result<std::string>() == "abc");
    }

    {
        runLua("result = stringFn('abc')");
        std::cout << "result: " << result() << "\n";
        assert(result().isString());
        assert(result<std::string>() == "abc");
    }

    {
        runLua("result = {1, 2}");
        std::cout << "result: " << result() << "\n";
    }

    {
        runLua(R"(
test = Test(10, 0);
print(test.value);
print(test:add(5));
print(Test.sum(1, 2));
test = nil;

test2 = Test(10, 12);
print(test2.value);
print(test2.value2);
test2.value2 = 100;
print(test2.value2);
test2 = nil;
)");
    }

    lua_close(L);
    return 0;
}
