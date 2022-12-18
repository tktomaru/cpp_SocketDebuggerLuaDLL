#include "pch.h"
#include <string>
#include <vector>
/*
######################################################################################
--------------------------------------------------------------------------------------
--  Lua配列を扱いやすくするための拡張関数群
--------------------------------------------------------------------------------------
<<概要>>
配列のアドレスとサイズを指定して、Luaスタックに積み上げる。

void lua_pushtableEx( lua_State *L, void* lpBuffer, DWORD count )

<<引数>>
L               Luaポインタ
lpBuffer        配列先頭アドレス
count           配列の個数
<<戻り値>>
無し

======================================================================================
<<概要>>
COleSafeArrayを指定して、配列をLuaスタックに積み上げる。

void lua_pushtableEx( lua_State *L, COleSafeArray& ary )

<<引数>>
L               Luaポインタ
ary             配列オブジェクト
<<戻り値>>
無し

======================================================================================
<<概要>>
Luaスタックの番号を指定して、配列データをCOleSafeArrayオブジェクトに取り出す。

COleSafeArray lua_totableEx( lua_State *L, int no )

<<引数>>
L               Luaポインタ
no              取得するスタック番号
<<戻り値>>
取り出したテーブルが格納されたCOleSafeArrayオブジェクト

--------------------------------------------------------------------------------------
######################################################################################
*/
void lua_pushtableEx(lua_State* L, void* lpBuffer, DWORD count)
{
	lua_createtable(L, 0, count);
	for (DWORD ix = 0; ix < count; ix++) {
		lua_pushnumber(L, ((LPBYTE)lpBuffer)[ix]);
		lua_rawseti(L, -2, ix + 1);
	}
}
void lua_pushtableEx(lua_State* L, COleSafeArray& ary)
{
	BYTE* lpBuffer = NULL;
	ary.AccessData((void**)& lpBuffer);
	lua_pushtableEx(L, lpBuffer, ary.GetOneDimSize());
	ary.UnaccessData();
}
void lua_pushString(lua_State* L, std::string ary)
{
	BYTE* lpBuffer = NULL;
	const char* c = ary.c_str();
	lua_pushstring(L, c);
}
void lua_pushVectorString(lua_State* L, std::vector<std::string> ary)
{
	BYTE* lpBuffer = NULL;
	int sum=0;

	lua_newtable(L);

	for (DWORD ivec = 0; ivec < ary.size(); ivec++) {
		const char* c = ary[ivec].c_str();
		lua_pushnumber(L, ivec);
		lua_pushstring(L, c);
		// lua_setfield(L, -2, ivec + 1);
		lua_settable(L,-3);
	}
}
COleSafeArray lua_totableEx(lua_State* L, int no)
{
	DWORD			bufferCount = 0;
	DWORD			bufferSize = 1024;
	BYTE* lpBuffer = NULL;
	COleSafeArray	ary;

	ary.CreateOneDim(VT_UI1, 0);							// 空配列の作成
	lpBuffer = (BYTE*)::GlobalAlloc(GPTR, bufferSize);	// 初期ﾊﾞｯﾌｧの作成

	// 配列を取る
	for (DWORD ix = 1; ; ix++) {
		// 値を一つ取り出す
		lua_rawgeti(L, no, ix);
		// NULLﾁｪｯｸ
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		// 数値以外ﾁｪｯｸ
		if (1 != lua_isnumber(L, -1)) {
			lua_pop(L, 1);
			break;
		}
		// ﾊﾞｯﾌｧに積み込み
		lpBuffer[bufferCount] = (BYTE)lua_tonumber(L, -1);
		lua_pop(L, 1);
		bufferCount++;
		// ﾊﾞｯﾌｧが尽きれば倍に拡張する
		if (bufferCount >= bufferSize) {
			BYTE* lpNewAddr = (BYTE*)::GlobalAlloc(GPTR, bufferSize * 2);
			CopyMemory(lpNewAddr, lpBuffer, bufferSize);
			::GlobalFree(lpBuffer);
			lpBuffer = lpNewAddr;
			bufferSize *= 2;
		}
	}
	// ｵﾌﾞｼﾞｪｸﾄに追加する
	if (bufferCount > 0)
		ary.CreateOneDim(VT_UI1, bufferCount, lpBuffer);
	::GlobalFree(lpBuffer);
	lpBuffer = NULL;
	return ary;
}
