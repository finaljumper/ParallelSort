// MonitorTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MonitorTest.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WINDOWPLACEMENT wpc;
HMENU hMenu;
static HBRUSH       g_brush[5];
static int          g_brush_index = 0;
bool FullScreen = false;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    HowToUse(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	g_brush[0] = (HBRUSH)GetStockObject(WHITE_BRUSH);
	g_brush[1] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	g_brush[2] = (HBRUSH)GetStockObject(GRAY_BRUSH);
	g_brush[3] = CreateSolidBrush(RGB(255, 0, 0));
	g_brush[4] = CreateSolidBrush(RGB(0, 0, 255));
	g_brush[5] = CreateSolidBrush(RGB(0, 255, 0));

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MONITORTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MONITORTEST));

    MSG msg;

    // Main message loop:
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


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MONITORTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = g_brush[g_brush_index];
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MONITORTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

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

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_LBUTTONDOWN:
		if (g_brush_index != 5)
		{
			g_brush_index += 1;
		}
		else
		{
			g_brush_index = 0;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
		{
			if (!FullScreen)//Из оконного во весь экран
			{
				GetWindowPlacement(hWnd, &wpc);//Сохраняем параметры оконного режима
				SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);//Устанавливаем новые стили
				SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
				hMenu = GetMenu(hWnd);
				SetMenu(hWnd, NULL);
				ShowWindow(hWnd, SW_SHOWMAXIMIZED);//Окно во весь экран
				FullScreen = true;
			}
			else//Из всего экрана в оконное
			{
				SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);//Устанавливаем стили оконного режима
				SetWindowLong(hWnd, GWL_EXSTYLE, 0L);
				SetWindowPlacement(hWnd, &wpc);//Загружаем парметры предыдущего оконного режима
				SetMenu(hWnd, hMenu);
				ShowWindow(hWnd, SW_SHOWDEFAULT);//Показываем обычное окно
				FullScreen = false;
			}
		}
		return 0;
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_WHITE:
				g_brush_index = 0;
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			case ID_BLACK:
				g_brush_index = 1;
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			case ID_GRAY:
				g_brush_index = 2;
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			case ID_RED:
				g_brush_index = 3;
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			case ID_GREEN:
				g_brush_index = 5;
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			case ID_BLUE:
				g_brush_index = 4;
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			case ID_HOWTOUSE:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_HOWTOUSE), hWnd, HowToUse);
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			FillRect(ps.hdc, &ps.rcPaint, g_brush[g_brush_index]);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

INT_PTR CALLBACK HowToUse(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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