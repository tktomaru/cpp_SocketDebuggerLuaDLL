// pch.h: プリコンパイル済みヘッダー ファイルです。
// 次のファイルは、その後のビルドのビルド パフォーマンスを向上させるため 1 回だけコンパイルされます。
// コード補完や多くのコード参照機能などの IntelliSense パフォーマンスにも影響します。
// ただし、ここに一覧表示されているファイルは、ビルド間でいずれかが更新されると、すべてが再コンパイルされます。
// 頻繁に更新するファイルをここに追加しないでください。追加すると、パフォーマンス上の利点がなくなります。

#ifndef PCH_H
#define PCH_H

// プリコンパイルするヘッダーをここに追加します
#include "framework.h"


//####################################################################################
//# Luaのライブラリを宣言
//####################################################################################
/*
<< 本サンプルで使用したLuaの概要 >>
バージョン          Lua 5.1.2
種類                Windows x86 Static Library and Includes
					(Visual C++ 6 Compatible)
ファイル名称        lua5_1_2_Win32_vc6_lib.zip
ダウンロードURL     http://luabinaries.luaforge.net/download.html

<< ダウンロードしたファイルの配置 >>
SampleLuaEx         <-Lua拡張DLLのプロジェクトフォルダ
├ include          <-ダウンロードしたLuaライブラリのインクルード用ヘッダフォルダ
└ lua5.1.lib       <-ダウンロードしたLuaライブラリのlibファイル
*/
#include "include/lua.hpp"
#pragma comment(lib, "lua5.1.lib")
#pragma comment(linker, "/NODEFAULTLIB:LIBC.LIB")


#endif //PCH_H
