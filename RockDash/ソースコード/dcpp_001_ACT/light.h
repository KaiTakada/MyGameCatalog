//==============================================
//
//ライト(light.h)
// Author: Kai Takada
//
//==============================================
#ifndef _LIGHT_H_								//このマクロ定義がされてなかったら
#define _LIGHT_H_								//二重インクルード防止のマクロを定義する

//============================
// マクロ定義
//============================
#define MAX_LIGHT (3)

//============================
// ライトクラス
//============================
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_light[MAX_LIGHT];		//ライトの情報
};

#endif
