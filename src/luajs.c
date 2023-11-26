#include "definitions.h"

#include "jslua_eval.h"
#include "jslua_async.h"

#include "jsvar.h"
#include "jsvar_array.h"
#include "jsvar_function.h"
#include "jsvar_object.h"

#include <lstate.h>

int luajs_eval(lua_State *L) {
  const char *str = lua_tostring(L, -1);
  lua_pop(L, 1);
  EM_ASM({ Module.__pushVar($0, eval(UTF8ToString($1))); }, L, str);
  return 1;
}

lua_State *jslua_new_state() {
  lua_State *L = luaL_newstate(); /* create state */
  lua_gc(L, LUA_GCSTOP, 0);       /* stop collector during initialization */
  luaL_openlibs(L);               /* open libraries */
  lua_gc(L, LUA_GCRESTART, 0);

  // Load myself
  lua_newtable(L);

  luaL_Reg reg_jsmain[] = {
    {"eval", luajs_eval},
    {"await", luajs_await},
    {NULL, NULL},
  };
  luaL_setfuncs(L, reg_jsmain, 0);

  luajs_jsarray_init(L);
  luajs_jsobject_init(L);
  luajs_jsfunction_init(L);
  luajs_jsvar_init(L);

  lua_setglobal(L, "js");
  // END: Load myself

  // Load js.global
  lua_getglobal(L, "js");

  lua_pushstring(L, "global");
  jslua_pushvar(L, -1, TYPE_JSOBJECT);
  lua_rawset(L, -3);

  lua_pop(L, 1);
  // END: Load js.global

  return L;
}

global_State *jslua_get_state_global(lua_State *L) { return G(L); }

void jslua_delete_state(lua_State *L) { lua_close(L); }

int* jslua_alloc_int() {
  int* ptr = malloc(sizeof(int));
  return ptr;
}

double jslua_read_int(int* ptr) {
  return *ptr;
}

size_t* jslua_alloc_size_t() {
  size_t* ptr = malloc(sizeof(size_t));
  return ptr;
}

double jslua_read_size_t(size_t* ptr) {
  return *ptr;
}

int main() {
  EM_ASM({
    Module.__onready();
    delete Module.__onready;
  });
  emscripten_exit_with_live_runtime();
  return 0;
}