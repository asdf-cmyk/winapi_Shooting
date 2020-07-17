// Win32Project2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Shooting.h"
#include "Resource.h"
#include "Block.h"
#include <vector>
#include <random>
#include <memory>

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void DrawGrid(HDC, int, int, int, int, int);
void DrawCircle(HDC, double, double, double);
int DrawFlower(HDC, int, int, int, int);
void DrawStar(HDC, int, int, int);
POINT crammer(POINT, POINT, POINT, POINT);
int mkRand(int range)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, range);
	return dist(mt);
}

void startScene(HDC);
void gameScene(HDC);
void endScene(HDC);

static TCHAR idStr[4] = L"ABC";
static int score;
static int radCannon;
static vector<Block*> blockCont;



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
	LoadStringW(hInstance, IDC_SHOOTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_SHOOTING));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHOOTING));
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SHOOTING);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(/*wcex.hInstance*/NULL, MAKEINTRESOURCE(IDI_QUESTION));

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

	HWND hWnd = CreateWindowW(szWindowClass, /*szTitle*/_T("두 번째 윈도우"), WS_OVERLAPPEDWINDOW,
		200, 200, 620, 800, nullptr, nullptr, hInstance, nullptr);

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
	HDC hdc;
	//static int count, yPos, x, y, r3;
	static int mx, my, frame, gameMode;
	//RECT rt = { 0, yPos, 1000, 500 };
	static RECT rectView;
	static int scene, sceneState, cnt;
	static int score;
	/*static TCHAR idStr[4] = L"ABC";
	static vector<Block*> rectCont;*/

	switch (message)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);
		//x = (rectView.left + rectView.right) / 2;
		//y = (rectView.top + rectView.bottom) / 2;
		sceneState = 2;
		cnt = 0;
		score = 0;
		radCannon = 0;
		//frame = 0;
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
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
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...

		//WCHAR str[15] = L"가나다라abcd";
		/*TCHAR str2[15] = _T("가나다라abcd");
		TextOut(hdc, 100, 100, str2, _tcslen(str2));
		RECT rc;
		rc.left = 450;
		rc.top = 0;
		rc.right = 550;
		rc.bottom = 100;
		SetTextColor(hdc, RGB(255, 0, 255));
		/*Rectangle(hdc, rt.left, rt.top, rt.right, rt.bottom);
		DrawText(hdc, str, _tcslen(str), &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		*/

		switch (sceneState)
		{
		case 1:
			startScene(hdc);
			break;
		case 2:
			gameScene(hdc);
			break;
		}

		SetTimer(hWnd, 1, 1000, NULL);
		SetTimer(hWnd, 2, 300, NULL);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			
		}
		InvalidateRgn(hWnd, NULL, TRUE);
	}
	break;
	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1:
			/*POINT tmP;
			for (int i = 0; i < 6; i++)
			{
				int width = (rectView.right - rectView.left) / 6;
				tmP = { width - width/2 + i*width + 2, 90 };
				blockCont.push_back(new Block(tmP, width, 40));
			}*/
			break;
		} // wm_Param switch end
	}
	InvalidateRgn(hWnd, NULL, TRUE);
	break;
	case WM_KEYDOWN:
	{
		switch (sceneState)
		{
		case 1:
			switch (wParam)
			{
			case VK_BACK:
				if (cnt > 0)
				{
					cnt--;
					idStr[cnt] = '\0';
				}
				break;
			case VK_RETURN:
				if (cnt > 0)
					sceneState = 2;
				break;
			}
			if (wParam >= 0x30 && wParam <= 0x5A)
			{
				if (cnt < 3)
				{
					idStr[cnt++] = wParam;
					idStr[cnt] = '\0';
				}
			}
			break;
		case 2:
			switch (wParam)
			{
			case 'A':
				if (radCannon - 3 >= -60)
					radCannon -= 3;
				break;
			case 'D':
				if (radCannon + 3 <= 60)
					radCannon += 3;
				break;
			}
			break;
		}

		InvalidateRgn(hWnd, NULL, TRUE);
	}
	break;
	case WM_CHAR:
	{
		InvalidateRgn(hWnd, NULL, TRUE);
	}
	break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
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

