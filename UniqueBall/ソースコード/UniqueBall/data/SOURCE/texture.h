//===============================================
//
// テクスチャ(texture.h)
// Author: Kai Takada
//
//===============================================
#ifndef _TEXTURE_H_		//このマクロ定義がされてなかったら
#define _TEXTURE_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "renderer.h"

//=========================
// マクロ定義
//=========================

//=========================
// 前方宣言
//=========================

//=========================
// プレイヤークラス
//=========================
class CTexture
{
public:
	CTexture();		//コンストラクタ
	~CTexture();	//デストラクタ

	HRESULT Load(void);								// テクスチャ読み込み
	void Unload(void);								// テクスチャ破棄
	int Regist(const char *pFilename);				// テクスチャ追加読込
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);		// テクスチャ情報取得
	int GetAddress() { return m_nNumAll; }			// 総数取得

protected:

private:
	std::list<LPDIRECT3DTEXTURE9> m_apTexture;				//共通テクスチャバッファ
	std::list<std::string> m_apFilename;		//ファイル文字列
	static int m_nNumAll;	//総テクスチャ数
};

#endif
