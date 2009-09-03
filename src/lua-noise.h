#ifndef LUANOISE_H_INCLUDED_
#define LUANOISE_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

LUALIB_API int luaopen_simplex(lua_State * L);
LUALIB_API int luaopen_perlin(lua_State * L);

#ifdef __cplusplus
}
#endif


#endif /* LUANOISE_H_INCLUDED_ */
