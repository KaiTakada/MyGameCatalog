//==========================================
//
// fade.cpp
// Author: 髙田 佳依
//
// コメント確認すべし。
//==========================================
#include "fade.h"
#include "input.h"

//===========================
// マクロ定義
//===========================
#define COL_A_ADD (1.0f / 80.0f)

//===========================
// コンストラクタ
//===========================
CFade::CFade(int nPriority) : CObject2D(nPriority)
{
	m_state = STATE_NONE;								//フェードの状態
	m_modeNext = CScene::MODE_TITLE;						//次の画面(モード)
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//ポリゴン(フェード)の色
	m_bExc = false;
}

//===========================
// デストラクタ
//===========================
CFade::~CFade()
{

}

//===========================
//フェードの初期化処理
//===========================
HRESULT CFade::Init(CScene::MODE modeNext)
{
	CObject2D::Init(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_state = STATE_IN;									//ノンフェード状態に

	m_modeNext = modeNext;									//次の画面（モード）を設定

	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);			//黒いポイゴン（不透明）にしておく
	
	SetTexCol(m_colorFade);
	
	//SetState(m_modeNext);

	return S_OK;
}

//===========================
//フェードの終了処理
//===========================
void CFade::Uninit(void)
{
	CObject2D::Uninit();
}

//===========================
//フェードの更新処理
//===========================
void CFade::Update(void)
{
	CObject2D::Update();

	if (m_state != STATE_NONE)
	{
		if (m_state == STATE_IN)
		{//フェードイン状態
			m_colorFade.a -= COL_A_ADD;					//ポリゴンを透明にしていく

			if (m_colorFade.a < 0.0f)
			{
				m_colorFade.a = 0.0f;
				m_state = STATE_NONE;					//何もしていない状態に
			}
		}
		else if (m_state == STATE_OUT)
		{//フェードアウト状態
			m_colorFade.a += COL_A_ADD;					//ポリゴンを不透明にしていく

			if (m_colorFade.a > 1.0f)
			{
				m_colorFade.a = 1.0f;
				m_state = STATE_NONE;					//フェードイン状態に

				m_bExc = false;

				//モード設定（次の画面に移行）
				CManager::SetMode(m_modeNext);

				//m_bExc = false;
				
				return;
			}
		}

		//色変え
		SetTexCol(m_colorFade);
	}
}
//===========================
//フェードの描画処理
//===========================
void CFade::Draw(void)
{
	CObject2D::Draw();
}

//===========================
// 生成処理
//===========================
CFade *CFade::Create(CScene::MODE modeNext)
{
	CFade *pFade = nullptr;

	//オブジェクト生成&初期化
	pFade = new CFade;

	if (pFade != nullptr)
	{
		if (FAILED(pFade->Init(modeNext)))
		{
			return nullptr;
		}
		else
		{
			pFade->SetIdxTexture(-1);

			pFade->SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			pFade->SetSize(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		}
	}
	else
	{
		return nullptr;
	}


	return pFade;
}

//===========================
// フェードの設定処理
//===========================
bool CFade::SetState(CScene::MODE modeNext)
{
	if (m_bExc == false)
	{
		m_bExc = true;

		m_state = STATE_OUT;									//フェードアウト状態に

		m_modeNext = modeNext;									//次の画面（モード）を設定

		m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//黒いポイゴン（透明）にしておく	
		SetTexCol(m_colorFade);
	
		return true;
	}

	return false;
}
