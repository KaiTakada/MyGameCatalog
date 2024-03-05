//=================================================
//
// 3D��(wall.cpp)
// Author: Kai Takada
//
//=================================================
#ifndef _WALL_H_								//���̃}�N����`������ĂȂ�������
#define _WALL_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//============================
// �C���N���[�h�t�@�C��
//============================
#include "manager.h"
#include "object3D.h"
#include "game.h"

//============================
// �}�N����`
//============================
#define DEF_VERTEX3D D3DXVECTOR3(0.0f,0.0f,0.0f)				//�f�t�H���g����

//============================
// 3D�|���S���N���X
//============================
class CWall : public CObject3D
{
public:
	CWall(int nPriority = 1);
	~CWall();

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);		//������(�I�o��)
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CWall *Create(void);		//����
	static CWall *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = DEF_VERTEX3D, const D3DXVECTOR3 size = mylib_const::DEF_FIELD_SIZE);		//����

	bool CollisionChara(D3DXVECTOR3 posObj, D3DXVECTOR3 posOldObj, D3DXVECTOR3 *posCross);		//�L�����N�^�[���ǂɂ߂荞�܂Ȃ��悤�␳����

private:

};
#endif
