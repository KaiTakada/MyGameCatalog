//===============================================
//
// �萔��`(constant.h)
// Author: Kai Takada
//
//===============================================
#ifndef _CONSTANT_H_		//���̃}�N����`������ĂȂ�������
#define _CONSTANT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//=================================
// �萔��`
//=================================
namespace mylib_const
{
	const int MAX_PLAYER = 4;	//max�v���C���[�l��
	const D3DXVECTOR3 DEFVEC3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�f�t�H���gvec3
	const D3DXVECTOR2 DEFVEC2 = D3DXVECTOR2(0.0f, 0.0f);	//�f�t�H���gvec3
	const D3DXVECTOR3 MID2D = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);	//2D�̐^��
	const D3DXVECTOR3 DEF_FIELD_SIZE = D3DXVECTOR3(2048.0f, 0.0f, 2048.0f);		//�}�b�v�f�t�H���g�T�C�Y
	const D3DXVECTOR2 DEF_FIELD_SIZE2 = D3DXVECTOR2(2048.0f, 2048.0f);		//�}�b�v�f�t�H���g�T�C�Y

	const D3DXCOLOR PERSONAL_COLOR[] =
	{// �e�v���C���[�F
		D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
		D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
		D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),
	};
}

#endif
