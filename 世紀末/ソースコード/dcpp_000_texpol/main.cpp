//===============================================
//
// |S`æ(main.cpp)
// Author ûüc ÀË
//
//===============================================

//=========================
// CN[ht@C
//=========================
#include "main.h"
#include "manager.h"

//=========================
// }Nè`
//=========================
#define CLASS_NAME "WindowClass"				//EBhENXÌ¼O
#define ID_TIMER (131)							//^C}[ÌID
#define TIMER_INTERVAL (1000/60)				//^C}[Ì­¶Ôu(~b)

//=========================
// vg^Cvé¾
//=========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=========================
// O[oÏ
//=========================
int g_nCountFPS = 0;							//FPSJE^

//============================
// CÖ
//============================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//I¹É[NðoÍ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	DWORD dwCurrentTime;						//»Ý
	DWORD dwExecLastTime;						//ÅãÉµ½
	DWORD dwFrameCount;							//t[JEg
	DWORD dwFPSLastTime;						//ÅãÉFPSðvªµ½

	WNDCLASSEX wcex 
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEXÌTCY
		CS_CLASSDC,								//EBhEÌX^C
		WindowProc,								//EBhEvV[W
		0,										//0É·éiÊígpµÈ¢j
		0,										//0É·éiÊígpµÈ¢j
		hInstance,								//CX^Xnh
		LoadIcon(NULL,IDI_APPLICATION),			//^XNo[ÌACR
		LoadCursor(NULL,IDC_ARROW),				//}EXJ[\
		(HBRUSH)(COLOR_WINDOW + 1),				//NCAgÌæÌwiF(+ 0`26ÌlðÏ¦é±ÆÅFªÏíé)
		NULL,									//j[o[
		CLASS_NAME,								//EBhENXÌ¼O
		LoadIcon(NULL,IDI_APPLICATION)			//t@CÌACR
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	
	//}l[W
	CManager *pManager = nullptr;

	//EBhENXÌo^
	RegisterClassEx(&wcex);

	//NCAgÌæðwèÌTCYÉ²®
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//EBhEð¶¬
	hWnd = CreateWindowEx(0,			//g£EBhEX^C
		CLASS_NAME,						//EBhENXÌ¼O
		WINDOW_NAME,					//EBhEÌ¼O
		WS_OVERLAPPEDWINDOW,			//EBhEX^C
		CW_USEDEFAULT,					//EBhEÌ¶ãXÀW
		CW_USEDEFAULT,					//EBhEÌ¶ãYÀW
		(rect.right - rect.left),		//EBhEÌ¡
		(rect.bottom - rect.top),		//EBhEÌc
		NULL,							//eEBhEÌnh
		NULL,							//j[nhÜ½ÍqEBhEID
		hInstance,						//CX^Xnh
		NULL);							//EBhEì¬f[^

	//ªð\ðÝè
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//ú»
	dwExecLastTime = timeGetTime();		//»Ýðæ¾
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//}l[WÌ¶¬
	if (pManager != nullptr)
	{
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	pManager = new CManager;

	//}l[WÌú»
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return E_FAIL;
	}

	//EBhE\¦
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//bZ[W[v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//WindowsÌ
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//bZ[WÌÝè
				TranslateMessage(&msg);			//EBhEÌ\¦óÔðÝè
				DispatchMessage(&msg);			//NCAgÌæðXV			
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();		//»Ýðæ¾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5boß
			 //FPSðªè
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		//FPSðªèµ½ðÛ¶
				dwFrameCount = 0;					//t[JEgðNA
			}

			if ((dwCurrentTime - dwExecLastTime) >= TIMER_INTERVAL)
			{//60ªÌ1boß

				dwExecLastTime = dwCurrentTime;		//Jn[»Ý]ðÛ¶

				//DirectXÌ
				//}l[WÌXV
				pManager->Update();

				//}l[WÌ`æ
				pManager->Draw();

				dwFrameCount++;
			}
		
		}
	}

	//}l[WÌjü
	if (pManager != nullptr)
	{
		pManager->Uninit();
		delete pManager;
		pManager = nullptr;
	}

	//ªð\ðß·
	timeEndPeriod(1);

	//EBhENXÌo^ðð
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//============================
// EBhEvV[W
//============================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{//
	case WM_DESTROY:
		//WM_QUITbZ[Wðé
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//L[ºÌbZ[W
		switch (wParam)
		{
		case VK_ESCAPE:		//{ESC}L[ª³ê½

			int nNumCsr = 0;

			nNumCsr = ShowCursor(true);

			do
			{//J[\\¦ðtrue(1)É·é½ß[v

				if (nNumCsr > 1)
				{
					nNumCsr = ShowCursor(false);
				}
				else if (nNumCsr < 1)
				{
					nNumCsr = ShowCursor(true);
				}

			} while (nNumCsr != 1);

			nID = MessageBox(hWnd, "I¹µÜ·©H", "I¹bZ[W", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{
				//EBhEðjü·é(WM_DESTROYbZ[Wðé)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:			//Â¶é{^ºÌbZ[W

		int nNumCsr = 0;
	
		do
		{//J[\\¦ðtrue(1)É·é½ß[v

			nNumCsr = ShowCursor(true);

			if (nNumCsr > 1)
			{
				nNumCsr = ShowCursor(false);
			}
			else if (nNumCsr < 1)
			{
				nNumCsr = ShowCursor(true);
			}

		} while (nNumCsr != 1);

		nID = MessageBox(hWnd, "I¹µÜ·©H", "I¹bZ[W", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			//EBhEðjü·é(WM_DESTROYbZ[Wðé)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0ðÔ³È¢ÆI¹µÄµÜ¤
		}
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);				//ùèÌðÔ·
}

//=================================
// FPSæ¾
//=================================
int GetFPS(void)
{
	return g_nCountFPS;
}
