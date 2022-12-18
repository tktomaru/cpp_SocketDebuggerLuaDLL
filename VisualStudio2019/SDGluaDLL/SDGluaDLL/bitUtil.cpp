#include "pch.h"
#include "Common.h"

#ifdef DOUBLE
#define BIT_BMAX 52
#define BIT_NMAX 0xfffffffffffff
#else
#define BIT_BMAX 32
#define BIT_NMAX 0xffffffff
#endif

#define BIT_XOP(BIT_XOP)						\
	uint64_t oper = lua_tonumber(L, 1);		        \
	const int args = lua_gettop(L);					\
													\
	for (int i = 2; i <= args; i++) {				\
		uint64_t oper2 = lua_tonumber(L, i);    	\
		oper BIT_XOP oper2;							\
	}												\
													\
	lua_pushnumber(L, oper);						\
	return 1;										\


int bitor(lua_State* L) {
	BIT_XOP(|= );
}

int bitand(lua_State* L) {
	BIT_XOP(&= );
}

int bitxor(lua_State* L) {
	BIT_XOP(^= );
}

int bitunset(lua_State* L) {
	BIT_XOP(&= ~);
}

int bitnot(lua_State* L) {
	lua_pushnumber(L,
		(~((uint64_t)lua_tonumber(L, 1))) & BIT_NMAX);
	return 1;
}

int bitshl(lua_State* L) {
	uint64_t oper = lua_tonumber(L, 1);
	oper <<= lua_tointeger(L, 2);
	if (oper > BIT_NMAX) {
		//return luaL_error(L, "arithmetic overflow");
		return 1;
	}
	else {
		lua_pushnumber(L, oper);
		return 1;
	}
}

int bitashr(lua_State* L) {
	int64_t oper = lua_tonumber(L, 1);
	lua_pushnumber(L, oper >> lua_tointeger(L, 2));
	return 1;
}

int bitshr(lua_State* L) {
	uint64_t oper = lua_tonumber(L, 1);
	lua_pushnumber(L, oper >> lua_tointeger(L, 2));
	return 1;
}
