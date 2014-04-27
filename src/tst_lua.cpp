#if defined(WIN32)
#define STDCALL __stdcall
#elif defined (__GNUG__)     /*  gcc  IIRC */
#define STDCALL
#endif

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdlib.h>
#include <stdio.h>

    // expose c's API
    int barfunc(int foo)
    {
        /* a dummy function to test with FFI */
        return foo + 1;
    }

    // more complex demo
    typedef struct _token_ctx {
        int a;
        char val[255];
    }token_ctx;

    // must replace with __stdcall in ffi define. compat for windows
    typedef int (STDCALL *charlevel_callback_proto)(token_ctx*, const char*, int);
    //typedef int (*charlevel_callback_proto)(const char*, int);
    int must_call_callback(charlevel_callback_proto func, const char* msg){
        if(func) {
            return (func)(NULL, msg, 100);
        }
        return 0;
    }

    // regisit ?
    /*
     *  定义一个全局的管理程序, 管理当前活跃的需要被 LUA 调用的对象(实际上应该只有一个);
     *  传入 LUA 代码此对象ID,
     *  后续注册操作, 皆通过本 ID 访问
     *
     */
} // end extern "C"

int
main(int argc, char* argv[])
{
    int status, result;
    lua_State *L;
    L = luaL_newstate();

    /*
    luaopen_io(L); // provides io.*
    luaopen_base(L);
    luaopen_table(L);
    luaopen_string(L);
    luaopen_math(L);
    luaopen_loadlib(L);
     */
    luaL_openlibs(L);  // grant all permission.

    /* Load the file containing the script we are going to run */
    status = luaL_loadfile(L, argv[1]);
    if (status) {
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
        exit(1);
    }

    /* Ask Lua to run our little script */
    if(1)
    {
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (result) {
            fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
            exit(1);
        }
    }

    /* Manual call lua function from c side. */
    {
        double z;
        /* push functions and arguments */
        lua_getglobal(L, "f");  /* function to be called */
        lua_pushnumber(L, 10);   /* push 1st argument */
        lua_pushnumber(L, 30);   /* push 2nd argument */

        /* do the call (2 arguments, 1 result) */
        if (lua_pcall(L, 2, 1, 0) != 0)
            printf("error running function `f': %s",
                 lua_tostring(L, -1));

        /* retrieve result */
        if (!lua_isnumber(L, -1))
            printf("function `f' must return a number");
        z = lua_tonumber(L, -1);
        lua_pop(L, 1);  /* pop returned value */
        printf("get function 'f' result=%f\n", z);
    }
    lua_close(L);   /* Cya, Lua */

    return 0;
}
