//===============================================
//
// レンダラー(renderer.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _RENDERER_H_		//このマクロ定義がされてなかったら
#define _RENDERER_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"

//=========================
// レンダラークラス
//=========================
class CRenderer
{
public:
	CRenderer();		//コンストラクタ
	~CRenderer();		//デストラクタ

	HRESULT Init(HWND hWnd, BOOL bWindow);		//初期化
	void Uninit(void);							//終了
	void Update(void);							//更新
	void Draw(void);							//描画
	LPDIRECT3DDEVICE9 GetDevice(void);			//デバイス取得
	int GetCamCtr() { return m_nCamCtr; }		//現在描画処理中カメラ番号

protected:

private:
	LPDIRECT3D9 m_pD3D;						//Dierct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Dierct3Dデバイスへのポインタ
	int m_nCamCtr;		//カメラ描画現在
};

#endif