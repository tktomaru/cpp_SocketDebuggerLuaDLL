#include "pch.h"
#include "Common.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream> // std::cout
#include <regex>

using namespace std;
namespace fs = filesystem;

int isExists(const fs::path& p, fs::file_status s = fs::file_status{})
{
	if (fs::status_known(s) ? fs::exists(s) : fs::exists(p))
		return 1;
	else
		// does not exist
		return 0;
}

/**
* @brief フォルダ以下のファイル一覧を取得する関数
* @param[in]    folderPath  フォルダパス
* @param[out]   file_names  ファイル名一覧
* return        true:成功, false:失敗
*/
int getFileNames(lua_State* L)
//bool getFileNames(std::string folderPath, std::vector<std::string>& file_names)
{
	using namespace std::filesystem;
	std::error_code err;

	std::string		folderPath;				// ﾊﾟﾗﾒｰﾀ:ﾌｧｲﾙ名称
	int			iAnswer = 0;			// 戻り値
	std::vector<std::string> file_names;
	directory_iterator iter, end;
	int folderExist=0;

	//----------------------------------------------
	// 引数の数をﾁｪｯｸ
	//----------------------------------------------
	int n = lua_gettop(L);
	if (n <= 0 || n > 1) {
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// 引数の解析＆取得
	//----------------------------------------------
	// 引数1 ﾌｧｲﾙ名称の型ﾁｪｯｸ
	
	if (lua_isstring(L, 1) != 1) {
		// ﾌｧｲﾙ名称文字列認識ｴﾗｰ
		iAnswer = 2;
		goto ExitBlock;
	}

	folderPath = lua_tostring(L, 1);

	folderExist=isExists(folderPath);

	if (folderExist != 1) {
		// フォルダ認識ｴﾗｰ
		iAnswer = 3;
		goto ExitBlock;
	}

	iter = std::filesystem::directory_iterator(folderPath);
	
	for (; iter != end && !err; iter.increment(err)) {
		const directory_entry entry = *iter;
		std::string fileName = entry.path().string();
		file_names.push_back(entry.path().string());
	}
	lua_pushVectorString(L, file_names);

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

vector<string> split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

// Value-Defintions of the different String values
static enum StringValue {
	evNotDefined,
	evStringValue1,
	evStringValue2,
	evStringValue3,
	evStringValue4,
	evStringValue5,
	evStringValue6,
	evStringValue7,
	evStringValue8,
	evStringValue9,
	evStringValue10,
	evStringValue11,
	evStringValue12,
	evStringValue13,
	evEnd
};
// Map to associate the strings with the enum values
static std::map<std::string, StringValue> s_mapStringValues;

typedef struct _tval
{
	LONG64         start;
	LONG64         length;
	LONG64      value;
}tval;
typedef struct _tcol
{
	string      filename;
	string      help;
	LONG64         count;
	LONG64         maxcount;
	vector<tval> value;
	string      str1;
	string      str2;
	LONG64         int1;
	LONG64         int2;
} tcol;

typedef struct _tret
{
	vector< tcol > col;
} tret;


void lua_pushtableEx(lua_State* L, tret ary)
{
	BYTE* lpBuffer = NULL;
	int sum = 0;
	DWORD ivec = 0;
	int optnum = 0;
	int stacksize = 1;

	lua_newtable(L);
	int c = lua_gettop(L);
	for (; ivec < ary.col.size(); ivec++) {
		lua_checkstack(L, stacksize = stacksize + 2);
		lua_pushnumber(L, ivec+1);
		lua_newtable(L);

		lua_checkstack(L, stacksize = stacksize + 2);
		lua_pushString(L, "filename");
		lua_pushString(L, ary.col[ivec].filename);
		lua_settable(L, -3);

		lua_checkstack(L, stacksize = stacksize + 2);
		lua_pushString(L, "help");
		lua_pushString(L, ary.col[ivec].help);
		lua_settable(L, -3);

		lua_checkstack(L, stacksize = stacksize + 2);
		lua_pushString(L, "count");
		lua_pushnumber(L, ary.col[ivec].count);
		lua_settable(L, -3);

		lua_checkstack(L, stacksize = stacksize + 2);
		lua_pushString(L, "maxcount");
		lua_pushnumber(L, ary.col[ivec].maxcount);
		lua_settable(L, -3);

		if (ary.col[ivec].int1) {
			lua_checkstack(L, stacksize = stacksize + 2);
			lua_pushnumber(L, optnum++);
			lua_pushnumber(L, ary.col[ivec].int1);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].int2) {
			lua_checkstack(L, stacksize = stacksize + 2);
			lua_pushnumber(L, optnum++);
			lua_pushnumber(L, ary.col[ivec].int2);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].str1 != "") {
			lua_checkstack(L, stacksize = stacksize + 2);
			lua_pushnumber(L, optnum++);
			lua_pushString(L, ary.col[ivec].str1);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].str2 != "") {
			lua_checkstack(L, stacksize = stacksize + 2);
			lua_pushnumber(L, optnum++);
			lua_pushString(L, ary.col[ivec].str2);
			lua_settable(L, -3);
		}

		lua_checkstack(L, stacksize = stacksize + 2);
		lua_pushString(L, "value");
		lua_newtable(L);
		int c2 = lua_gettop(L);
		for (DWORD ival = 0; ival < ary.col[ivec].value.size(); ival++) {
			lua_checkstack(L, stacksize = stacksize + 2);
			lua_pushnumber(L, ival+1);
			lua_newtable(L);

			lua_checkstack(L, stacksize = stacksize + 2);
			lua_pushString(L, "start");
			lua_pushnumber(L, ary.col[ivec].value[ival].start);
			lua_settable(L, -3);

			lua_checkstack(L, stacksize = stacksize + 2);
			lua_pushString(L, "length");
			lua_pushnumber(L, ary.col[ivec].value[ival].length);
			lua_settable(L, -3);

			lua_checkstack(L, stacksize = stacksize + 2);
			lua_pushString(L, "value");
			lua_pushnumber(L, ary.col[ivec].value[ival].value);
			// lua_setfield(L, -2, ivec + 1);
			lua_settable(L, -3);

			lua_settable(L, c2);
		}
		lua_settable(L, -3);
		lua_settable(L, c);
	}
}

