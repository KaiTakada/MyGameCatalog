//=================================================
//
// �r���{�[�h(billboard.cpp)
// Author: Kai Takada
//
//=================================================
#include "manager.h"
#include "objectBillboard.h"
#include "debugproc.h"
#include "input.h"
#include "texture.h"
#include "Anim.h"

//==============================
// �}�N����`
//==============================
#define BILLBOARD_SIZE (100.0f)
#define TEX_PASS "data\\TEXTURE\\chara.png"

//==============================
// �ÓI�����o�ϐ��錾
//==============================

//==============================
// �R���X�g���N�^
//==============================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject3D(nPriority)
{
	m_pTexture = nullptr;							//�e�N�X�`���ւ̃|�C���^
	m_move = mylib_const::DEFVEC3;			//�ړ�
}

//==============================
// �f�X�g���N�^
//==============================
CObjectBillboard::~CObjectBillboard()
{

}

//==============================
// �r���{�[�h�̏���������
//==============================
HRESULT CObjectBillboard::Init(void)
{
	CObject3D::Init();

	SetSize(D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE, BILLBOARD_SIZE));			//�傫��
	SetNormal(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	SetAtest(true);

	SetType(TYPE_BILLBOARD);

	return S_OK;
}

//==============================
// �r���{�[�h�̏���������
//==============================
HRESULT CObjectBillboard::Init(const D3DXVECTOR3 pos)
{
	CObject3D::Init();

	SetPos(pos);			//�傫��
	SetSize(D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE, BILLBOARD_SIZE));			//�傫��
	SetNormal(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	SetAtest(true);

	SetType(TYPE_BILLBOARD);

	return S_OK;
}

//==============================
// �r���{�[�h�̏I������
//==============================
void CObjectBillboard::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;					//�e�N�X�`���j��
	}

	CObject3D::Uninit();
}

//==============================
// �r���{�[�h�̍X�V����
//==============================
void CObjectBillboard::Update(void)
{
	CObject3D::Update();
}

//==============================
// �r���{�[�h�̕`�揈��
//==============================
void CObjectBillboard::Draw(void)
{
	//�e�N�X�`���擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXVECTOR3 pos = GetPos();
	LPDIRECT3DVERTEXBUFFER9 VtxBuff = GetVtxBuff();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;									//�r���[�}�g���b�N�X�擾�p

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&mtxWorld, NULL, &mtxView);

	//mtxWorld._21 = 0.0f;		//Y����]�̑ł�����
	//mtxWorld._22 = 1.0f;		//rot��xyz?
	//mtxWorld._23 = 0.0f;

	mtxWorld._41 = 0.0f;		//�S�������t��
	mtxWorld._42 = 0.0f;
	mtxWorld._43 = 0.0f;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	SetMtxWorld(mtxWorld);

	//---------------------------------------------------
	//�����_���[
	DrawRenderSet(pDevice);
	//---------------------------------------------------

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(GetIdxTexture()));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,												//�`�悷��ŏ��̃C���f�b�N�X
		2);												//�`�悷��v���~�e�B�u��

	//---------------------------------------------------
	//�����_���[
	DrawRenderReset(pDevice);
	//---------------------------------------------------
}

//============================
// �r���{�[�h�̐�������
//============================
CObjectBillboard * CObjectBillboard::Create(void)
{
	CObjectBillboard *pObject3D;

	//�I�u�W�F�N�g����
	pObject3D = new CObjectBillboard;

	//������
	pObject3D->Init();

	return pObject3D;
}

//============================
// �r���{�[�h�̐�������
//============================
CObjectBillboard * CObjectBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, bool bZtest)
{
	CObjectBillboard *pBillBoard = nullptr;

	//�I�u�W�F�N�g����&������
	pBillBoard = new CObjectBillboard;

	if (pBillBoard != nullptr)
	{
		pBillBoard->Init(pos);

		pBillBoard->SetZtest(bZtest);

		pBillBoard->SetSize(size);

		pBillBoard->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pBillBoard;
}

//====================================
// �ʒu�̐ݒ�
//====================================
void CObjectBillboard::SetSize(const D3DXVECTOR3 size)
{
	D3DXVECTOR3 Mysize = GetSize();
	Mysize = size;
	Mysize.z = 0.0f;

	SetSizeValue(size);

	VERTEX_3D* pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, 0.0f);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//==========================================
//============= �A�j���[�V���� =============
//============================
// �R���X�g���N�^
//============================
CObjectBillboardAnim::CObjectBillboardAnim(int nPriority) : CObjectBillboard(nPriority)
{
	m_anim = nullptr;
}

//============================
// �f�X�g���N�^
//============================
CObjectBillboardAnim::~CObjectBillboardAnim()
{

}

//====================================
// ����������(�f�t�H)
//====================================
HRESULT CObjectBillboardAnim::Init(void)
{
	//������
	CObjectBillboard::Init();

	m_anim = new CAnim;

	if (m_anim != nullptr)
	{
		m_anim->Init();
	}

	CObjectBillboard::SetTexPos(D3DXVECTOR2(1.0f / m_anim->GetNumPtnX(), 1.0f / m_anim->GetNumPtnY()), D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//====================================
// ������(�ʒu�I�o��)
//====================================
HRESULT CObjectBillboardAnim::Init(const D3DXVECTOR3 pos)
{
	if(FAILED(CObjectBillboard::Init(pos)))
	{
		return E_FAIL;
	}

	m_anim = new CAnim;

	if (m_anim != nullptr)
	{
		m_anim->Init();
	}

	CObjectBillboard::SetTexPos(D3DXVECTOR2(1.0f / m_anim->GetNumPtnX(), 1.0f / m_anim->GetNumPtnY()), D3DXVECTOR2(0.0f, 0.0f));

	return S_OK;
}

//============================
// �I������
//============================
void CObjectBillboardAnim::Uninit(void)
{
	if (m_anim != nullptr)
	{
		m_anim->Uninit();
		delete m_anim;
		m_anim = nullptr;
	}

	//�I��
	CObjectBillboard::Uninit();
}

//============================
// �X�V����
//============================
void CObjectBillboardAnim::Update(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CObjectBillboard::GetVtxBuff();
	VERTEX_3D *pVtx;

	CObjectBillboard::Update();

	//�A�j���[�V�����X�V
	m_anim->Update();
	D3DXVECTOR2 *pTexpos = m_anim->GetTexpos();

	//�e�N�X�`���̍X�V
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	for (int nCntTex = 0; nCntTex < MAX_VTX; nCntTex++)
	{
		pVtx[nCntTex].tex = pTexpos[nCntTex];		//(x,y)
	}

	//���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	if (m_anim->IsEnd())
	{
		SetDeath(true);
	}
}

//============================
// �`�揈��
//============================
void CObjectBillboardAnim::Draw(void)
{
	//�`��
	CObjectBillboard::Draw();
}

//============================
// �A�j��BB�̐�������
//============================
CObjectBillboardAnim * CObjectBillboardAnim::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CObjectBillboardAnim *pBillBoardAnim = nullptr;

	//�I�u�W�F�N�g����&������
	pBillBoardAnim = new CObjectBillboardAnim;

	if (pBillBoardAnim != nullptr)
	{
		pBillBoardAnim->Init(pos);

		pBillBoardAnim->SetSize(size);

		pBillBoardAnim->SetIdxTexture(0);
	}
	else
	{
		return nullptr;
	}

	return pBillBoardAnim;
}
