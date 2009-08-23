/*
* perlin.c: Improved Perlin Noise Lua Module
* Copyright (c) 2009, lua-noise authors
* See copyright information in the COPYRIGHT file
*/

/*
* Improved Perlin noise generation algorithm
* Copyright (c) 2002 Ken Perlin
* http://mrl.nyu.edu/~perlin/noise/
*/

/*
* WARNING: This implementation has bugs: the noise seems to be not uniform
*          for some values. Use with caution.
*/

/* TODO: Fix it. */

#include <stdlib.h> /* For rand() */
#define RAND rand

#include "luaheaders.h"

#define LUAPERLIN_VERSION "0.1"
#define LUAPERLIN_MT      "luaperlin-mt"

#define NUM_PERMUTATIONS  (512)
#define HALF_PERMUTATIONS (256)

#define FASTFLOOR(x) (((x)>0) ? ((int)(x)) : (((int)(x)) - 1))

#define FADE(t) ((t) * (t) * (t) * ((t) * ((t) * 6.0f - 15.0f) + 10.0f))

#define LERP(t, a, b) ((a) + (t) * ((b) - (a)))

#define GRADH(hash) ((int)((hash) & 15))

#define GRADU(h, x, y) \
  ((double)((h) < 8 ? (x) : (y)))

#define GRADV(h, y, z) \
  ((double)((h) < 4 ? (y) : (h) == 12 || (h) == 14 ? (x) : (z)))

/* CONVERT LO 4 BITS OF HASH CODE */
/* INTO 12 GRADIENT DIRECTIONS.   */
#define GRAD(hash, x, y, z) \
  ( \
    ( \
      (GRADH(hash) & 1) == 0 \
        ? GRADU(GRADH(hash), (x), (y)) \
        : -GRADU(GRADH(hash), (x), (y)) \
    ) + \
    ( \
      (GRADH(hash) & 2) == 0 \
        ? GRADV(GRADH(hash), (y), (z)) \
        : -GRADV(GRADH(hash), (y), (z)) \
    ) \
  )

static double noise3(int * p, double x, double y, double z)
{
  int X = (int)FASTFLOOR(x) & 255,                   /* FIND UNIT CUBE THAT */
      Y = (int)FASTFLOOR(y) & 255,                   /* CONTAINS POINT.     */
      Z = (int)FASTFLOOR(z) & 255;

  double u, v, w;

  int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,      /* HASH COORDINATES OF */
      B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;      /* THE 8 CUBE CORNERS, */

  x -= FASTFLOOR(x);                                  /* FIND RELATIVE X,Y,Z */
  y -= FASTFLOOR(y);                                  /* OF POINT IN CUBE.   */
  z -= FASTFLOOR(z);

  u = FADE(x);                                       /* COMPUTE FADE CURVES */
  v = FADE(y);                                       /* FOR EACH OF X,Y,Z.  */
  w = FADE(z);

  return LERP(w, LERP(v, LERP(u, GRAD(p[AA  ], x    , y    , z     ),  /* AND ADD */
                                 GRAD(p[BA  ], x-1.0, y    , z     )), /* BLENDED */
                         LERP(u, GRAD(p[AB  ], x    , y-1.0, z     ),  /* RESULTS */
                                 GRAD(p[BB  ], x-1.0, y-1.0, z     ))),/* FROM  8 */
                 LERP(v, LERP(u, GRAD(p[AA+1], x    , y    , z-1.0 ),  /* CORNERS */
                                 GRAD(p[BA+1], x-1.0, y    , z-1.0 )), /* OF CUBE */
                         LERP(u, GRAD(p[AB+1], x    , y-1.0, z-1.0 ),
                                 GRAD(p[BB+1], x-1.0, y-1.0, z-1.0 ))));
}

#undef FADE
#undef LERP
#undef GRAD
#undef GRADU
#undef GRADV
#undef GRADH

