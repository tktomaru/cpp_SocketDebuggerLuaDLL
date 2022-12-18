// SDGluaDLL.h : SDGluaDLL DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CSDGluaDLLApp
// このクラスの実装に関しては SDGluaDLL.cpp をご覧ください
//

class CSDGluaDLLApp : public CWinApp
{
public:
	CSDGluaDLLApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
