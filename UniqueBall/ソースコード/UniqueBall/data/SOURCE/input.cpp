//===============================================
//
// 入力処理(input.cpp)
// Author 髙田 佳依
//
//===============================================
#include "input.h"

#include "debugproc.h"
#include "manager.h"

//======================================
// 静的メンバ変数宣言
//======================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//======================================
// [入力]コンストラクタ
//======================================
CInput::CInput() 
{
	//m_pInput = nullptr;
	m_pDevice = nullptr;
}

//======================================
// [入力]デストラクタ
//======================================
CInput::~CInput()
{

}

//======================================
// [入力]初期化
//======================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		//DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//======================================
// [入力]終了(仮装)
//======================================
void CInput::Uninit(void)
{
	//入力デバイス(キーボード)の破棄
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();				//キーボードへのアクセス権を放棄
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	//入力デバイス(キーボード)の破棄
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//========================= キーボード =========================
//======================================
// [キーボード]コンストラクタ
//======================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCnt = 0; nCnt < NUM_KEY_MAX; nCnt++)
	{
		m_aKeyState[nCnt] = 0;
		m_aKeyStateTrigger[nCnt] = 0;
		m_aKeyStateRelease[nCnt] = 0;
	}
}

//======================================
// [キーボード]デストラクタ
//======================================
CInputKeyboard::~CInputKeyboard()
{

}

//======================================
// [キーボード]初期化
//======================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	if (m_pInput != nullptr)
	{
		//入力デバイス(キーボード)の生成
		if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}

		//データフォーマットの設定
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			return E_FAIL;
		}

		//協調モードを設定
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		//キーボードへのアクセス権を獲得
		m_pDevice->Acquire();

		return S_OK;
	}

	return E_FAIL;
}

//======================================
// [キーボード]終了
//======================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//======================================
// [キーボード]更新
//======================================
void CInputKeyboard::Update(void)
{
 	BYTE aKeyState[NUM_KEY_MAX];		//キーボードの入力情報
	int nCntKey;

	if (GetTrigger(DIK_RETURN))
	{
		int n;
		n = 1;
	}

	//入力デバイスからデータ取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];		//キーボードのリリース情報を保有
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//キーボードのトリガー情報を保有
			m_aKeyState[nCntKey] = aKeyState[nCntKey];		//キーボードのプレス情報を保有
		}
	}
	else
	{
		m_pDevice->Acquire();						//キーボードへのアクセス権を獲得
	}
}

//========================= XPad =========================
//======================================
// [XPad]コンストラクタ
//======================================
CInputGamepad::CInputGamepad()
{
	XInputEnable(true);

	for (int nCnt = 0; nCnt < NUM_PAD_MAX; nCnt++)
	{
		//メモリをクリア
		memset(&m_aButtonState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&m_aButtonStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&m_aButtonStateRelease[nCnt], 0, sizeof(XINPUT_STATE));
	}
}

//======================================
// [XPad]デストラクタ
//======================================
CInputGamepad::~CInputGamepad()
{

}

//======================================
// [XPad]初期化
//======================================
HRESULT CInputGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	return S_OK;
}

//======================================
// [XPad]終了
//======================================
void CInputGamepad::Uninit(void)
{
	CInput::Uninit();

	XInputEnable(false);
}

//======================================
// [XPad]更新
//======================================
void CInputGamepad::Update(void)
{
	XINPUT_STATE aButtonState[NUM_PAD_MAX];			//ボタンの入力情報
	//XINPUT_STATE aStickState[NUM_KEY_MAX];		//スティックの入力情報
	static int nCntSpace = 0;						//スペース

	//入力デバイスからデータ取得
	for (int nCntKey = 0; nCntKey < NUM_PAD_MAX; nCntKey++)
	{
		if ((XInputGetState(nCntKey, &aButtonState[nCntKey])) == ERROR_SUCCESS)
		{
			m_aButtonStateRelease[nCntKey].Gamepad.wButtons = (m_aButtonState[nCntKey].Gamepad.wButtons ^ aButtonState[nCntKey].Gamepad.wButtons) & m_aButtonState[nCntKey].Gamepad.wButtons;		//キーボードのリリース情報を保有
			m_aButtonStateTrigger[nCntKey].Gamepad.wButtons = (m_aButtonState[nCntKey].Gamepad.wButtons ^ aButtonState[nCntKey].Gamepad.wButtons) & aButtonState[nCntKey].Gamepad.wButtons;		//キーボードのトリガー情報を保有
			m_aButtonState[nCntKey].Gamepad.wButtons = aButtonState[nCntKey].Gamepad.wButtons;		//PADのプレス情報を保有

			//トリガーリリース修正すべし
			m_aButtonStateRelease[nCntKey].Gamepad.bLeftTrigger = (m_aButtonState[nCntKey].Gamepad.bLeftTrigger ^ aButtonState[nCntKey].Gamepad.bLeftTrigger) & m_aButtonState[nCntKey].Gamepad.bLeftTrigger;		//キーボードのリリース情報を保有
			m_aButtonStateTrigger[nCntKey].Gamepad.bLeftTrigger = (m_aButtonState[nCntKey].Gamepad.bLeftTrigger ^ aButtonState[nCntKey].Gamepad.bLeftTrigger) & aButtonState[nCntKey].Gamepad.bLeftTrigger;		//キーボードのトリガー情報を保有
			m_aButtonState[nCntKey].Gamepad.bLeftTrigger = aButtonState[nCntKey].Gamepad.bLeftTrigger;		//Lトリガーのプレス情報を保有

			m_aButtonStateRelease[nCntKey].Gamepad.bRightTrigger = (m_aButtonState[nCntKey].Gamepad.bRightTrigger ^ aButtonState[nCntKey].Gamepad.bRightTrigger) & m_aButtonState[nCntKey].Gamepad.bRightTrigger;		//キーボードのリリース情報を保有
			m_aButtonStateTrigger[nCntKey].Gamepad.bRightTrigger = (m_aButtonState[nCntKey].Gamepad.bRightTrigger ^ aButtonState[nCntKey].Gamepad.bRightTrigger) & aButtonState[nCntKey].Gamepad.bRightTrigger;		//キーボードのトリガー情報を保有
			m_aButtonState[nCntKey].Gamepad.bRightTrigger = aButtonState[nCntKey].Gamepad.bRightTrigger;		//Rトリガーのプレス情報を保有

			//スティック ReleaseとTriggerはだるい
			m_aButtonState[nCntKey].Gamepad = aButtonState[nCntKey].Gamepad;
		}
	}
}

