//==============================================
//
// �J�����̏���(camera.h)
// Author: Kai Takada
//
//==============================================
#ifndef _CAMERA_H_								//���̃}�N����`������ĂȂ�������
#define _CAMERA_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//===============================
// �C���N���[�h�t�@�C��
//================================

//===============================
// �J�����N���X
//================================
class CCamera
{
public:
	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void ReviseRot(void);		//��荞��
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void PlayerStalk(void);
	void EditStalk(void);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		//�p�x

protected:
	D3DXMATRIX GetMtxProj() { return m_mtxProjection; }				//�v���W�F�N�V�����}�g���b�N�X
	void SetMtxProj(D3DXMATRIX mtx) { m_mtxProjection = mtx; }		//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX GetMtxView() { return m_mtxView; }				//�r���[�}�g���b�N�X
	void SetMtxView(D3DXMATRIX mtx) { m_mtxView = mtx; }		//�v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 GetPosV() { return m_posV; }			//���_
	void SetPosV(D3DXVECTOR3 pos) { m_posV = pos; }		//���_
	D3DXVECTOR3 GetPosR() { return m_posR; }			//�����_
	void SetPosR(D3DXVECTOR3 pos) { m_posR = pos; }		//�����_
	D3DXVECTOR3 GetPosVDest() { return m_posVDest; }			//���_
	void SetPosVDest(D3DXVECTOR3 pos) { m_posVDest = pos; }		//���_
	D3DXVECTOR3 GetPosRDest() { return m_posRDest; }			//�����_
	void SetPosRDest(D3DXVECTOR3 pos) { m_posRDest = pos; }		//�����_
	D3DXVECTOR3 GetVecU() { return m_vecU; }			//������x�N�g��
	void SetVecU(D3DXVECTOR3 vec) { m_vecU = vec; }		//������x�N�g��
	D3DXVECTOR3 GetRotDest() { return m_rotDest; }						//�ڕW����
	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }		//�ڕW����
	float GetAngle() { return m_fAngle; }					//�p�x
	void SetAngle(float fAngle) { m_fAngle = fAngle; }		//�p�x
	float GetDis() { return m_fDis; }				//����
	void SetDis(float fDis) { m_fDis = fDis; }		//����

	void RotControll();		//�}�E�X�Ō����𒲐�

private:
	void RotHorPosR();		//�����_��������
	void RotVerPosR();		//�����_��������
	
	D3DXMATRIX m_mtxProjection;		//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			//�r���[�}�g���b�N�X
	D3DXVECTOR3 m_posV;				//���_
	D3DXVECTOR3 m_posR;				//�����_
	D3DXVECTOR3 m_posVDest;			//�\�莋�_
	D3DXVECTOR3 m_posRDest;			//�\�蒍���_
	float m_posRAdjt;				//�����_���炵
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_vecU;				//������x�N�g��
	D3DXVECTOR3 m_rot;				//����
	D3DXVECTOR3 m_rotDest;			//�\�����
	float m_fAngle;					//�p�x
	float m_fDis;					//����
};

#endif
