#include "pch.h"
#include "Common.h"
#include <string>

int getSubStr(lua_State* L)
//bool getFileNames(std::string folderPath, std::vector<std::string>& file_names)
{

	std::string		strinput;				// ���Ұ�:�؂�o����������
	std::string		stroutput;				// ���Ұ�:�؂�o������
	int             istart, iend;
	int			iAnswer = 0;			// �߂�l
	std::vector<std::string> file_names;
	int folderExist = 0;

	//----------------------------------------------
	// �����̐�������
	//----------------------------------------------
	int n = lua_gettop(L);
	if (n <= 0 || n > 3) {
		// �����͈͊O
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// �����̉�́��擾
	//----------------------------------------------
	// ����1 ̧�ٖ��̂̌^����

	if (lua_isstring(L, 1) != 1) {
		// �؂�o����������F���װ
		iAnswer = 2;
		goto ExitBlock;
	}

	strinput = lua_tostring(L, 1);
	if (lua_isnumber(L, 2) != 1) {
		// �X�^�[�g�ʒu�̌^�����̌^�����װ
		iAnswer = 4;
		goto ExitBlock;
	}
	istart = (int)lua_tonumber(L, 2);
	if (lua_isnumber(L, 3) != 1) {
		// �؂�o���T�C�Y�̌^�����̌^�����װ
		iAnswer = 4;
		goto ExitBlock;
	}
	iend = (int)lua_tonumber(L, 3);

	if (strinput.size() < istart + iend) {
		// �؂�o���T�C�Y�̃G���[
		iAnswer = 5;
		goto ExitBlock;
	}
	stroutput = strinput.substr(istart, iend);
	lua_pushString(L, stroutput);

ExitBlock:
	if (iAnswer > 0) {
		// �װ�����������Ұ����
		lua_pushnumber(L, iAnswer);
		// ���Ұ�����Ԃ�
		return 1;
	}
	// ���Ұ�����Ԃ�
	return 1;
}
