//===============================================
//
// �v���R���p�C��(precompile.h)
// Author: ���c ����
//
//===============================================
#ifndef _PRECOMPILE_H_		//���̃}�N����`������ĂȂ�������
#define _PRECOMPILE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include <windows.h>
#include <string>
#include <stdio.h>
#include <time.h>
#include <string.h>

//input
#include <Xinput.h>
#define DIRECTINPUT_VERSION (0x0800)			//�r���h���̌x���Ώ��}�N���p
#include <dinput.h>

#include <assert.h>

//.���������[�N�o�͗p�}�N��
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		//���������[�N���o�ɕK�v

//input���C�u�����̃����N
#pragma comment(lib,"xinput.lib")				//Gamepad���͏����ɕK�v
#pragma comment(lib,"dinput8.lib")				//���͏����ɕK�v

//����
#include "useful.h"		//����֐��Q

//���O��Ԏw��(�ȗ�)
//using namespace std;	//���O��Ԃ���
//using std::begin;		//���O��ԓ��̊֐��̂�

////=================================
//// �}�N����`
////=================================
//#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#else
//#define DBG_NEW new
//#endif

#endif
