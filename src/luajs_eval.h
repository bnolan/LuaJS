#ifndef _luajs_EVAL_H_INCLUDED

int luajs_call(lua_State *L, int argcount);
int luajs_execute(lua_State *L, char *str, size_t len, char *name);
int luajs_eval(lua_State *L);

#define _luajs_EVAL_H_INCLUDED
#endif
