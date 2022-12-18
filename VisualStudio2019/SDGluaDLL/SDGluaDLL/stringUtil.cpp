#include "pch.h"
#include "Common.h"
#include <string>

int getSubStr(lua_State* L)
//bool getFileNames(std::string folderPath, std::vector<std::string>& file_names)
{

	std::string		strinput;				// ﾊﾟﾗﾒｰﾀ:切り出し元文字列
	std::string		stroutput;				// ﾊﾟﾗﾒｰﾀ:切り出し結果
	int             istart, iend;
	int			iAnswer = 0;			// 戻り値
	std::vector<std::string> file_names;
	int folderExist = 0;

	//----------------------------------------------
	// 引数の数をﾁｪｯｸ
	//----------------------------------------------
	int n = lua_gettop(L);
	if (n <= 0 || n > 3) {
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// 引数の解析＆取得
	//----------------------------------------------
	// 引数1 ﾌｧｲﾙ名称の型ﾁｪｯｸ

	if (lua_isstring(L, 1) != 1) {
		// 切り出し元文字列認識ｴﾗｰ
		iAnswer = 2;
		goto ExitBlock;
	}

	strinput = lua_tostring(L, 1);
	if (lua_isnumber(L, 2) != 1) {
		// スタート位置の型ﾁｪｯｸの型ﾁｪｯｸｴﾗｰ
		iAnswer = 4;
		goto ExitBlock;
	}
	istart = (int)lua_tonumber(L, 2);
	if (lua_isnumber(L, 3) != 1) {
		// 切り出しサイズの型ﾁｪｯｸの型ﾁｪｯｸｴﾗｰ
		iAnswer = 4;
		goto ExitBlock;
	}
	iend = (int)lua_tonumber(L, 3);

	if (strinput.size() < istart + iend) {
		// 切り出しサイズのエラー
		iAnswer = 5;
		goto ExitBlock;
	}
	stroutput = strinput.substr(istart, iend);
	lua_pushString(L, stroutput);

ExitBlock:
	if (iAnswer > 0) {
		// ｴﾗｰ発生時はﾊﾟﾗﾒｰﾀｾｯﾄ
		lua_pushnumber(L, iAnswer);
		// ﾊﾟﾗﾒｰﾀ数を返す
		return 1;
	}
	// ﾊﾟﾗﾒｰﾀ数を返す
	return 1;
}