std::string ReplaceString
(
	std::string StringTarget   // 置き換え対象
	, std::string StringFrom   // 検索対象
	, std::string StringTo   // 置き換える内容
);

string reg(string s)
{
	string out;
	std::smatch match;
	if ( std::regex_search(s, match, std::regex("\".*?\"")) ) {
		out = match.str();      //  (マッチした文字列)
		//auto position = results.position(); //   (マッチした文字列の位置)
		//auto length = results.length();   //   (マッチした文字列の長さ)
		out = ReplaceString(out, "\"", "");
	}
	if (std::regex_search(s, match, std::regex("\'.*?\'"))) {
		out = match.str();      //  (マッチした文字列)
		//auto position = results.position(); // (マッチした文字列の位置)
		//auto length = results.length();   //   (マッチした文字列の長さ)
		out = ReplaceString(out, "\'", "");
	}
	return out;
}


/*
	string中の特定文字列をstringで置換する（作りかけ）
*/
std::string ReplaceString
(
	std::string StringTarget   // 置き換え対象
	, std::string StringFrom   // 検索対象
	, std::string StringTo   // 置き換える内容
)
{
	string strE{'"'};
	string strQ{"'"};

	std::string::size_type  Pos(StringTarget.find(StringFrom));

	while (Pos != std::string::npos)
	{
		// 置換
		StringTarget.replace(Pos, StringFrom.length(), StringTo);
		Pos = StringTarget.find(StringFrom, Pos + StringTo.length());
	}

	return StringTarget;
}

int UtilStoi(const std::string& str, LONG64* p_value, std::size_t* pos = 0, int base = 10) {
	// wrapping std::stoi because it may throw an exception

	try {
		*p_value = std::stoll(str, pos, base);
		return 0;
	}

	catch (const std::invalid_argument& ia) {
		//std::cerr << "Invalid argument: " << ia.what() << std::endl;
		return 5;
	}

	catch (const std::out_of_range& oor) {
		//std::cerr << "Out of Range error: " << oor.what() << std::endl;
		return 6;
	}

	catch (const std::exception& e)
	{
		//std::cerr << "Undefined error: " << e.what() << std::endl;
		return 7;
	}
}

