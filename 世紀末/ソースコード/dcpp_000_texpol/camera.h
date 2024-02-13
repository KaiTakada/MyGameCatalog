//==============================================
//
//カメラの処理(camera.h)
// Author: Kai Takada
//
//==============================================
#ifndef _CAMERA_H_								//このマクロ定義がされてなかったら
#define _CAMERA_H_								//二重インクルード防止のマクロを定義する

//===============================
// インクルードファイル
//================================

//===============================
// カメラクラス
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
	void CameraStalk(void);		//回り込み
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void PlayerStalk(void);

protected:

private:

	D3DXMATRIX m_mtxProjection;		//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			//ビューマトリックス
	D3DXVECTOR3 m_posV;				//視点
	D3DXVECTOR3 m_posR;				//注視点
	D3DXVECTOR3 m_posVDest;			//予定視点
	D3DXVECTOR3 m_posRDest;			//予定注視点
	float m_posRAdjt;				//注視点ずらし
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_vecU;				//上方向ベクトル
	D3DXVECTOR3 m_rot;				//向き
	D3DXVECTOR3 m_rotDest;			//予定向き
	float m_fAngle;					//角度
	float m_fDis;					//距離
};

#endif
