// ***************************************************************
//  System     
//  -------------------------------------------------------------
//  Purpose :	Main startup file. Window Creation, Initialization,
//				Message loop, main WndProc gets created here.
// ***************************************************************

#include "Game/Game.h"
#include "RenderDevice/RenderDevice.h"
#include <windows.h>

//------------------------------------------------------------------------
// Purpose  : Global Variables
//------------------------------------------------------------------------
HWND g_hwnd;
static TCHAR szAppName[] = TEXT ("DoD__Engine");
WNDCLASSEX   wndclassex = {0};


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


void CreateMainWindow(HINSTANCE hInstance, int iCmdShow)
{

	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = WndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wndclassex.lpszMenuName  = NULL;
	wndclassex.lpszClassName = szAppName;
	wndclassex.hIconSm       = wndclassex.hIcon;

	if (!RegisterClassEx (&wndclassex))
	{
		MessageBox (NULL, TEXT ("RegisterClassEx failed!"), szAppName, MB_ICONERROR);
	}

	int width	= 1280; //GetSystemMetrics(SM_CXSCREEN);
	int height	= 720; //GetSystemMetrics(SM_CYSCREEN);

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;	

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;;
	AdjustWindowRect(&rect, style, false);

	g_hwnd = CreateWindow ( szAppName, 
							TEXT ("iWorld®"),
							style,
							CW_USEDEFAULT, 
							CW_USEDEFAULT, 
							rect.right-rect.left, 
							rect.bottom-rect.top, 
							NULL, 
							NULL, 
							hInstance,
							NULL); 

	ShowWindow (g_hwnd, iCmdShow);
	UpdateWindow (g_hwnd);

}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG msg = {0};

	// Create the Main Window
    CreateMainWindow(hInstance, iCmdShow);

	// Initialize Everything!
	if(!Game::GetInstance()->Initialize(g_hwnd,hInstance))
		return E_FAIL;

    while (WM_QUIT != msg.message)
    {
		if(PeekMessage(&msg, g_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}
		else
		{
			Game::GetInstance()->Tick();
		}
    }

	UnregisterClass( szAppName, wndclassex.hInstance );

    return msg.wParam;
}

LRESULT CALLBACK  WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
		Game::GetInstance()->Shutdown();
        SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		exit(0);
        break;
    }

	return Game::GetInstance()->HandleMessages(hwnd, message, wParam, lParam);
}