int getIniFile(lua_State* L)
{
	using namespace std::filesystem;
	std::error_code err;

	std::string		inifilePath;				// ﾊﾟﾗﾒｰﾀ:ﾌｧｲﾙ名称
	int			iAnswer = 0;			// 戻り値
	std::vector<std::string> file_names;
	directory_iterator iter, end;
	int folderExist = 0;
	std::ifstream ifs;
	std::string strline;
	vector<string> setting;
	tret ret = {};
	int sret;
	

	//----------------------------------------------
	// 引数の数をﾁｪｯｸ
	//----------------------------------------------
	int n = lua_gettop(L);
	if (n <= 0 || n > 1) {
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// 引数の解析＆取得
	//----------------------------------------------
	// 引数1 ﾌｧｲﾙ名称の型ﾁｪｯｸ

	if (lua_isstring(L, 1) != 1) {
		// ﾌｧｲﾙ名称文字列認識ｴﾗｰ
		iAnswer = 2;
		goto ExitBlock;
	}

	inifilePath = lua_tostring(L, 1);

	folderExist = isExists(inifilePath);

	if (folderExist != 1) {
		// フォルダ認識ｴﾗｰ
		iAnswer = 3;
		goto ExitBlock;
	}

	ifs.open(inifilePath);

	if (ifs.fail())
	{
		// ファイル認識ｴﾗｰ
		iAnswer = 3;
		goto ExitBlock;
	}

	// 一行目の設定を読み込む
	int ilineNum = 0;
	LONG64 ivectnum = 0;
	s_mapStringValues["filename"] = evStringValue1;
	s_mapStringValues["num"] = evStringValue2;
	s_mapStringValues["dec"] = evStringValue3;
	s_mapStringValues["hex"] = evStringValue4;
	s_mapStringValues["start"] = evStringValue5;
	s_mapStringValues["length"] = evStringValue6;
	s_mapStringValues["help"] = evStringValue7;
	s_mapStringValues["int1"] = evStringValue8;
	s_mapStringValues["int2"] = evStringValue9;
	s_mapStringValues["str1"] = evStringValue10;
	s_mapStringValues["str2"] = evStringValue11;
	s_mapStringValues["count"] = evStringValue12;
	s_mapStringValues["maxcount"] = evStringValue13;

	while (getline(ifs, strline))
	{
		// strlineに一行の文字列が入っている
		// コメント行は読み飛ばす
		if (strline.substr(0, 1) == ";") {
			continue;
		}
		if (strline.empty()) {
			continue;
		}
		if (strline == "") {
			continue;
		}

		if (ilineNum == 0) {
			// ,区切りで分割する
			// 行は設定が入っている
			strline = ReplaceString(strline, " ", "");
			setting = split(strline, ',');
		}
		else {
			ivectnum = -1;
			int hexOrDecNum = 0;
			// 実際の値を判別する
			tcol col1 = {};
			tval value = { 0,0,0 };
			strline = ReplaceString(strline, " ", "");
			vector<string> strvec = split(strline, ',');
			for (int ii = 0; ii < strvec.size() && ii < setting.size(); ii++) {

				string strcolumn = reg(strvec[ii]);

				if (strcolumn.empty()) {
					strcolumn = ReplaceString(strvec[ii], " ", "");
				}
				switch (s_mapStringValues[setting[ii]]) {
				case evStringValue1:
					// filename
					col1.filename = strcolumn;
					break;
				case evStringValue2:
					// num
					sret = UtilStoi(strcolumn, &ivectnum);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue3:
					// dec
					sret = UtilStoi(strcolumn, &value.value);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					hexOrDecNum++;
					break;
				case evStringValue4:
					// hex
					hexOrDecNum++;
					sret = UtilStoi(strcolumn, &value.value, nullptr, 16);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue5:
					// start
					sret = UtilStoi(strcolumn, &value.start);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue6:
					// length
					sret = UtilStoi(strcolumn, &value.length);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue7:
					// help
					col1.help = strcolumn;
					break;
				case evStringValue8:
					// int1
					sret = UtilStoi(strcolumn, &col1.int1);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue9:
					// int2
					sret = UtilStoi(strcolumn, &col1.int2);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue10:
					// str1
					col1.str1 = strcolumn;
					break;
				case evStringValue11:
					// str2
					col1.str2 = strcolumn;
					break;
				case evStringValue12:
					// str2
					sret = UtilStoi(strcolumn, &col1.count);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue13:
					// maxcount
					sret = UtilStoi(strcolumn, &col1.maxcount);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				default:
					// 予期しない文字列はエラー
					iAnswer = 4;
					goto ExitBlock;
					break;
				}
				if (value.length != 0 && value.start != 0 && value.value != 0) {
					col1.value.push_back(value);
					value = { 0,0,0 };
				}
			}
			if (col1.filename == "") {
				iAnswer = 8;
				goto ExitBlock;
			}
			if (col1.value.empty()) {
				iAnswer = 9;
				goto ExitBlock;
			}
			ret.col.push_back(col1);
			col1.value.clear();
		}
		ilineNum++;
		lua_pushtableEx(L, ret);
	}

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