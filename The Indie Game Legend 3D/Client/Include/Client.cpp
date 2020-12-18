// Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "MainApp.h"
#include "MsgManager.h"
#define MAX_LOADSTRING 128

#pragma region GLOBAL VARIABLE
HINSTANCE	g_hInstance;                        // ���� �ν��Ͻ��Դϴ�.
HWND		g_hWnd;								// ���� ������ �ڵ�.
TCHAR		szTitle[MAX_LOADSTRING];			// ���� ǥ���� �ؽ�Ʈ.
TCHAR		szWindowClass[MAX_LOADSTRING];		// ���� ������ Ŭ���� �̸�.
CONST INT	g_nWinCX = 1280;						// ���� ������ ���� ����.
CONST INT	g_nWinCY = 720;						// ���� ������ ���� ����.
#pragma endregion

#pragma region FUNCTION PROTOTYPE
ATOM                MyRegisterClass(HINSTANCE hInstance);
HRESULT             InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
#pragma endregion

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdParam, int nCmdShow)
{
	//���� �õ� �ʱ�ȭ.
	srand((unsigned)time(NULL));
	//Ŀ�� ���߱�.
	ShowCursor(false);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//���� ���ڿ� �ʱ�ȭ.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);

	//������ Ŭ���� ���.
	MyRegisterClass(hInstance);

	//������ ���� �� ���� ���α׷� �ʱ�ȭ.
	if (FAILED(InitInstance(hInstance, nCmdShow)))
	{
		return FALSE;
	}

	// �޽��� ����.
	MSG tMessage;
	tMessage.message = WM_NULL;

	CMainApp* pMainApp = CMainApp::Create();
	if (nullptr == pMainApp)
		return FALSE;

	while (WM_QUIT != tMessage.message)
	{
		if (PeekMessage(&tMessage, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&tMessage);
			DispatchMessage(&tMessage);
		}

		pMainApp->Update();
	}

	SafeRelease(pMainApp);
	
	return int(tMessage.wParam);
}


//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
HRESULT InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInstance = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

							 //������ ũ�� ����.
	RECT tWndRect = { 0,0,g_nWinCX,g_nWinCY };
	AdjustWindowRect(&tWndRect, WS_OVERLAPPEDWINDOW, FALSE);

	//������ ����.
	g_hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		tWndRect.right - tWndRect.left,
		tWndRect.bottom - tWndRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	if (INVALID_HANDLE_VALUE == g_hWnd)
	{
		return FALSE;
	}
	//������ ������ ǥ��.
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	if (WM_DESTROY == nMessage)
		PostQuitMessage(0);
	if (WM_SIZE == nMessage)
	{
		CMsgManager::GetInstance()->ReSizeClient();
	}
	return DefWindowProc(hWnd, nMessage, wParam, lParam);
}

