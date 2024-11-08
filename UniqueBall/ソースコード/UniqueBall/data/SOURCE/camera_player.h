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
// 前方宣言
//===============================
class CObjectCharacter;

//===============================
// カメラクラス
//================================
class CCameraPlayer : public CCamera
{
public:
	CCameraPlayer();
	~CCameraPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera();
	void SetIdx(int nIdx) { m_nIdx = nIdx; }	//番号設定
	void SetViewPos();	//位置設定
	void PlayerStalk();
	void RotControll();
	D3DVIEWPORT9 GetViewPort() { return m_viewport; }	//ビューポート取得
	void FocusOnTarget();
	void SetTarget(CObjectCharacter *pChara) { m_pTarget = pChara; }

private:
	void Debug();
		
	D3DVIEWPORT9 m_viewport;	//ビューポート
	int m_nIdx;	//番号
	CObjectCharacter* m_pTarget;
};

#endif
