#include "pch.h"
#include "Common.h"

/*
######################################################################################
--------------------------------------------------------------------------------------
--  <<SocketDebuggerリンケージ>>
--
--  モジュール名称取得
--------------------------------------------------------------------------------------
<<概要>>
本モジュールを元に、SocketDebuggerは対応DLLであることを判断します。
本モジュールの count を0から順次、1,2...と呼び出し、
モジュール名称を取得しようとします。

戻り値0、及びモジュール名を返すことにより、呼び出せるモジュール名を認識します。
戻り値1を返した場合は、呼び出せるモジュールは全て取得したと判断します。

取得できたモジュールは、SocketDebugger内で利用可能となります。

<<引数>>
lpName                モジュール名称を格納するアドレス
                      (ここにモジュール名称を格納する)
bufferCount           バッファlpNameの利用可能バイト数
count                 関数名称取り出し番号
                      SocketDebuggerは0から1,2...と値を変えて
					  戻り値が0以外となるまで繰り返す呼び出します。

<<戻り値>>
                      0:正常終了  関数有り(lpNameの名称有効)
                      1:関数無し
                      2:格納サイズ不足

--------------------------------------------------------------------------------------
######################################################################################
*/
DWORD GetModuleName(char* lpName, int bufferCount, int count ){
	DWORD	dwResult = 0;
	TCHAR buf[128];

	// 名称選択
	switch( count ){
	case 0:		lstrcpy(buf, (TCHAR*)"FileReadEx" );	break;
	case 1:		lstrcpy(buf, (TCHAR*)"FileWriteEx" );	break;
	case 2:		lstrcpy(buf, (TCHAR*)"getFileNames");	break;
	case 3:		lstrcpy(buf, (TCHAR*)"getSubStr");	break;
	case 4:		lstrcpy(buf, (TCHAR*)"getIniFile");	break;
	case 5:		lstrcpy(buf, (TCHAR*)"bitor");	break; 
	case 6:		lstrcpy(buf, (TCHAR*)"bitand");	break;
	case 7:		lstrcpy(buf, (TCHAR*)"bitxor");	break;
	case 8:		lstrcpy(buf, (TCHAR*)"bitunset");	break;
	case 9:		lstrcpy(buf, (TCHAR*)"bitnot");	break;
	case 10:	lstrcpy(buf, (TCHAR*)"bitshl");	break;
	case 11:	lstrcpy(buf, (TCHAR*)"bitashr");	break;
	case 12:	lstrcpy(buf, (TCHAR*)"bitshr");	break;
	default:	return 1;
	}

	// ﾊﾞｯﾌｧｺﾋﾟｰ
	if( strlen((const char *)buf)+1 > (DWORD)bufferCount )	return 2;
	lstrcpy((TCHAR*)lpName, buf );
	return 0;
}
/*
######################################################################################
--------------------------------------------------------------------------------------
--  <<スクリプト追加モジュール>>
--
--  拡張ファイル読み込み
--------------------------------------------------------------------------------------
<<概要>>
ファイル読み込みの拡張処理

result = FileReadEx( sFileName )
result = FileReadEx( sFileName, iStartPosition )
result = FileReadEx( sFileName, iStartPosition, iSize )

<<引数>>
sFileName(文字列)     読み込むファイル名称
iStartPosition(数値)  読み込み開始ﾊﾞｲﾄ位置(省略可)
                      省略時、負値は先頭から読み込み
iSize(数値)           読み込みﾊﾞｲﾄ数(省略可)
                      省略時、負値、0の場合は全て読み込み

<<戻り値>>
テーブル              読み込んだデータ
数値                  異常終了
                      1:パラメータエラー
                      2:ファイル名称文字列認識エラー
                      3:読込開始バイト位置の型チェックエラー
                      4:読込サイズの型チェックの型チェックエラー
                      5:ファイル情報取得エラー
                      6:ファイルサイズ無し
                      7:読み込み開始位置はEOF超過
                      8:指定された読み込みバイト数に満たなかった
                      9:ファイル読み込みエリア確保エラー
                      10:ファイルオープンエラー
                      11:ファイル読み込みエラー
                      12:ファイル読み込みその他のエラー

<<例>>
(1)
result = FileReadEx( "c:\\test.txt" )
└指定されたファイルを全て読み込む

(2)
result = FileReadEx( "c:\\test.txt", 16 )
└指定されたファイルの16バイト目から以降全て読み込む

(3)
result = FileReadEx( "c:\\test.txt", 16, 32 )
└指定されたファイルの16バイト目から32バイト分読み込む

--------------------------------------------------------------------------------------
######################################################################################
*/
int FileReadEx(lua_State *L)
{
	CString		sFilename;				// ﾊﾟﾗﾒｰﾀ:ﾌｧｲﾙ名称
	int			iReadPosition = 0;		// 読み込み開始位置
	int			iReadSize = 0;			// 読み込みｻｲｽﾞ

	int			iAnswer = 0;			// 戻り値
	BYTE		*lpBuffer = NULL;		// ﾌｧｲﾙ読み込みﾊﾞｯﾌｧﾎﾟｲﾝﾀ
	UINT		uiReadSize;				// 読み込みｻｲｽﾞ
	CFileStatus status;					// ﾌｧｲﾙｽﾃｰﾀｽ

	//----------------------------------------------
	// 引数の数をﾁｪｯｸ
	//----------------------------------------------
	int n = lua_gettop(L);
	if( n<=0 || n > 3){
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// 引数の解析＆取得
	//----------------------------------------------
	// 引数1 ﾌｧｲﾙ名称の型ﾁｪｯｸ
	if( lua_isstring(L,1) != 1 ){
		// ﾌｧｲﾙ名称文字列認識ｴﾗｰ
		iAnswer = 2;
		goto ExitBlock;
	}
	sFilename = lua_tostring(L,1);

	// 引数2 読込開始ﾊﾞｲﾄ位置の型ﾁｪｯｸ
	if( n >= 2 ){
		if( lua_isnumber(L,2) != 1 ){
			// 読込開始ﾊﾞｲﾄ位置の型ﾁｪｯｸｴﾗｰ
			iAnswer = 3;
			goto ExitBlock;
		}
		iReadPosition = (int)lua_tonumber(L,2);
		if( iReadPosition < 0 ) iReadPosition = 0;
	}
	// 引数3 読込ｻｲｽﾞの型ﾁｪｯｸ
	if( n == 3 ){
		if( lua_isnumber(L,3) != 1 ){
			// 読込ｻｲｽﾞの型ﾁｪｯｸの型ﾁｪｯｸｴﾗｰ
			iAnswer = 4;
			goto ExitBlock;
		}
		iReadSize = (int)lua_tonumber(L,3);
		if( iReadSize < 0 ) iReadSize = 0;
	}
	//----------------------------------------------
	// ﾌｧｲﾙｻｲｽﾞの取得とﾒﾓﾘ確保
	//----------------------------------------------
	if( FALSE == CFile::GetStatus( sFilename, status ) ){
		// ﾌｧｲﾙ情報取得ｴﾗｰ
		iAnswer = 5;
		goto ExitBlock;
	}
	if( status.m_size == 0 ){
		// ﾌｧｲﾙｻｲｽﾞ無し
		iAnswer = 6;
		goto ExitBlock;
	}
	if( iReadPosition > 0 ){
		if( status.m_size <= iReadPosition ){
			// 読み込み開始位置はEOF超過
			iAnswer = 7;
			goto ExitBlock;
		}
	}
	if( iReadSize > 0 ){
		if( status.m_size < iReadPosition + iReadSize ){
			// 読み込みﾃﾞｰﾀ無し
			iAnswer = 8;
			goto ExitBlock;
		}
	}
	// 読み込みｻｲｽﾞ計算
	if( iReadSize > 0 )		uiReadSize = iReadSize;
	else					uiReadSize = status.m_size - iReadPosition;

	lpBuffer = (BYTE*)::GlobalAlloc( GPTR, uiReadSize );
	if( lpBuffer == NULL ){
		// ﾌｧｲﾙ読み込みｴﾘｱ確保ｴﾗｰ
		iAnswer = 9;
		goto ExitBlock;
	}
	//----------------------------------------------
	// ﾌｧｲﾙを読み込む
	//----------------------------------------------
	try{
		CFile file;
		if( FALSE == file.Open( sFilename, CFile::modeRead ) ){
			// ﾌｧｲﾙｵｰﾌﾟﾝｴﾗｰ
			iAnswer = 10;
			goto ExitBlock;
		}
		if( iReadPosition > 0 ){
			// 指定があれば移動する
			file.Seek( iReadPosition, CFile::begin );
		}
		file.Read( lpBuffer, uiReadSize );
		file.Close();
	}
	catch( CFileException *e ){
		e->Delete();
		// ﾌｧｲﾙ読み込みｴﾗｰ
		iAnswer = 11;
		goto ExitBlock;
	}
	catch( CException *e ){
		e->Delete();
		// その他のｴﾗｰ
		iAnswer = 12;
		goto ExitBlock;
	}
	//----------------------------------------------
	// Luaﾃｰﾌﾞﾙに配置
	//----------------------------------------------
	lua_pushtableEx(L, lpBuffer, uiReadSize );

ExitBlock:
	if( lpBuffer ){
		::GlobalFree(lpBuffer);
		lpBuffer = NULL;
	}
	if( iAnswer > 0 ){
		// ｴﾗｰ発生時はﾊﾟﾗﾒｰﾀｾｯﾄ
		lua_pushnumber(L,iAnswer);
	}
	// ﾊﾟﾗﾒｰﾀ数を返す
	return 1;
}
/*
######################################################################################
--------------------------------------------------------------------------------------
--  <<スクリプト追加モジュール>>
--
--  拡張ファイル書き込み
--------------------------------------------------------------------------------------
<<概要>>
ファイル書き込みの拡張処理

result = FileWriteEx( data, sFileName, iFlg )
result = FileWriteEx( data, sFileName )

<<引数>>
data(テーブル)        書き込むバイナリデータ
data(文字列)          書き込む文字列
                      (書き込むデータはテーブル、文字列どちらでも可)
sFileName(文字列)     読み込むファイル名称
iFlg(数値)            書き込み処理フラグ(省略可)
                      0:既存ファイルがあれば、
                        削除し新たにファイルを作成する。
                      1:既存ファイルがあればデータを追記する。
                      省略時、または上記範囲外の数値の場合は
                      0が適用されます。

<<戻り値>>
数値                  0:正常終了
                      1:パラメータエラー
                      2:ファイル名称文字列認識エラー
                      3:書き込み指定認識エラー
                      4:データ型認識エラー
                      5:ファイルオープンエラー
                      6:ファイル書き込みエラー

<<例>>
(1)
result = FileWriteEx( tbl, "c:\\test.txt" )
└テーブルtblの内容を"c:\test.txt"に書き込む。
  既にファイルが存在すれば上書きする。

(2)
result = FileWriteEx( "ABCDEFG", "c:\\test.txt", 1 )
└文字列"ABCDEFG"を"c:\test.txt"に書き込む。
  既にファイルが存在すれば追記する。

--------------------------------------------------------------------------------------
######################################################################################
*/
int FileWriteEx(lua_State *L)
{
	COleSafeArray	sd;
	int				iResult;
	CString			str;
	CString			sFilename;
	int				iAnswer = 0;
	BOOL			bAdd = FALSE;
	BYTE			*lpBuffer = NULL;

	//----------------------------------------------------------
	// 初期化
	//----------------------------------------------------------
	sd.CreateOneDim( VT_UI1, 0 );
	//----------------------------------------------------------
	// ﾊﾟﾗﾒｰﾀ数ﾁｪｯｸ
	//----------------------------------------------------------
	int n = lua_gettop(L);		// 引数の数
	if( n<=1 || n > 3){
		// 引数範囲外
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------------------
	// ﾌｧｲﾙ名称取得
	//----------------------------------------------------------
	if( lua_isstring(L,2) != 1 ){
		// ﾌｧｲﾙ名称文字列認識ｴﾗｰ
		iAnswer = 2;
		goto ExitBlock;
	}
	sFilename = lua_tostring(L,2);
	//----------------------------------------------------------
	// 書き込み指定取得
	//----------------------------------------------------------
	if( n==3 ){
		if( lua_isnumber(L,3) != 1 ){
			// 書き込み指定認識ｴﾗｰ
			iAnswer = 3;
			goto ExitBlock;
		}
		if( 1 == lua_tonumber(L,3) ){
			bAdd = TRUE;
		}
	}
	//----------------------------------------------------------
	// ﾃﾞｰﾀを認識する
	//----------------------------------------------------------
	iResult = lua_istable(L,1);
	if( iResult == 1 ){
		// ﾃﾞｰﾀはﾃｰﾌﾞﾙ
		sd = lua_totableEx(L,1);
	}
	else {
		iResult = lua_isstring(L,1);
		if( iResult == 1 ){
			// ﾃﾞｰﾀは文字列
			str = lua_tostring(L,1);
			if( str.GetLength() > 0 )
				sd.CreateOneDim( VT_UI1, str.GetLength(), (const char*)CT2A(str));
		}
		else{
			// ﾃﾞｰﾀ型認識ｴﾗｰ
			iAnswer = 4;
			goto ExitBlock;
		}
	}
	//----------------------------------------------------------
	// ﾌｧｲﾙ書き込み
	//----------------------------------------------------------
	if( sd.GetOneDimSize() <= 0 ){
		// 書き込むﾃﾞｰﾀは存在しない
		goto ExitBlock;
	}
	try{
		CFile file;
		CFileStatus fStatus;
		sd.AccessData( (void**)&lpBuffer );
		UINT uiMode = CFile::modeCreate | CFile::modeWrite;
		if( bAdd && TRUE == CFile::GetStatus( sFilename, fStatus ) ){
			// ﾌｧｲﾙが存在した場合は作成ﾌﾗｸﾞを落とす
			uiMode = CFile::modeWrite;
		}
		if( file.Open( sFilename, uiMode ) ){
			if( bAdd ){
				// 追記ﾓｰﾄﾞの場合、最後までｼｰｸ
				file.SeekToEnd();
			}
			file.Write( lpBuffer, sd.GetOneDimSize() );
			file.Close();
		}
		else{
			sd.UnaccessData();
			// ﾌｧｲﾙｵｰﾌﾟﾝｴﾗｰ
			iAnswer = 5;
			goto ExitBlock;
		}
		sd.UnaccessData();
	}
	catch( CException *e ){
		e->Delete();
		if( lpBuffer!=NULL )sd.UnaccessData();
		// ﾌｧｲﾙ書き込みｴﾗｰ
		iAnswer = 6;
		goto ExitBlock;
	}

ExitBlock:
	// ﾊﾟﾗﾒｰﾀｾｯﾄ
	lua_pushnumber(L,iAnswer);
	// ﾊﾟﾗﾒｰﾀ数を返す
	return 1;
}