/*
* Based on Perlin Noise Math FAQ by M. Zucker
*/
static double tiled_noise2(int * p, double x, double y, double w, double h)
{  
  return
    (
      noise3(p,     x,     y, 0.0) * (w - x) * (h - y) +
      noise3(p, x - w,     y, 0.0) *     (x) * (h - y) +
      noise3(p, x - w, y - h, 0.0) *     (x) *     (y) +
      noise3(p,     x, y - h, 0.0) * (w - x) *     (y)
    ) / (w * h);
}

/* Assumes permutations array is of least NUM_PERMUTATION elements size */
static void init_permutations(int * p)
{
  int i = 0;
  int j = 0;
  int k = 0;

  /* Fill permutations array */
  for (i = 0; i < HALF_PERMUTATIONS; ++i)
  {
    p[i] = i;
  }

  /* Shuffle permutations array */
  for (i = 0; i < HALF_PERMUTATIONS; ++i)
  {
    k = p[i];
    j = RAND() % HALF_PERMUTATIONS;
    p[i] = p[j];
    p[j] = k;
  }

  /* Duplicate permutations array */
  for (i = 0; i < HALF_PERMUTATIONS; ++i)
  {
    p[HALF_PERMUTATIONS + i] = p[i];
  }
}

static int l_noise1(lua_State * L)
{
  int * p = (int *)luaL_checkudata(L, 1, LUAPERLIN_MT);
  double x = luaL_checknumber(L, 2);
  double y = 0.0;
  double z = 0.0;

  lua_pushnumber(L, noise3(p, x, y, z));

  return 1;
}

static int l_noise2(lua_State * L)
{
  int * p = (int *)luaL_checkudata(L, 1, LUAPERLIN_MT);
  double x = luaL_checknumber(L, 2);
  double y = luaL_checknumber(L, 3);
  double z = 0.0;

  lua_pushnumber(L, noise3(p, x, y, z));

  return 1;
}

static int l_noise3(lua_State * L)
{
  int * p = (int *)luaL_checkudata(L, 1, LUAPERLIN_MT);
  double x = luaL_checknumber(L, 2);
  double y = luaL_checknumber(L, 3);
  double z = luaL_checknumber(L, 4);

  lua_pushnumber(L, noise3(p, x, y, z));

  return 1;
}

static int l_tiled_noise2(lua_State * L)
{
  int * p = (int *)luaL_checkudata(L, 1, LUAPERLIN_MT);
  double x = luaL_checknumber(L, 2);
  double y = luaL_checknumber(L, 3);
  double w = luaL_checknumber(L, 4);
  double h = luaL_checknumber(L, 5);

  lua_pushnumber(L, tiled_noise2(p, x, y, w, h));

  return 1;
}

static int l_new(lua_State * L)
{
  int * permutations = (int *)lua_newuserdata(
      L,
      NUM_PERMUTATIONS * sizeof(int)
    );

  init_permutations(permutations);

  /* Finish userdata initialization */
  luaL_getmetatable(L, LUAPERLIN_MT);
  lua_setmetatable(L, -2);

  return 1;
}

/* Methods */
static const struct luaL_reg M[] =
{
	{ "noise1", l_noise1 },
	{ "noise2", l_noise2 },
	{ "noise3", l_noise3 },
	{ "tiled_noise2", l_tiled_noise2 },
	{ NULL, NULL }
};

/* Lua module API */
static const struct luaL_reg R[] =
{
	{ "new", l_new },
	{ NULL, NULL }
};

#ifdef __cplusplus
extern "C" {
#endif

LUALIB_API int luaopen_perlin(lua_State * L)
{
  luaL_newmetatable(L, LUAPERLIN_MT);

  lua_newtable(L);
  luaL_register(L, NULL, M);
  lua_setfield(L, -2, "__index"); /* mt.__index = M */

  lua_pushliteral(L, LUAPERLIN_MT);
  lua_setfield(L, -2, "__metatable"); /* mt.__metatable = LUAPERLIN_MT */

  lua_pop(L, 1); /* Done with metatable. */

  luaL_register(L, "perlin", R);
  lua_pushliteral(L, LUAPERLIN_VERSION);
  lua_setfield(L, -2, "VERSION");

  return 1;
}

#ifdef __cplusplus
}
#endif