//======================================
// いずれか入力
//======================================
bool CInputGamepad::GetPressOR(PADBUTTON button)
{
	for (int i = 0; i < sizeof(m_aButtonState) / sizeof(m_aButtonState[0]); i++)
	{
		if (m_aButtonState[i].Gamepad.wButtons & (0x01 << button) ? true : false)
		{
			return true;
		}
	}

	return false;
}

//======================================
// いずれか入力
//======================================
bool CInputGamepad::GetTriggerOR(PADBUTTON button)
{
	for (int i = 0; i < sizeof(m_aButtonStateTrigger) / sizeof(m_aButtonStateTrigger[0]); i++)
	{
		if (m_aButtonStateTrigger[i].Gamepad.wButtons & (0x01 << button) ? true : false)
		{
			return true;
		}
	}

	return false;
}

//======================================
// いずれか入力
//======================================
bool CInputGamepad::GetReleaseOR(PADBUTTON button)
{
	for (int i = 0; i < sizeof(m_aButtonStateRelease) / sizeof(m_aButtonStateRelease[0]); i++)
	{
		if (m_aButtonStateRelease[i].Gamepad.wButtons & (0x01 << button) ? true : false)
		{
			return true;
		}
	}

	return false;
}

//========================= マウス =========================
//======================================
// [マウス]コンストラクタ
//======================================
CInputMouse::CInputMouse()
{
	for (int nCnt = 0; nCnt < NUM_MOUSE_BUTTON; nCnt++)
	{
		m_aKeyState[nCnt] = 0;
		m_aKeyStateTrigger[nCnt] = 0;
		m_aKeyStateRelease[nCnt] = 0;
		MouseState.rgbButtons[nCnt] = 0;
	}

	MouseState.lX = 0;
	MouseState.lY = 0;
	MouseState.lZ = 0;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = m_pos;

	// マウス座標を取得する
	POINT p;
	if (GetCursorPos(&p))
	{
		// スクリーン座標をクライアント座標に変換する
		ScreenToClient(FindWindowA(WINDOW_NAME, nullptr), &p);

		m_pos = D3DXVECTOR3(float(p.x), float(p.y), 0.0f);
	}
}

//======================================
// [マウス]デストラクタ
//======================================
CInputMouse::~CInputMouse()
{

}

//======================================
// [マウス]初期化
//======================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	if (m_pInput != nullptr)
	{
		//入力デバイス(マウス)の生成
		if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}

		//データフォーマットの設定
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
		{
			return E_FAIL;
		}

		//協調モードを設定
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		//デバイスの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;	//相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

		if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
		{
			//デバイスの設定に失敗
			return E_FAIL;
		}

		//マウスへのアクセス権を獲得
		m_pDevice->Acquire();
	
		return S_OK;
	}

	return E_FAIL;
}

//======================================
// [マウス]終了
//======================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//======================================
// [マウス]更新
//======================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 mouse;	//マウスの入力情報
	m_posOld = m_pos;

	//入力デバイスからデータ取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(mouse), &mouse)))
	{
		for (int nCntKey = 0; nCntKey < NUM_MOUSE_BUTTON; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (MouseState.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey]) & mouse.rgbButtons[nCntKey];				//キーボードのトリガー情報を保有
			m_aKeyStateRelease[nCntKey] = (MouseState.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey]) & MouseState.rgbButtons[nCntKey];		//キーボードのリリース情報を保有
			m_aKeyState[nCntKey] = mouse.rgbButtons[nCntKey];					//キーボードのプレス情報を保有
			MouseState.rgbButtons[nCntKey] = mouse.rgbButtons[nCntKey];			//キーボードのプレス情報を保有
		}

		MouseState = mouse;
		m_pos += D3DXVECTOR3((float)mouse.lX, (float)mouse.lY, (float)mouse.lZ);
	}
	else
	{
		m_pDevice->Acquire();	//マウスへのアクセス権を獲得
	}

	//デバッグ
	D3DXVECTOR3 changepos = m_pos - m_posOld;

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- インプット情報 ---\n");
	pDebug->Print("変更位置：%f %f %f\n", changepos.x, changepos.y, changepos.z);
}