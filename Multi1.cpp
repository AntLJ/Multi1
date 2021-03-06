// Multi1.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Multi1.h"
#include <math.h> 

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

int cyChar;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MULTI1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MULTI1));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

int CheckBottom(HWND hwnd, int cyClient, int iLine)
{
	if (iLine * cyChar + cyChar > cyClient)
	{
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
		iLine = 0;
	}
	return iLine;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MULTI1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MULTI1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// -------------------------------------------------------------------------
// Window 1: Display increasing sequence of numbers
// -------------------------------------------------------------------------
LRESULT APIENTRY WndProc1(HWND hwnd, UINT message, WPARAM wParam, LPARAM
	lParam)
{
	static int iNum, iLine, cyClient;
	HDC hdc;
	TCHAR szBuffer[16];
	switch (message)
	{
	case WM_SIZE:
		cyClient = HIWORD(lParam);
		return 0;
	case WM_TIMER:
		if (iNum < 0)
			iNum = 0;
		iLine = CheckBottom(hwnd, cyClient, iLine);
		hdc = GetDC(hwnd);
		TextOut(hdc, 0, iLine * cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum++));
		ReleaseDC(hwnd, hdc);
		iLine++;
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
// --------------------------------------------------------------------------
// Window 2: Display increasing sequence of prime numbers
// --------------------------------------------------------------------------
LRESULT APIENTRY WndProc2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int iNum = 1, iLine, cyClient;
	HDC hdc;
	int i, iSqrt;
	TCHAR szBuffer[16];

	switch (message)
	{
	case WM_SIZE:
		cyClient = HIWORD(lParam);
		return 0;

	case WM_TIMER:
		do {
			if (++iNum < 0)
				iNum = 0;

			iSqrt = (int)sqrt(iNum);
			for (i = 2; i <= iSqrt; i++)
				if (iNum % i == 0)
					break;
		} while (i <= iSqrt);

		iLine = CheckBottom(hwnd, cyClient, iLine);
		hdc = GetDC(hwnd);
		TextOut(hdc, 0, iLine * cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum));
		ReleaseDC(hwnd, hdc);
		iLine++;
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
// --------------------------------------------------------------------------
// Window 3: Display increasing sequence of Fibonacci numbers
// --------------------------------------------------------------------------
LRESULT APIENTRY WndProc3(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int iNum = 0, iNext = 1, iLine, cyClient;
	HDC hdc;
	int iTemp;
	TCHAR szBuffer[16];

	switch (message)
	{
	case WM_SIZE:
		cyClient = HIWORD(lParam);
		return 0;

	case WM_TIMER:
		if (iNum < 0)
		{
			iNum = 0;
			iNext = 1;
		}

		iLine = CheckBottom(hwnd, cyClient, iLine);
		hdc = GetDC(hwnd);
		TextOut(hdc, 0, iLine * cyChar, szBuffer, wsprintf(szBuffer, TEXT("%d"), iNum));
		ReleaseDC(hwnd, hdc);
		iTemp = iNum;
		iNum = iNext;
		iNext += iTemp;
		iLine++;
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
// --------------------------------------------------------------------------
// Window 4: Display circles of random radii
//---------------------------------------------------------------------------
LRESULT APIENTRY WndProc4(HWND hwnd, UINT message, WPARAM wParam, LPARAM
	lParam)
{
	static int cxClient, cyClient;
	HDC hdc;
	int iDiameter;

	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);

		iDiameter = rand() % (max(1, min(cxClient, cyClient)));
		hdc = GetDC(hwnd);

		Ellipse(hdc, (cxClient - iDiameter) / 2, (cyClient - iDiameter) / 2, (cxClient + iDiameter) / 2, (cyClient + iDiameter) / 2);

		ReleaseDC(hwnd, hdc);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

// --------------------------------------------------------------------------
// Main window to create child windows
// --------------------------------------------------------------------------
LRESULT APIENTRY WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM
	lParam)
{
	static HWND hwndChild[4];
	static const TCHAR * szChildClass[] = { TEXT("Child1"), TEXT("Child2"), TEXT("Child3"), TEXT("Child4") };
	static WNDPROC ChildProc[] = { WndProc1, WndProc2, WndProc3, WndProc4 };
	HINSTANCE hInstance;
	int i, cxClient, cyClient;
	WNDCLASS wndclass;

	switch (message)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInstance;
		wndclass.hIcon = NULL;
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;

		for (i = 0; i < 4; i++)
		{
			wndclass.lpfnWndProc = ChildProc[i];
			wndclass.lpszClassName = szChildClass[i];

			RegisterClass(&wndclass);

			hwndChild[i] = CreateWindow(szChildClass[i], NULL,
				WS_CHILDWINDOW | WS_BORDER | WS_VISIBLE,
				0, 0, 0, 0,
				hwnd, (HMENU)i, hInstance, NULL);
		}
		cyChar = HIWORD(GetDialogBaseUnits());
		SetTimer(hwnd, 1, 10, NULL);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		for (i = 0; i < 4; i++)
			MoveWindow(hwndChild[i], (i % 2) * cxClient / 2, (i > 1) * cyClient / 2, cxClient / 2, cyClient / 2, TRUE);
		return 0;
	case WM_TIMER:
		for (i = 0; i < 4; i++)
			SendMessage(hwndChild[i], WM_TIMER, wParam, lParam);
		return 0;
	case WM_CHAR:
		if (wParam == '\x1B')
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
