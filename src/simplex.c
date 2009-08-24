/*
* mesa.c: Simplex Perlin Noise Lua Module
* Copyright (c) 2009, lua-noise authors
* See copyright information in the COPYRIGHT file
*/

typedef float GLfloat;

#include "mesa/prog_noise.h"

#include "luaheaders.h"

#define LUASIMPLEX_VERSION "0.1"
#define LUASIMPLEX_MT      "luasimplex-mt"

/*
* Based on Perlin Noise Math FAQ by M. Zucker
* TODO: Ensure it works fine for Simplex noise
*/
static double tiled_noise2(double x, double y, double w, double h)
{
  return
    (
      _mesa_noise2(    x,     y) * (w - x) * (h - y) +
      _mesa_noise2(x - w,     y) *     (x) * (h - y) +
      _mesa_noise2(x - w, y - h) *     (x) *     (y) +
      _mesa_noise2(    x, y - h) * (w - x) *     (y)
    ) / (w * h);
}

static int l_noise1(lua_State * L)
{
  double x = luaL_checknumber(L, 1);

  lua_pushnumber(L, _mesa_noise1(x));

  return 1;
}

static int l_noise2(lua_State * L)
{
  double x = luaL_checknumber(L, 1);
  double y = luaL_checknumber(L, 2);

  lua_pushnumber(L, _mesa_noise2(x, y));

  return 1;
}

static int l_noise3(lua_State * L)
{
  double x = luaL_checknumber(L, 1);
  double y = luaL_checknumber(L, 2);
  double z = luaL_checknumber(L, 3);

  lua_pushnumber(L, _mesa_noise3(x, y, z));

  return 1;
}

static int l_noise4(lua_State * L)
{
  double x = luaL_checknumber(L, 1);
  double y = luaL_checknumber(L, 2);
  double z = luaL_checknumber(L, 3);
  double t = luaL_checknumber(L, 4);

  lua_pushnumber(L, _mesa_noise4(x, y, z, t));

  return 1;
}

static int l_tiled_noise2(lua_State * L)
{
  double x = luaL_checknumber(L, 2);
  double y = luaL_checknumber(L, 3);
  double w = luaL_checknumber(L, 4);
  double h = luaL_checknumber(L, 5);

  lua_pushnumber(L, tiled_noise2(x, y, w, h));

  return 1;
}

/* Lua module API */
static const struct luaL_reg R[] =
{
	{ "noise1", l_noise1 },
	{ "noise2", l_noise2 },
	{ "noise3", l_noise3 },
	{ "noise4", l_noise4 },
	{ "tiled_noise2", l_tiled_noise2 },
	{ NULL, NULL }
};

#ifdef __cplusplus
extern "C" {
#endif

LUALIB_API int luaopen_simplex(lua_State * L)
{
  luaL_register(L, "simplex", R);
  lua_pushliteral(L, LUASIMPLEX_VERSION);
  lua_setfield(L, -2, "VERSION");

  return 1;
}

#ifdef __cplusplus
}
#endif
