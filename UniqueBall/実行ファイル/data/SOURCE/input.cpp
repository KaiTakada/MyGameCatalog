//===============================================
//
// όΝ(input.cpp)
// Author ϋόc ΐΛ
//
//===============================================
#include "input.h"

#include "debugproc.h"
#include "manager.h"

//======================================
// ΓIoΟιΎ
//======================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//======================================
// [όΝ]RXgN^
//======================================
CInput::CInput() 
{
	//m_pInput = nullptr;
	m_pDevice = nullptr;
}

//======================================
// [όΝ]fXgN^
//======================================
CInput::~CInput()
{

}

//======================================
// [όΝ]ϊ»
//======================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == nullptr)
	{
		//DirectInputIuWFNgΜΆ¬
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//======================================
// [όΝ]IΉ(Ό)
//======================================
void CInput::Uninit(void)
{
	//όΝfoCX(L[{[h)Μjό
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();				//L[{[hΦΜANZX πϊό
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	//όΝfoCX(L[{[h)Μjό
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

//========================= L[{[h =========================
//======================================
// [L[{[h]RXgN^
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
// [L[{[h]fXgN^
//======================================
CInputKeyboard::~CInputKeyboard()
{

}

//======================================
// [L[{[h]ϊ»
//======================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	if (m_pInput != nullptr)
	{
		//όΝfoCX(L[{[h)ΜΆ¬
		if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}

		//f[^tH[}bgΜέθ
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			return E_FAIL;
		}

		//¦²[hπέθ
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		//L[{[hΦΜANZX πlΎ
		m_pDevice->Acquire();

		return S_OK;
	}

	return E_FAIL;
}

//======================================
// [L[{[h]IΉ
//======================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//======================================
// [L[{[h]XV
//======================================
void CInputKeyboard::Update(void)
{
 	BYTE aKeyState[NUM_KEY_MAX];		//L[{[hΜόΝξρ
	int nCntKey;

	if (GetTrigger(DIK_RETURN))
	{
		int n;
		n = 1;
	}

	//όΝfoCX©ηf[^ζΎ
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];		//L[{[hΜ[XξρπΫL
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];		//L[{[hΜgK[ξρπΫL
			m_aKeyState[nCntKey] = aKeyState[nCntKey];		//L[{[hΜvXξρπΫL
		}
	}
	else
	{
		m_pDevice->Acquire();						//L[{[hΦΜANZX πlΎ
	}
}

//========================= XPad =========================
//======================================
// [XPad]RXgN^
//======================================
CInputGamepad::CInputGamepad()
{
	XInputEnable(true);

	for (int nCnt = 0; nCnt < NUM_PAD_MAX; nCnt++)
	{
		//πNA
		memset(&m_aButtonState[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&m_aButtonStateTrigger[nCnt], 0, sizeof(XINPUT_STATE));
		memset(&m_aButtonStateRelease[nCnt], 0, sizeof(XINPUT_STATE));
	}
}

//======================================
// [XPad]fXgN^
//======================================
CInputGamepad::~CInputGamepad()
{

}

//======================================
// [XPad]ϊ»
//======================================
HRESULT CInputGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	return S_OK;
}

//======================================
// [XPad]IΉ
//======================================
void CInputGamepad::Uninit(void)
{
	CInput::Uninit();

	XInputEnable(false);
}

