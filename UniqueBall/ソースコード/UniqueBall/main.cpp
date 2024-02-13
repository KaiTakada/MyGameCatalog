//===============================================
//
// 根幹(main.cpp)
// Author 髙田 佳依
//
//===============================================

//=========================
// インクルードファイル
//=========================
#include "main.h"
#include "manager.h"

//=========================
// マクロ定義
//=========================
#define CLASS_NAME "Unique Ball"				//ウィンドウクラスの名前
#define ID_TIMER (131)							//タイマーのID
#define TIMER_INTERVAL (1000/TIMER_FPS)				//タイマーの発生間隔(ミリ秒)

//=========================
// プロトタイプ宣言
//=========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=========================
// グローバル変数
//=========================
int g_nCountFPS = 0;							//FPSカウンタ

//============================
// メイン関数
//============================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//終了時にメモリリークを出力
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	DWORD dwCurrentTime;						//現在時刻
	DWORD dwExecLastTime;						//最後に処理した時刻
	DWORD dwFrameCount;							//フレームカウント
	DWORD dwFPSLastTime;						//最後にFPSを計測した時刻

	WNDCLASSEX wcex 
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								//ウィンドウのスタイル
		WindowProc,								//ウィンドウプロシージャ
		0,										//0にする（通常使用しない）
		0,										//0にする（通常使用しない）
		hInstance,								//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),			//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),				//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				//クライアント領域の背景色(+ 0～26の数値を変えることで色が変わる)
		//(HBRUSH)(COLOR_WINDOW + 1),			//変えれない？
		NULL,									//メニューバー
		CLASS_NAME,								//ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)			//ファイルのアイコン
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	
	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,			//拡張ウィンドウスタイル
		CLASS_NAME,						//ウィンドウクラスの名前
		WINDOW_NAME,					//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,			//ウィンドウスタイル
		CW_USEDEFAULT,					//ウィンドウの左上X座標
		CW_USEDEFAULT,					//ウィンドウの左上Y座標
		(rect.right - rect.left),		//ウィンドウの横幅
		(rect.bottom - rect.top),		//ウィンドウの縦幅
		NULL,							//親ウィンドウのハンドル
		NULL,							//メニューハンドルまたは子ウィンドウID
		hInstance,						//インスタンスハンドル
		NULL);							//ウィンドウ作成データ

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//初期化
	dwExecLastTime = timeGetTime();		//現在時刻を取得
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//マネージャの初期化
	if (FAILED(CManager::GetInstance()->Init(hInstance, hWnd, TRUE)))
	{
		return E_FAIL;
	}

	//ウィンドウ表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);			//ウィンドウの表示状態を設定
				DispatchMessage(&msg);			//クライアント領域を更新			
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();		//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを測定
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;		//FPSを測定した時刻を保存
				dwFrameCount = 0;					//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= TIMER_INTERVAL)
			{//60分の1秒経過

				dwExecLastTime = dwCurrentTime;		//処理開始時刻[現在時刻]を保存

				//DirectXの処理
				//マネージャの更新
				CManager::GetInstance()->Update();

				//マネージャの描画
				CManager::GetInstance()->Draw();

				dwFrameCount++;
			}
		
		}
	}

	CManager::Release();

	//分解能を戻す
	timeEndPeriod(1);

	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//============================
// ウィンドウプロシージャ
//============================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{//
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		//{ESC}キーが押された

			int nNumCsr = 0;

			nNumCsr = ShowCursor(true);

			do
			{//カーソル表示数をtrue(1)にするためループ

				if (nNumCsr > 1)
				{
					nNumCsr = ShowCursor(false);
				}
				else if (nNumCsr < 1)
				{
					nNumCsr = ShowCursor(true);
				}

			} while (nNumCsr != 1);

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);

			if (nID == IDYES)
			{
				//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:			//閉じるボタン押下のメッセージ

		int nNumCsr = 0;
	
		do
		{//カーソル表示数をtrue(1)にするためループ

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

		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0を返さないと終了してしまう
		}
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);				//既定の処理を返す
}

//=================================
// FPS取得
//=================================
int GetFPS(void)
{
	return g_nCountFPS;
}
