//==============================================
//
// ゲーム用カメラの処理(camera_game.h)
// Author: Kai Takada
//
//==============================================
#ifndef _CAMERA_GAME_H_								//このマクロ定義がされてなかったら
#define _CAMERA_GAME_H_								//二重インクルード防止のマクロを定義する

//===============================
// インクルードファイル
//================================
#include "camera.h"

//===============================
// カメラクラス
//================================
class CCameraGame : public CCamera
{
public:
	CCameraGame();
	~CCameraGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
	void SetIdx(int nIdx) { m_nIdx = nIdx; }	//番号設定
	void SetViewPos();	//位置設定
	void PlayerStalk();
	void RotControll();

protected:

private:
	D3DVIEWPORT9 m_viewport;	//ビューポート
	int m_nIdx;	//番号
};

#endif
