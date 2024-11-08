//===============================================
//
// チュートリアル(tutorial.h)
// Author: 髙田 佳依
//
//===============================================
#ifndef _TUTORIAL_H_		//このマクロ定義がされてなかったら
#define _TUTORIAL_H_		//二重インクルード防止のマクロを定義する

//=========================
// インクルードファイル
//=========================
#include "main.h"
#include "scene.h"

//=========================
// 前方宣言
//=========================
//class CRenderer;

//=========================
// 画面(シーン)クラス
//=========================
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();

	virtual HRESULT Init();		//初期化
	virtual void Uninit();		//終了
	virtual void Update();		//更新
	virtual void Draw();		//描画
	static CTutorial *Create();		//生成

protected:

private:

};

#endif