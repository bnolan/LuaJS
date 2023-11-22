#include "definitions.h"

#include "jslua_async.h"
#include "jsvar_definitions.h"


EM_ASYNC_JS(int, jslua_await_int, (lua_State *state, int ref), {
  const val = Module.__getVarByRef(ref);
  try {
    const result = await val;
    Module.__pushVar(state, result);
  } catch (e) {
    Module.__pushVar(state, e.toString());
    return 0;
  }
  return 1;
});

int luajs_await(lua_State *L) {
  GET_SelfTypedPointerData();
  if (!jslua_await_int(L, data->ptr)) {
    lua_error(L);
  }
  return 1;
}
