// 2osisp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "2osisp.h"
#include "TableDrawer.h"
#include "TextGenerator.h"

#define MAX_LOADSTRING 100
#define ROW_COUNT 2
#define COLUMN_COUNT 5

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
VOID				SourceArrayInit(HANDLE hFile);
const wchar_t*      GetWC(char* c);

RECT clientSize;
char sourceList[ROW_COUNT * COLUMN_COUNT][255];
std::vector<std::wstring> stdsourceList(ROW_COUNT*COLUMN_COUNT);
TableDrawer* drawer;// = new TableDrawer(ROW_COUNT, COLUMN_COUNT, stdsourceList);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2OSISP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

   
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
       DispatchMessage(&msg);
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
    wcex.hIcon          = NULL;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = NULL;

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
//  PURPOSE: Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		const TCHAR szFileName[] = L"D:/MY/STUDY/thx/5 сем/OSnSP/2osisp/2osisp/data.dat";
		HANDLE hFile = CreateFile(szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile) {
			MessageBox(hWnd, L"File not found!", L"Error", MB_OK);
		}
		else {
			SourceArrayInit(hFile);
			drawer = new TableDrawer(ROW_COUNT, COLUMN_COUNT, stdsourceList);
			CloseHandle(hFile);
		}
	}
	break;

	case (WM_PAINT):
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			HBRUSH brush;
			COLORREF colorText = RGB(100, 100, 144),
				colorBack = RGB(4, 20, 4);
			brush = CreateSolidBrush(colorBack);
			SelectObject(hdc, brush);
			Rectangle(hdc, clientSize.left, clientSize.top, clientSize.right, clientSize.bottom);
			DeleteObject(brush);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, colorText);
			const auto color = RGB(195, 195, 195);
			const auto pen = CreatePen(PS_SOLID, 4, color);
			SelectObject(hdc, pen);

			drawer->SetClientSize(clientSize);
			drawer->RefreshTable(hdc);

			DeleteObject(hdc);
            
			EndPaint(hWnd, &ps);
        }
        break;
	case WM_SIZE:
	{
		GetClientRect(hWnd, &clientSize);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HBRUSH brush;
		COLORREF colorText = RGB(100, 100, 144),
			colorBack = RGB(4, 20, 4);
		brush = CreateSolidBrush(colorBack);
		SelectObject(hdc, brush);
		Rectangle(hdc, clientSize.left, clientSize.top, clientSize.right, clientSize.bottom);
		DeleteObject(brush);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, colorText);
		const auto color = RGB(195, 195, 195);
		const auto pen = CreatePen(PS_SOLID, 1, color);
		SelectObject(hdc, pen);

		drawer->SetClientSize(clientSize);
		drawer->RefreshTable(hdc);

		DeleteObject(hdc);

		EndPaint(hWnd, &ps);

	}
	break;
	case WM_GETMINMAXINFO:
	{
		const auto lpMmi = reinterpret_cast<LPMINMAXINFO>(lParam);
		lpMmi->ptMinTrackSize.x = 500;
		lpMmi->ptMinTrackSize.y = 300;
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

VOID SourceArrayInit(HANDLE hFile) {

	//const int num = ROW_COUNT * COLUMN_COUNT;
	DWORD numOfBytesToRead = GetFileSize(hFile, NULL) / (ROW_COUNT * COLUMN_COUNT);

	for (int i = 0; i < ROW_COUNT*COLUMN_COUNT; i++)
		{
				char cBufferText[255];
				DWORD dwByte;
				ReadFile(hFile, cBufferText, numOfBytesToRead, &dwByte, NULL);
				cBufferText[numOfBytesToRead] = 0;
				const wchar_t* str = GetWC(cBufferText);
				//strcpy_s(sourceList[i], sizeof(cBufferText), cBufferText);
				std::wstring copied(str,numOfBytesToRead);
				stdsourceList[i] = copied;
		
	}

}

const wchar_t* GetWC( char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