void startScene(HDC hdc)
{
	HFONT hFont, OldFont;
	hFont = CreateFont(100, 60, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
	                    VARIABLE_PITCH | FF_ROMAN, TEXT("HY엽서M"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	//TextOut(hdc, 160, 140, L"Title", lstrlen(L"Title"));
	RECT rt = { 0, 0, 600, 400 };
	DrawText(hdc, L"Title", lstrlen(L"Title"),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	
	hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("HY엽서M"));
	(HFONT)SelectObject(hdc, hFont);
	//TextOut(hdc, 200, 400, L"ID: ", _tcslen(L"ID: "));
	//TextOut(hdc, 300, 400, id, _tcslen(id));
	rt = { 0, 600, 400, 200 };
	DrawText(hdc, L"ID: ", _tcslen(L"ID: "),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rt = { 300, 600, 400, 200 };
	DrawText(hdc, idStr, _tcslen(idStr),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
}


void gameScene(HDC hdc)
{
	HFONT hFont, OldFont;
	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("HY엽서M"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	RECT rt = { 0, 0, 120, 60 };
	DrawText(hdc, idStr, _tcslen(idStr),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	rt = { 120, 0, 500, 60 };
	char buf[64];
	itoa(score, buf, 10);
	TCHAR tBuf[64];
	mbstowcs(tBuf, buf, strlen(buf) + 1);
	DrawText(hdc, tBuf, _tcslen(tBuf),
		&rt, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

	POINT centerCannon = { 300, 750 };
	int lineCannon = 100;
	POINT lineGun = { 20, 200 };
	//radCannon = 30;
	POINT gunP3 = { long(double(centerCannon.x) + lineGun.x*cos(2 * acos(0.0) / 180 * (radCannon))),
		long(double(centerCannon.y) + lineGun.x*sin(2 * acos(0.0) / 180 * (radCannon))) };
	POINT gunP4 = { long(double(centerCannon.x) - lineGun.x*cos(2 * acos(0.0) / 180 * (radCannon))),
		long(double(centerCannon.y) - lineGun.x*sin(2 * acos(0.0) / 180 * (radCannon))) };
	POINT gun[4] =
	{ { long(double(gunP4.x) + lineGun.y*cos(2 * acos(0.0) / 180 * (90 - radCannon))),
	long(double(gunP4.y) - lineGun.y*sin(2 * acos(0.0) / 180 * (90 - radCannon))) },
	{ long(double(gunP3.x) + lineGun.y*cos(2 * acos(0.0) / 180 * (90 - radCannon))),
	long(double(gunP3.y) - lineGun.y*sin(2 * acos(0.0) / 180 * (90 - radCannon))) },
	gunP3, gunP4 };

	Polygon(hdc, gun, 4);
	//Arc(hdc, 200, 600, 400, 800, 400, 800, 200, 800);
	/*Arc(hdc, centerCannon.x - lineCannon, centerCannon.y - lineCannon,
		centerCannon.x + lineCannon, centerCannon.y + lineCannon,
		centerCannon.x + lineCannon, centerCannon.y + lineCannon,
		centerCannon.x - lineCannon, centerCannon.y + lineCannon);*/
	Ellipse(hdc, centerCannon.x - lineCannon,
		centerCannon.y - lineCannon,
		centerCannon.x + lineCannon,
		centerCannon.y + lineCannon);
	


	for (unsigned short i = 0; i < blockCont.size(); i++)
	{
		blockCont[i]->show(hdc);
		blockCont[i]->update();
	}
}


//void startScene()
//{
//
//}









void DrawGrid(HDC hdc, int left, int top, int right, int bottom, int num)
{
	for (int i = 0; i <= ((right - left) / num); i++)
	{
		MoveToEx(hdc, left + i*num, top, NULL);
		LineTo(hdc, left + i*num, bottom);
	}
	for (int i = 0; i <= ((bottom - top) / num); i++)
	{
		MoveToEx(hdc, left, top + i*num, NULL);
		LineTo(hdc, right, top + i*num);
	}
}

void DrawCircle(HDC hdc, double centerX, double centerY, double r1)
{
	Ellipse(hdc, centerX - r1, centerY - r1, centerX + r1, centerY + r1);
}

int DrawFlower(HDC hdc, int centerX, int centerY, int r1, int petal)
{
	DrawCircle(hdc, centerX, centerY, r1);

	double sinT = sin(2 * acos(0.0) / 180 * (180 / petal));
	double r2 = (r1 * sinT) / (1 - sinT);
	for (int i = 0; i < petal; i++)
		DrawCircle(hdc,
			centerX + (r1 + r2)*cos((2 * acos(0.0) / 180 * (90 - (360 / petal)*i))),
			centerY + (r1 + r2)*sin((2 * acos(0.0) / 180 * (90 - (360 / petal)*i))),
			r2);
	return r1 + r2;
}

void DrawStar(HDC hdc, int x, int y, int r)
{
	POINT starP[10];

	for (int i = 0; i < 5; i++)
		starP[0 + 2 * i] =
	{ long(double(x) + r*cos(2 * acos(0.0) / 180 * (90 - 72 * i))),
		long(double(y) - r*sin(2 * acos(0.0) / 180 * (90 - 72 * i))) };

	for (int i = 0; i < 5; i++)
		starP[1 + 2 * i] =
		crammer(starP[(0 + 2 * i) % 10], starP[(4 + 2 * i) % 10],
			starP[(2 + 2 * i) % 10], starP[(8 + 2 * i) % 10]);

	Polygon(hdc, starP, 10);
}

POINT crammer(POINT p1, POINT p2, POINT p3, POINT p4)
{
	long a, b, c, d, e, f;

	a = p1.y - p2.y;
	b = -(p1.x - p2.x);
	c = p3.y - p4.y;
	d = -(p3.x - p4.x);
	/*e = a*p1.x + b*p1.y;
	f = c*p3.x + d*p3.y;*/
	e = a*p4.x + b*p4.y;
	f = c*p3.x + d*p3.y;

	return{ (e*d - b*f) / (a*d - b*c),
		(a*f - e*c) / (a*d - b*c) };
}