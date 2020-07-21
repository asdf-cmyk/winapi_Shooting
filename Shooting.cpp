// Shooting.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Shooting.h"
#include "Block.h"
#include "Life.h"
#include "Cannon.h"
#include "Bullet.h"
#include <vector>
#include <random>
#include <memory>
#include <fstream>

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

int mkRand(int range)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(1, range);
	return dist(mt);
}
void init(HWND);
void startScene(HDC);
void startToGame();
void gameScene(HDC, HWND);
void gameToEnd();
void endScene(HWND, HDC);
void timer1(HWND);
void timer2(HWND);
void timer3(HWND);
void quit(HWND);
//void CALLBACK timer1(HWND, UINT, UINT, DWORD);
//void CALLBACK timer2(HWND, UINT, UINT, DWORD);
//void CALLBACK timer3(HWND, UINT, UINT, DWORD);

static int scene, sceneState, cnt;
static TCHAR idStr[4];
static int score = 0;
static int radCannon = 0;
static int timer1T;
static string ranking[3];
static int scoreRanking[3];

vector<Block*> blockCont;
vector<Life*> lifeCont;
Cannon cannon;
vector<Bullet*> bulletCont;
RECT rectView;


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

	HWND hWnd = CreateWindowW(szWindowClass, /*szTitle*/_T("µÎ ¹øÂ° À©µµ¿ì"), WS_OVERLAPPEDWINDOW,
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
	static int mx, my, frame, gameMode;
	//static RECT rectView;
	//static int scene, sceneState, cnt;
	/*static TCHAR idStr[4] = L"ABC";
	static vector<Block*> rectCont;*/
	//static int timer2Flag = 0;
	//static int radFlag = 0;
	//static string rank;

	switch (message)
	{
	case WM_CREATE:
		init(hWnd);
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

		switch (sceneState)
		{
		case 1:
			startScene(hdc);
			break;
		case 2:
			gameScene(hdc, hWnd);
			break;
		case 3:
			endScene(hWnd, hdc);
			break;
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			RECT rtRetry = { 0, 200, 400, 700 };
			RECT rtQuit = { 300, 200, 500, 700 };
			if (sceneState == 3)
			{
				if (mx > rtRetry.left && mx < rtRetry.right &&
					my < rtRetry.bottom &&  my > rtRetry.top)
				{
					startToGame();
					sceneState = 2;
				}

				if (mx > rtQuit.left && mx < rtQuit.right &&
					my < rtQuit.bottom &&  my > rtQuit.top)
					quit(hWnd);
			}
		}
		InvalidateRgn(hWnd, NULL, TRUE);
	}
	break;
	case WM_TIMER:
	{
		if (sceneState == 2)
			switch (wParam)
			{
			case 1:
				timer1(hWnd);
				break;
			case 2:
				timer2(hWnd);
				break;
			case 3:
				timer3(hWnd);
				break;
			}
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
					startToGame();
					//sceneState = 2;
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
				if (radCannon - 3 >= -60/* && timer2Flag*/)
				{
					radCannon -= 3;
					//radFlag = -1;
					//timer2Flag = 0;
				}
				break;
			case 'D':
				if (radCannon + 3 <= 60 /* && timer2Flag*/)
				{
					radCannon += 3;
					//radFlag = 1;
					//timer2Flag = 0;
				}
				break;
			case VK_RETURN:
				gameToEnd();
				//sceneState = 3;
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
		quit(hWnd);
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


void init(HWND hWnd)
{
	GetClientRect(hWnd, &rectView);
	sceneState = 1;
	cnt = 0;
	score = 0;
	radCannon = 0;
	timer1T = 500;

	ifstream readRank("rank.txt");
	int i = 0;
	while (!readRank.eof())
	{
		getline(readRank, ranking[i]);
		i++;
	}
	readRank.close();

	/*POINT tmP;
	for (int i = 0; i < 6; i++)
	{
		int width = (rectView.right - rectView.left) / 6;
		tmP = { width - width / 2 + i*width + 2, 720 };
		lifeCont.push_back(new Life(tmP, width, 40));
	}*/
	SetTimer(hWnd, 1, 1000, NULL);
	SetTimer(hWnd, 2, 750, NULL);
	SetTimer(hWnd, 3, 60, NULL);
}

void startScene(HDC hdc)
{
	HFONT hFont, OldFont;
	hFont = CreateFont(100, 60, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("HY¿±¼­M"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	//TextOut(hdc, 160, 140, L"Title", lstrlen(L"Title"));
	RECT rt = { 0, 0, 600, 400 };
	DrawText(hdc, L"Title", lstrlen(L"Title"),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("HY¿±¼­M"));
	(HFONT)SelectObject(hdc, hFont);
	//TextOut(hdc, 200, 400, L"ID: ", _tcslen(L"ID: "));
	//TextOut(hdc, 300, 400, id, _tcslen(id));
	rt = { 0, 200, 400, 600 };
	DrawText(hdc, L"ID: ", _tcslen(L"ID: "),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rt = { 280, 200, 420, 600 };
	DrawText(hdc, idStr, _tcslen(idStr),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
}

void startToGame()
{
	score = 0;
	radCannon = 0;
	timer1T = 500;
	POINT tmP;
	for (int i = 0; i < 6; i++)
	{
		int width = (rectView.right - rectView.left) / 6;
		tmP = { width - width / 2 + i*width + 2, 720 };
		lifeCont.push_back(new Life(tmP, width, 40));
	}

	sceneState = 2;
}

void gameScene(HDC hdc, HWND hWnd)
{
	HFONT hFont, OldFont;
	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("HY¿±¼­M"));
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

	cannon.update(radCannon);
	cannon.show(hdc);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	for (unsigned short i = 0; i < lifeCont.size(); i++)
		lifeCont[i]->show(hdc);
	SelectObject(hdc, OldBrush);
	DeleteObject(hBrush);


	for (unsigned short i = 0; i < bulletCont.size(); i++)
		bulletCont[i]->show(hdc);
	for (unsigned short i = 0; i < blockCont.size(); i++)
		blockCont[i]->show(hdc);
}

void gameToEnd()
{
	bool rankFlag = 0;
	int flagI = -1;
	for (int i = 2; i >= 0; i--)
	{
		string tmpScore = ranking[i].substr(7, ranking[i].size());
		scoreRanking[i] = stoi(tmpScore);
		if (score >= scoreRanking[i])
			flagI = i;
	}

	if (flagI != -1)
	{
		for (int i = 0; i < 2 - flagI; i++)
		{
			ranking[2 - i - 1][0]++;
			ranking[2 - i] = ranking[2 - i - 1];
		}
		char buf[16];
		sprintf(buf, "%d. %ls %d", flagI+1, idStr, score);
		ranking[flagI] = buf;
	}

	ofstream writeRank("rank.txt");
	for (int i = 0; i<3; i++)
	{
		writeRank << ranking[i];
		writeRank << endl;
	}
	writeRank.close();


	for (unsigned short i = 0; i < blockCont.size(); i++)
		delete blockCont[i];
	for (unsigned short i = 0; i < bulletCont.size(); i++)
		delete bulletCont[i];
	for (unsigned short i = 0; i < lifeCont.size(); i++)
		delete lifeCont[i];
	blockCont.clear();
	bulletCont.clear();
	lifeCont.clear();

	sceneState = 3;
}

void endScene(HWND hWnd, HDC hdc)
{
	HFONT hFont, OldFont;
	hFont = CreateFont(50, 30, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("HY¿±¼­M"));
	OldFont = (HFONT)SelectObject(hdc, hFont);
	RECT rt = { 0, 0, 600, 200 };
	for (int i = 0; i < 3; i++)
	{
		int len;
		int slength = (int)ranking[i].length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, ranking[i].c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, ranking[i].c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		
		DrawText(hdc, r.c_str(), lstrlen(r.c_str()),
			&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rt.bottom += 200;
	}

	hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("HY¿±¼­M"));
	(HFONT)SelectObject(hdc, hFont);
	rt = { 0, 200, 400, 700 };
	DrawText(hdc, L"RETRY", _tcslen(L"RETRY"),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	rt = { 300, 200, 500, 700 };
	DrawText(hdc, L"QUIT", _tcslen(L"QUIT"),
		&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
}

void timer1(HWND hWnd)
{
	POINT tmP;
	int width = (rectView.right - rectView.left) / 6;
	tmP = { width - width / 2 + (mkRand(6) - 1)*width + 2, 90 };
	blockCont.push_back(new Block(tmP, width, 40, timer1T));

	for (unsigned short i = 0; i < blockCont.size(); i++)
	{
		if (blockCont.size() > 0)
		{
			blockCont[i]->update();
		}
	}
	InvalidateRgn(hWnd, NULL, TRUE);
}

void timer2(HWND hWnd)
{
	bulletCont.push_back(new Bullet(cannon, rectView));

	for (unsigned short i = 0; i < bulletCont.size(); i++)
	{
		if (bulletCont.size() > 0)
		{
			bulletCont[i]->update();
			//bulletCont[i]->show(hdc);
		}
	}
	InvalidateRgn(hWnd, NULL, TRUE);
}

void timer3(HWND hWnd)
{
	for (unsigned short i = 0; i < blockCont.size(); i++)
	{
		//blockCont[i]->collision();
		if (!(blockCont[i]->getIsExist()))
		{
			delete blockCont[i];
			blockCont.erase(blockCont.begin() + i);
		}
	}
	for (unsigned short i = 0; i < lifeCont.size(); i++)
	{
		lifeCont[i]->collision(blockCont);
		if (!(lifeCont[i]->getIsExist()))
		{
			delete lifeCont[i];
			lifeCont.erase(lifeCont.begin() + i);
			if (lifeCont.size() < 1)
			{
				gameToEnd();
			}
		}
	}
	for (unsigned short i = 0; i < bulletCont.size(); i++)
	{
		bulletCont[i]->collision(blockCont, score);
		if (!(bulletCont[i]->getIsExist()))
		{
			delete bulletCont[i];
			bulletCont.erase(bulletCont.begin() + i);
		}
	}
	//InvalidateRgn(hWnd, NULL, TRUE);
}

void quit(HWND hWnd)
{
	KillTimer(hWnd, 1);
	KillTimer(hWnd, 2);
	KillTimer(hWnd, 3);
	for (unsigned short i = 0; i < blockCont.size(); i++)
		delete blockCont[i];
	for (unsigned short i = 0; i < bulletCont.size(); i++)
		delete bulletCont[i];
	for (unsigned short i = 0; i < lifeCont.size(); i++)
		delete lifeCont[i];

	PostQuitMessage(0);
}