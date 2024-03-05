//===============================================
//
// 入力処理(input.h)
// Author 髙田 佳依
//
//===============================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"


//マクロ定義
#define NUM_KEY_MAX (256)		//キーの最大数
#define NUM_PAD_MAX (4)			//PAD使用数

//入力クラス(親)
class CInput
{
public:
	CInput();		//コンストラクタ
	~CInput();		//デストラクタ
	
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//初期化()
	virtual void Uninit(void);			//終了(仮装)
	virtual void Update(void) = 0;		//更新(純粋仮装)

protected:
	static LPDIRECTINPUT8 m_pInput;		//DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;		//入力デバイスへのポインタ

private:
	//CInputKeyboard *pKeyboard;
	//CInputGamepad *pGamepad;
	//CInputMouse *pMouse;
};

//キーボードクラス(子)
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();		//コンストラクタ
	~CInputKeyboard();		//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//初期化()
	void Uninit(void);			//終了(仮装)
	void Update(void);			//更新(純粋仮装)

	bool GetPress(int nKey) { return (m_aKeyState[nKey] & 0x80) ? true : false; }		//プレス情報取得
	bool GetTrigger(int nKey) { return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false; }		//トリガー情報取得
	bool GetRelease(int nKey) { return (m_aKeyStateRelease[nKey] & 0x80) ? true : false; }		//リリース情報取得

	//void SetPress(int nKey) { m_aKeyState[nKey] = 128; }		//プレス情報取得
	//void SetTrigger(int nKey) { m_aKeyStateTrigger[nKey] = 128; }		//トリガー情報取得
	//void SetRelease(int nKey) { m_aKeyStateRelease[nKey] = 128; }		//リリース情報取得

protected:
	BYTE m_aKeyState[NUM_KEY_MAX];					//キーボードのプレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];			//キーボードのトリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];			//キーボードのリリース情報

private:

};

//Padクラス(子)
class CInputGamepad : public CInput
{
public:
	//ボタンの列挙型
	typedef enum
	{
		BUTTON_UP = 0,			//上ボタン
		BUTTON_DOWN,			//下ボタン
		BUTTON_LEFT,			//左ボタン
		BUTTON_RIGHT,			//右ボタン
		BUTTON_START,			//スタートボタン
		BUTTON_BACK,			//バックボタン
		BUTTON_6,				//Lスティック押し込み
		BUTTON_7,				//Rスティック押し込み
		BUTTON_LB,				//LBボタン
		BUTTON_RB,				//RBボタン
		BUTTON_LT,				//スティックトリガーL？
		BUTTON_RT,				//スティックトリガーR？
		BUTTON_A,				//Aボタン
		BUTTON_B,				//Bボタン
		BUTTON_Y,				//Yボタン
		BUTTON_X,				//Xボタン
		BUTTON_LSTICKX,			//LスティックX
		BUTTON_LSTICKY,			//LスティックY
		BUTTON_MAX
	}PADBUTTON;

	CInputGamepad();		//コンストラクタ
	~CInputGamepad();		//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//初期化()
	void Uninit(void);			//終了()
	void Update(void);			//更新()

