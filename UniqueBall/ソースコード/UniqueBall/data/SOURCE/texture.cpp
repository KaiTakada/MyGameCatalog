//===============================================
//
// テクスチャ(texture.cpp)
// Author: Kai Takada
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
// 定数
//============================
namespace
{
	const char* c_apTexFilepass[] = 	//デフォルトテクスチャパス
	{
		"data\\TEXTURE\\null.jpg" 
	};
}

//============================
// コンストラクタ
//============================
CTexture::CTexture()
{
	m_apTexture.clear();
}

//============================
// デストラクタ
//============================
CTexture::~CTexture()
{
	m_apTexture.clear();
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
	m_apTexture.clear();
}

//============================
// テクスチャ追加読込
//============================
int CTexture::Regist(const char *pFilename)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	LPDIRECT3DTEXTURE9 pObj = nullptr;

	// string型に変換
	std::string strFilename(pFilename);

	int i = 0;

	// リストループ
	for (std::list<std::string>::const_iterator it = m_apFilename.cbegin(); it != m_apFilename.cend(); it++)
	{
		std::string filename = *it;

		if (strcmp(filename.c_str(), strFilename.c_str()) == 0)
		{//すでに読み込んであれば

			return i;
		}

		i++;
	}

	//テクスチャの読み込み[0]
	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice,
		strFilename.c_str(),			//テクスチャのファイル名
		&pObj)))
	{//成功した場合
		m_apFilename.push_back(strFilename);
		m_apTexture.push_back(pObj);
		m_nNumAll++;
		return i;
	}
	else
	{
		return 0;
	}

	return 0;
}

//============================
// 番号に合ったテクスチャを渡す
//============================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx < 0 || nIdx > m_apTexture.size())
	{
		return nullptr;
	}
	return *std::next(m_apTexture.begin(), nIdx);
}
