//===============================================
//
// テクスチャ(texture.cpp)
// Author 髙田 佳依
//
//===============================================

//============================
// インクルードファイル
//============================
#include "texture.h"
#include "manager.h"

//============================
// マクロ定義
//============================

//============================
// 静的メンバ変数宣言
//============================
int CTexture::m_nNumAll = 0;				//共通テクスチャバッファ

//============================
// テクスチャパス
//============================
const char *c_apTexFilepass[] =
{
	"data\\TEXTURE\\null.jpg"
};

//==========================================
//============= 2Dアニメーション =============
//============================
// コンストラクタ
//============================
CTexture::CTexture()
{
	for (int nCntCons = 0; nCntCons < MAX_TEXTURE; nCntCons++)
	{
		//テクスチャ破棄
		if (m_apTexture[nCntCons] != nullptr)
		{
			m_apTexture[nCntCons] = nullptr;
		}
		//テクスチャ名破棄
		if (apFilename[nCntCons] != nullptr)
		{
			apFilename[nCntCons] = nullptr;
		}
	}
}

//============================
// デストラクタ
//============================
CTexture::~CTexture()
{
	for (int nCntDes = 0; nCntDes < MAX_TEXTURE; nCntDes++)
	{
		//テクスチャ破棄
		if (m_apTexture[nCntDes] != nullptr)
		{
			m_apTexture[nCntDes] = nullptr;
		}
		//テクスチャ名破棄
		if (apFilename[nCntDes] != nullptr)
		{
			apFilename[nCntDes] = nullptr;
		}
	}
}

//============================
// 共有テクスチャ取得
//============================
HRESULT CTexture::Load(void)
{
	for (int nCntFile = 0; nCntFile < sizeof(c_apTexFilepass) / sizeof(c_apTexFilepass[0]); nCntFile++)
	{
		Regist(c_apTexFilepass[nCntFile]);
	}

	return S_OK;
}

//============================
// 共有テクスチャ破棄
//============================
void CTexture::Unload(void)
{
	for (int nCntUnload = 0; nCntUnload < MAX_TEXTURE; nCntUnload++)
	{
		//テクスチャ破棄
		if (m_apTexture[nCntUnload] != nullptr)
		{
			m_apTexture[nCntUnload]->Release();
			m_apTexture[nCntUnload] = nullptr;
		}

		if (apFilename[nCntUnload] != nullptr)
		{
			apFilename[nCntUnload] = nullptr;
		}
	}
}

//============================
// テクスチャ追加読込
//============================
int CTexture::Regist(const char *pFilename)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	for (int nCntReg = 0; nCntReg < MAX_TEXTURE; nCntReg++)
	{
		if (m_apTexture[nCntReg] != nullptr)
		{//情報が入っていて
		
			if (strcmp(apFilename[nCntReg], pFilename) == 0)
			{//すでに読み込んであれば
			
				return nCntReg;
			}
		}
		else if (m_apTexture[nCntReg] == nullptr)
		{//NULLチェック

		 //テクスチャの読み込み[0]
			if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice,
				pFilename,			//テクスチャのファイル名
				&m_apTexture[nCntReg])))
			{//成功した場合
				//strcpy(apFilename[nCntReg], pFilename);
				apFilename[nCntReg] = (char*)pFilename;
				m_nNumAll++;
				return nCntReg;
			}
			else
			{
				return 0;
			}
		}
	}

	return 0;
}

//============================
// 番号に合ったテクスチャを渡す
//============================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx < 0 || nIdx > MAX_TEXTURE)
	{
		return nullptr;
	}
	else
	{
		return m_apTexture[nIdx];
	}
}