	//ボタン状態取得
	bool CInputGamepad::GetPress(PADBUTTON button, int nPlayer) { return (m_aButtonState[nPlayer].Gamepad.wButtons & (0x01 << button)) ? true : false; }
	bool CInputGamepad::GetTrigger(PADBUTTON button, int nPlayer) { return (m_aButtonStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << button)) ? true : false; }
	bool CInputGamepad::GetRelease(PADBUTTON button, int nPlayer) { return (m_aButtonStateRelease[nPlayer].Gamepad.wButtons & (0x01 << button)) ? true : false; }

	bool CInputGamepad::GetPressOR(PADBUTTON button);
	bool CInputGamepad::GetTriggerOR(PADBUTTON button);
	bool CInputGamepad::GetReleaseOR(PADBUTTON button);


	//R/Lトリガー(取れる)
	BYTE CInputGamepad::GetPressByteRT(int nPlayer) { return m_aButtonState[nPlayer].Gamepad.bRightTrigger; }	//int型は何番のコントローラーかを指す
	BYTE CInputGamepad::GetTriggerByteRT(int nPlayer) { return m_aButtonStateTrigger[nPlayer].Gamepad.bRightTrigger; }
	BYTE CInputGamepad::GetReleaseByteRT(int nPlayer) { return m_aButtonStateRelease[nPlayer].Gamepad.bRightTrigger; }
	BYTE CInputGamepad::GetPressByteLT(int nPlayer) { return m_aButtonState[nPlayer].Gamepad.bLeftTrigger; }	//int型は何番のコントローラーかを指す
	BYTE CInputGamepad::GetTriggerByteLT(int nPlayer) { return m_aButtonStateTrigger[nPlayer].Gamepad.bLeftTrigger; }
	BYTE CInputGamepad::GetReleaseByteLT(int nPlayer) { return m_aButtonStateRelease[nPlayer].Gamepad.bLeftTrigger; }

	//左スティック
	SHORT GetGameStickLXPress(int nPlayer) { return m_aButtonState[nPlayer].Gamepad.sThumbLX; }
	SHORT GetGameStickLYPress(int nPlayer) { return m_aButtonState[nPlayer].Gamepad.sThumbLY; }
	SHORT GetGameStickLXTrigger(int nPlayer) { return m_aButtonStateTrigger[nPlayer].Gamepad.sThumbLX; }
	SHORT GetGameStickLYTrigger(int nPlayer) { return m_aButtonStateTrigger[nPlayer].Gamepad.sThumbLY; }
	SHORT GetGameStickLXRelease(int nPlayer) { return m_aButtonStateRelease[nPlayer].Gamepad.sThumbLX; }
	SHORT GetGameStickLYRelease(int nPlayer) { return m_aButtonStateRelease[nPlayer].Gamepad.sThumbLY; }
	float GetStickLRot(int nPlayer) { return atan2f(float(m_aButtonState[nPlayer].Gamepad.sThumbLX), float(m_aButtonState[nPlayer].Gamepad.sThumbLY)); }

	//右スティック
	SHORT GetGameStickRXPress(int nPlayer) { return m_aButtonState[nPlayer].Gamepad.sThumbRX; }
	SHORT GetGameStickRYPress(int nPlayer) { return m_aButtonState[nPlayer].Gamepad.sThumbRY; }
	SHORT GetGameStickRXTrigger(int nPlayer) { return m_aButtonStateTrigger[nPlayer].Gamepad.sThumbRX; }
	SHORT GetGameStickRYTrigger(int nPlayer) { return m_aButtonStateTrigger[nPlayer].Gamepad.sThumbRY; }
	SHORT GetGameStickRXRelease(int nPlayer) { return m_aButtonStateRelease[nPlayer].Gamepad.sThumbRX; }
	SHORT GetGameStickRYRelease(int nPlayer) { return m_aButtonStateRelease[nPlayer].Gamepad.sThumbRY; }
	float GetStickRRot(int nPlayer) { return atan2f(float(m_aButtonState[nPlayer].Gamepad.sThumbRX), float(m_aButtonState[nPlayer].Gamepad.sThumbRY)); }

protected:
	XINPUT_STATE m_aButtonState[NUM_PAD_MAX];			//PADのプレス情報
	XINPUT_STATE m_aButtonStateTrigger[NUM_PAD_MAX];	//PADのトリガー情報
	XINPUT_STATE m_aButtonStateRelease[NUM_PAD_MAX];	//PADのリリース情報

private:

};


//マウスクラス(子)
class CInputMouse : public CInput
{
public:
	//ボタンの列挙型
	typedef enum
	{
		BUTTON_LEFT = 0,			//左ボタン
		BUTTON_RIGHT,				//右ボタン
		BUTTON_WHEEL,				//ホイール押し込み
		BUTTON_MAX
	}BUTTON;

	//#define NUM_MOUSE_BUTTON (sizeof(DIMOUSESTATE2 rgbButtons) / sizeof(BYTE))			//構造体配列数
	#define NUM_MOUSE_BUTTON (8)			//マウスボタン配列数

	CInputMouse();		//コンストラクタ
	~CInputMouse();		//デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//初期化
	void Uninit(void);			//終了
	void Update(void);			//更新

	bool GetPress(int nKey) { return (m_aKeyState[nKey] & 0x80) ? true : false; }		//プレス情報取得
	bool GetTrigger(int nKey) { return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false; }		//トリガー情報取得
	bool GetRelease(int nKey) { return (m_aKeyStateRelease[nKey] & 0x80) ? true : false; }		//リリース情報取得
	D3DXVECTOR3 GetPos() { return m_pos; }		//位置取得
	D3DXVECTOR3 GetChangePos() { return D3DXVECTOR3((float)MouseState.lX, (float)MouseState.lY, (float)MouseState.lZ); }		//位置取得
protected:
	BYTE m_aKeyState[NUM_MOUSE_BUTTON];					//キーボードのプレス情報
	BYTE m_aKeyStateTrigger[NUM_MOUSE_BUTTON];			//キーボードのトリガー情報
	BYTE m_aKeyStateRelease[NUM_MOUSE_BUTTON];			//キーボードのリリース情報
	DIMOUSESTATE2 MouseState;		//全入力情報の保管
private:
	D3DXVECTOR3 m_pos;	//位置
};

#endif
