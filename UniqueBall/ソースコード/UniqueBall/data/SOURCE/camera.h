//==============================================
//
// カメラの処理(camera.h)
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
	void ReviseRot(void);		//回り込み
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void PlayerStalk(void);
	void EditStalk(void);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		//角度

protected:
	D3DXMATRIX GetMtxProj() { return m_mtxProjection; }				//プロジェクションマトリックス
	void SetMtxProj(D3DXMATRIX mtx) { m_mtxProjection = mtx; }		//プロジェクションマトリックス
	D3DXMATRIX GetMtxView() { return m_mtxView; }				//ビューマトリックス
	void SetMtxView(D3DXMATRIX mtx) { m_mtxView = mtx; }		//プロジェクションマトリックス
	D3DXVECTOR3 GetPosV() { return m_posV; }			//視点
	void SetPosV(D3DXVECTOR3 pos) { m_posV = pos; }		//視点
	D3DXVECTOR3 GetPosR() { return m_posR; }			//注視点
	void SetPosR(D3DXVECTOR3 pos) { m_posR = pos; }		//注視点
	D3DXVECTOR3 GetPosVDest() { return m_posVDest; }			//視点
	void SetPosVDest(D3DXVECTOR3 pos) { m_posVDest = pos; }		//視点
	D3DXVECTOR3 GetPosRDest() { return m_posRDest; }			//注視点
	void SetPosRDest(D3DXVECTOR3 pos) { m_posRDest = pos; }		//注視点
	D3DXVECTOR3 GetVecU() { return m_vecU; }			//上方向ベクトル
	void SetVecU(D3DXVECTOR3 vec) { m_vecU = vec; }		//上方向ベクトル
	D3DXVECTOR3 GetRotDest() { return m_rotDest; }						//目標向き
	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }		//目標向き
	float GetAngle() { return m_fAngle; }					//角度
	void SetAngle(float fAngle) { m_fAngle = fAngle; }		//角度
	float GetDis() { return m_fDis; }				//距離
	void SetDis(float fDis) { m_fDis = fDis; }		//距離

	void RotControll();		//マウスで向きを調整

private:
	void RotHorPosR();		//注視点水平旋回
	void RotVerPosR();		//注視点垂直旋回
	
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
