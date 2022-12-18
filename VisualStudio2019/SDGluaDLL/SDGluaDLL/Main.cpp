#include "pch.h"
#include "Common.h"

/*
######################################################################################
--------------------------------------------------------------------------------------
--  <<SocketDebugger�����P�[�W>>
--
--  ���W���[�����̎擾
--------------------------------------------------------------------------------------
<<�T�v>>
�{���W���[�������ɁASocketDebugger�͑Ή�DLL�ł��邱�Ƃ𔻒f���܂��B
�{���W���[���� count ��0���珇���A1,2...�ƌĂяo���A
���W���[�����̂��擾���悤�Ƃ��܂��B

�߂�l0�A�y�у��W���[������Ԃ����Ƃɂ��A�Ăяo���郂�W���[������F�����܂��B
�߂�l1��Ԃ����ꍇ�́A�Ăяo���郂�W���[���͑S�Ď擾�����Ɣ��f���܂��B

�擾�ł������W���[���́ASocketDebugger���ŗ��p�\�ƂȂ�܂��B

<<����>>
lpName                ���W���[�����̂��i�[����A�h���X
                      (�����Ƀ��W���[�����̂��i�[����)
bufferCount           �o�b�t�@lpName�̗��p�\�o�C�g��
count                 �֐����̎��o���ԍ�
                      SocketDebugger��0����1,2...�ƒl��ς���
					  �߂�l��0�ȊO�ƂȂ�܂ŌJ��Ԃ��Ăяo���܂��B

<<�߂�l>>
                      0:����I��  �֐��L��(lpName�̖��̗L��)
                      1:�֐�����
                      2:�i�[�T�C�Y�s��

--------------------------------------------------------------------------------------
######################################################################################
*/
DWORD GetModuleName(char* lpName, int bufferCount, int count ){
	DWORD	dwResult = 0;
	TCHAR buf[128];

	// ���̑I��
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

	// �ޯ̧��߰
	if( strlen((const char *)buf)+1 > (DWORD)bufferCount )	return 2;
	lstrcpy((TCHAR*)lpName, buf );
	return 0;
}
/*
######################################################################################
--------------------------------------------------------------------------------------
--  <<�X�N���v�g�ǉ����W���[��>>
--
--  �g���t�@�C���ǂݍ���
--------------------------------------------------------------------------------------
<<�T�v>>
�t�@�C���ǂݍ��݂̊g������

result = FileReadEx( sFileName )
result = FileReadEx( sFileName, iStartPosition )
result = FileReadEx( sFileName, iStartPosition, iSize )

<<����>>
sFileName(������)     �ǂݍ��ރt�@�C������
iStartPosition(���l)  �ǂݍ��݊J�n�޲Ĉʒu(�ȗ���)
                      �ȗ����A���l�͐擪����ǂݍ���
iSize(���l)           �ǂݍ����޲Đ�(�ȗ���)
                      �ȗ����A���l�A0�̏ꍇ�͑S�ēǂݍ���

<<�߂�l>>
�e�[�u��              �ǂݍ��񂾃f�[�^
���l                  �ُ�I��
                      1:�p�����[�^�G���[
                      2:�t�@�C�����̕�����F���G���[
                      3:�Ǎ��J�n�o�C�g�ʒu�̌^�`�F�b�N�G���[
                      4:�Ǎ��T�C�Y�̌^�`�F�b�N�̌^�`�F�b�N�G���[
                      5:�t�@�C�����擾�G���[
                      6:�t�@�C���T�C�Y����
                      7:�ǂݍ��݊J�n�ʒu��EOF����
                      8:�w�肳�ꂽ�ǂݍ��݃o�C�g���ɖ����Ȃ�����
                      9:�t�@�C���ǂݍ��݃G���A�m�ۃG���[
                      10:�t�@�C���I�[�v���G���[
                      11:�t�@�C���ǂݍ��݃G���[
                      12:�t�@�C���ǂݍ��݂��̑��̃G���[

<<��>>
(1)
result = FileReadEx( "c:\\test.txt" )
���w�肳�ꂽ�t�@�C����S�ēǂݍ���

(2)
result = FileReadEx( "c:\\test.txt", 16 )
���w�肳�ꂽ�t�@�C����16�o�C�g�ڂ���ȍ~�S�ēǂݍ���

(3)
result = FileReadEx( "c:\\test.txt", 16, 32 )
���w�肳�ꂽ�t�@�C����16�o�C�g�ڂ���32�o�C�g���ǂݍ���

--------------------------------------------------------------------------------------
######################################################################################
*/
int FileReadEx(lua_State *L)
{
	CString		sFilename;				// ���Ұ�:̧�ٖ���
	int			iReadPosition = 0;		// �ǂݍ��݊J�n�ʒu
	int			iReadSize = 0;			// �ǂݍ��ݻ���

	int			iAnswer = 0;			// �߂�l
	BYTE		*lpBuffer = NULL;		// ̧�ٓǂݍ����ޯ̧�߲��
	UINT		uiReadSize;				// �ǂݍ��ݻ���
	CFileStatus status;					// ̧�ٽð��

	//----------------------------------------------
	// �����̐�������
	//----------------------------------------------
	int n = lua_gettop(L);
	if( n<=0 || n > 3){
		// �����͈͊O
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// �����̉�́��擾
	//----------------------------------------------
	// ����1 ̧�ٖ��̂̌^����
	if( lua_isstring(L,1) != 1 ){
		// ̧�ٖ��̕�����F���װ
		iAnswer = 2;
		goto ExitBlock;
	}
	sFilename = lua_tostring(L,1);

	// ����2 �Ǎ��J�n�޲Ĉʒu�̌^����
	if( n >= 2 ){
		if( lua_isnumber(L,2) != 1 ){
			// �Ǎ��J�n�޲Ĉʒu�̌^�����װ
			iAnswer = 3;
			goto ExitBlock;
		}
		iReadPosition = (int)lua_tonumber(L,2);
		if( iReadPosition < 0 ) iReadPosition = 0;
	}
	// ����3 �Ǎ����ނ̌^����
	if( n == 3 ){
		if( lua_isnumber(L,3) != 1 ){
			// �Ǎ����ނ̌^�����̌^�����װ
			iAnswer = 4;
			goto ExitBlock;
		}
		iReadSize = (int)lua_tonumber(L,3);
		if( iReadSize < 0 ) iReadSize = 0;
	}
	//----------------------------------------------
	// ̧�ٻ��ނ̎擾����؊m��
	//----------------------------------------------
	if( FALSE == CFile::GetStatus( sFilename, status ) ){
		// ̧�ُ��擾�װ
		iAnswer = 5;
		goto ExitBlock;
	}
	if( status.m_size == 0 ){
		// ̧�ٻ��ޖ���
		iAnswer = 6;
		goto ExitBlock;
	}
	if( iReadPosition > 0 ){
		if( status.m_size <= iReadPosition ){
			// �ǂݍ��݊J�n�ʒu��EOF����
			iAnswer = 7;
			goto ExitBlock;
		}
	}
	if( iReadSize > 0 ){
		if( status.m_size < iReadPosition + iReadSize ){
			// �ǂݍ����ް�����
			iAnswer = 8;
			goto ExitBlock;
		}
	}
	// �ǂݍ��ݻ��ތv�Z
	if( iReadSize > 0 )		uiReadSize = iReadSize;
	else					uiReadSize = status.m_size - iReadPosition;

	lpBuffer = (BYTE*)::GlobalAlloc( GPTR, uiReadSize );
	if( lpBuffer == NULL ){
		// ̧�ٓǂݍ��ݴر�m�۴װ
		iAnswer = 9;
		goto ExitBlock;
	}
	//----------------------------------------------
	// ̧�ق�ǂݍ���
	//----------------------------------------------
	try{
		CFile file;
		if( FALSE == file.Open( sFilename, CFile::modeRead ) ){
			// ̧�ٵ���ݴװ
			iAnswer = 10;
			goto ExitBlock;
		}
		if( iReadPosition > 0 ){
			// �w�肪����Έړ�����
			file.Seek( iReadPosition, CFile::begin );
		}
		file.Read( lpBuffer, uiReadSize );
		file.Close();
	}
	catch( CFileException *e ){
		e->Delete();
		// ̧�ٓǂݍ��ݴװ
		iAnswer = 11;
		goto ExitBlock;
	}
	catch( CException *e ){
		e->Delete();
		// ���̑��̴װ
		iAnswer = 12;
		goto ExitBlock;
	}
	//----------------------------------------------
	// Luað��قɔz�u
	//----------------------------------------------
	lua_pushtableEx(L, lpBuffer, uiReadSize );

ExitBlock:
	if( lpBuffer ){
		::GlobalFree(lpBuffer);
		lpBuffer = NULL;
	}
	if( iAnswer > 0 ){
		// �װ�����������Ұ����
		lua_pushnumber(L,iAnswer);
	}
	// ���Ұ�����Ԃ�
	return 1;
}
/*
######################################################################################
--------------------------------------------------------------------------------------
--  <<�X�N���v�g�ǉ����W���[��>>
--
--  �g���t�@�C����������
--------------------------------------------------------------------------------------
<<�T�v>>
�t�@�C���������݂̊g������

result = FileWriteEx( data, sFileName, iFlg )
result = FileWriteEx( data, sFileName )

<<����>>
data(�e�[�u��)        �������ރo�C�i���f�[�^
data(������)          �������ޕ�����
                      (�������ރf�[�^�̓e�[�u���A������ǂ���ł���)
sFileName(������)     �ǂݍ��ރt�@�C������
iFlg(���l)            �������ݏ����t���O(�ȗ���)
                      0:�����t�@�C��������΁A
                        �폜���V���Ƀt�@�C�����쐬����B
                      1:�����t�@�C��������΃f�[�^��ǋL����B
                      �ȗ����A�܂��͏�L�͈͊O�̐��l�̏ꍇ��
                      0���K�p����܂��B

<<�߂�l>>
���l                  0:����I��
                      1:�p�����[�^�G���[
                      2:�t�@�C�����̕�����F���G���[
                      3:�������ݎw��F���G���[
                      4:�f�[�^�^�F���G���[
                      5:�t�@�C���I�[�v���G���[
                      6:�t�@�C���������݃G���[

<<��>>
(1)
result = FileWriteEx( tbl, "c:\\test.txt" )
���e�[�u��tbl�̓��e��"c:\test.txt"�ɏ������ށB
  ���Ƀt�@�C�������݂���Ώ㏑������B

(2)
result = FileWriteEx( "ABCDEFG", "c:\\test.txt", 1 )
��������"ABCDEFG"��"c:\test.txt"�ɏ������ށB
  ���Ƀt�@�C�������݂���ΒǋL����B

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
	// ������
	//----------------------------------------------------------
	sd.CreateOneDim( VT_UI1, 0 );
	//----------------------------------------------------------
	// ���Ұ�������
	//----------------------------------------------------------
	int n = lua_gettop(L);		// �����̐�
	if( n<=1 || n > 3){
		// �����͈͊O
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------------------
	// ̧�ٖ��̎擾
	//----------------------------------------------------------
	if( lua_isstring(L,2) != 1 ){
		// ̧�ٖ��̕�����F���װ
		iAnswer = 2;
		goto ExitBlock;
	}
	sFilename = lua_tostring(L,2);
	//----------------------------------------------------------
	// �������ݎw��擾
	//----------------------------------------------------------
	if( n==3 ){
		if( lua_isnumber(L,3) != 1 ){
			// �������ݎw��F���װ
			iAnswer = 3;
			goto ExitBlock;
		}
		if( 1 == lua_tonumber(L,3) ){
			bAdd = TRUE;
		}
	}
	//----------------------------------------------------------
	// �ް���F������
	//----------------------------------------------------------
	iResult = lua_istable(L,1);
	if( iResult == 1 ){
		// �ް���ð���
		sd = lua_totableEx(L,1);
	}
	else {
		iResult = lua_isstring(L,1);
		if( iResult == 1 ){
			// �ް��͕�����
			str = lua_tostring(L,1);
			if( str.GetLength() > 0 )
				sd.CreateOneDim( VT_UI1, str.GetLength(), (const char*)CT2A(str));
		}
		else{
			// �ް��^�F���װ
			iAnswer = 4;
			goto ExitBlock;
		}
	}
	//----------------------------------------------------------
	// ̧�ُ�������
	//----------------------------------------------------------
	if( sd.GetOneDimSize() <= 0 ){
		// ���������ް��͑��݂��Ȃ�
		goto ExitBlock;
	}
	try{
		CFile file;
		CFileStatus fStatus;
		sd.AccessData( (void**)&lpBuffer );
		UINT uiMode = CFile::modeCreate | CFile::modeWrite;
		if( bAdd && TRUE == CFile::GetStatus( sFilename, fStatus ) ){
			// ̧�ق����݂����ꍇ�͍쐬�׸ނ𗎂Ƃ�
			uiMode = CFile::modeWrite;
		}
		if( file.Open( sFilename, uiMode ) ){
			if( bAdd ){
				// �ǋLӰ�ނ̏ꍇ�A�Ō�܂ż��
				file.SeekToEnd();
			}
			file.Write( lpBuffer, sd.GetOneDimSize() );
			file.Close();
		}
		else{
			sd.UnaccessData();
			// ̧�ٵ���ݴװ
			iAnswer = 5;
			goto ExitBlock;
		}
		sd.UnaccessData();
	}
	catch( CException *e ){
		e->Delete();
		if( lpBuffer!=NULL )sd.UnaccessData();
		// ̧�ُ������ݴװ
		iAnswer = 6;
		goto ExitBlock;
	}

ExitBlock:
	// ���Ұ����
	lua_pushnumber(L,iAnswer);
	// ���Ұ�����Ԃ�
	return 1;
}