//======================================
// [XPad]XV
//======================================
void CInputGamepad::Update(void)
{
	XINPUT_STATE aButtonState[NUM_PAD_MAX];			//{^ΜόΝξρ
	//XINPUT_STATE aStickState[NUM_KEY_MAX];		//XeBbNΜόΝξρ
	static int nCntSpace = 0;						//Xy[X

	//όΝfoCX©ηf[^ζΎ
	for (int nCntKey = 0; nCntKey < NUM_PAD_MAX; nCntKey++)
	{
		if ((XInputGetState(nCntKey, &aButtonState[nCntKey])) == ERROR_SUCCESS)
		{
			m_aButtonStateRelease[nCntKey].Gamepad.wButtons = (m_aButtonState[nCntKey].Gamepad.wButtons ^ aButtonState[nCntKey].Gamepad.wButtons) & m_aButtonState[nCntKey].Gamepad.wButtons;		//L[{[hΜ[XξρπΫL
			m_aButtonStateTrigger[nCntKey].Gamepad.wButtons = (m_aButtonState[nCntKey].Gamepad.wButtons ^ aButtonState[nCntKey].Gamepad.wButtons) & aButtonState[nCntKey].Gamepad.wButtons;		//L[{[hΜgK[ξρπΫL
			m_aButtonState[nCntKey].Gamepad.wButtons = aButtonState[nCntKey].Gamepad.wButtons;		//PADΜvXξρπΫL

			//gK[[XC³·Χ΅
			m_aButtonStateRelease[nCntKey].Gamepad.bLeftTrigger = (m_aButtonState[nCntKey].Gamepad.bLeftTrigger ^ aButtonState[nCntKey].Gamepad.bLeftTrigger) & m_aButtonState[nCntKey].Gamepad.bLeftTrigger;		//L[{[hΜ[XξρπΫL
			m_aButtonStateTrigger[nCntKey].Gamepad.bLeftTrigger = (m_aButtonState[nCntKey].Gamepad.bLeftTrigger ^ aButtonState[nCntKey].Gamepad.bLeftTrigger) & aButtonState[nCntKey].Gamepad.bLeftTrigger;		//L[{[hΜgK[ξρπΫL
			m_aButtonState[nCntKey].Gamepad.bLeftTrigger = aButtonState[nCntKey].Gamepad.bLeftTrigger;		//LgK[ΜvXξρπΫL

			m_aButtonStateRelease[nCntKey].Gamepad.bRightTrigger = (m_aButtonState[nCntKey].Gamepad.bRightTrigger ^ aButtonState[nCntKey].Gamepad.bRightTrigger) & m_aButtonState[nCntKey].Gamepad.bRightTrigger;		//L[{[hΜ[XξρπΫL
			m_aButtonStateTrigger[nCntKey].Gamepad.bRightTrigger = (m_aButtonState[nCntKey].Gamepad.bRightTrigger ^ aButtonState[nCntKey].Gamepad.bRightTrigger) & aButtonState[nCntKey].Gamepad.bRightTrigger;		//L[{[hΜgK[ξρπΫL
			m_aButtonState[nCntKey].Gamepad.bRightTrigger = aButtonState[nCntKey].Gamepad.bRightTrigger;		//RgK[ΜvXξρπΫL

			//XeBbN ReleaseΖTriggerΝΎι’
			m_aButtonState[nCntKey].Gamepad = aButtonState[nCntKey].Gamepad;
		}
	}
}

//======================================
// ’Έκ©όΝ
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
// ’Έκ©όΝ
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
// ’Έκ©όΝ
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

//========================= }EX =========================
//======================================
// [}EX]RXgN^
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

	// }EXΐWπζΎ·ι
	POINT p;
	if (GetCursorPos(&p))
	{
		// XN[ΐWπNCAgΐWΙΟ··ι
		ScreenToClient(FindWindowA(WINDOW_NAME, nullptr), &p);

		m_pos = D3DXVECTOR3(float(p.x), float(p.y), 0.0f);
	}
}

//======================================
// [}EX]fXgN^
//======================================
CInputMouse::~CInputMouse()
{

}

//======================================
// [}EX]ϊ»
//======================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	if (m_pInput != nullptr)
	{
		//όΝfoCX(}EX)ΜΆ¬
		if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}

		//f[^tH[}bgΜέθ
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
		{
			return E_FAIL;
		}

		//¦²[hπέθ
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		//foCXΜέθ
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;	//Ξl[hΕέθiβΞlΝDIPROPAXISMODE_ABSj

		if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
		{
			//foCXΜέθΙΈs
			return E_FAIL;
		}

		//}EXΦΜANZX πlΎ
		m_pDevice->Acquire();
	
		return S_OK;
	}

	return E_FAIL;
}

//======================================
// [}EX]IΉ
//======================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//======================================
// [}EX]XV
//======================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 mouse;	//}EXΜόΝξρ
	m_posOld = m_pos;

	//όΝfoCX©ηf[^ζΎ
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(mouse), &mouse)))
	{
		for (int nCntKey = 0; nCntKey < NUM_MOUSE_BUTTON; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (MouseState.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey]) & mouse.rgbButtons[nCntKey];				//L[{[hΜgK[ξρπΫL
			m_aKeyStateRelease[nCntKey] = (MouseState.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey]) & MouseState.rgbButtons[nCntKey];		//L[{[hΜ[XξρπΫL
			m_aKeyState[nCntKey] = mouse.rgbButtons[nCntKey];					//L[{[hΜvXξρπΫL
			MouseState.rgbButtons[nCntKey] = mouse.rgbButtons[nCntKey];			//L[{[hΜvXξρπΫL
		}

		MouseState = mouse;
		m_pos += D3DXVECTOR3((float)mouse.lX, (float)mouse.lY, (float)mouse.lZ);
	}
	else
	{
		m_pDevice->Acquire();	//}EXΦΜANZX πlΎ
	}

	//fobO
	D3DXVECTOR3 changepos = m_pos - m_posOld;

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("--- Cvbgξρ ---\n");
	pDebug->Print("ΟXΚuF%f %f %f\n", changepos.x, changepos.y, changepos.z);
}