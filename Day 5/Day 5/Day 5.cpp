// Day 5.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Day 5.h"

#define MAX_LOADSTRING 100
#define ID_SOLVE_BUTTON 1

using namespace std;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND editLabel;

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DAY5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DAY5));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DAY5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DAY5);
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
      CW_USEDEFAULT, 0, 340, 160, nullptr, nullptr, hInstance, nullptr);

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
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

struct CharacterListType
{
	wchar_t character = 0;
	CharacterListType* prev = NULL;
	CharacterListType* next = NULL;
};

CharacterListType* TryRemoveCharacter(CharacterListType*& charList)
{
	if (charList->prev && charList)
	{
		CharacterListType* returnNext = charList->next;

		if (charList->prev->character != charList->character && tolower(charList->prev->character) == tolower(charList->character))
		{
			CharacterListType* prevPrev = charList->prev->prev;
			if (prevPrev)
			{
				prevPrev->next = charList->next;
			}

			if (charList->next)
			{
				charList->next->prev = prevPrev;
			}

			delete charList->prev;
			charList->prev = NULL;

			delete charList;
			charList = NULL;

			if (prevPrev)
			{
				return TryRemoveCharacter(prevPrev);
			}
		}

		return returnNext;
	}

	if (charList)
	{
		return charList->next;
	}

	return NULL;
}

int Solve1(CharacterListType* charList)
{
	wchar_t prev = 0;
	int result = 0;

	int index = 0;
	vector<int> deleteIndices;

	CharacterListType* currentMember = charList;

	while (currentMember)
	{
		CharacterListType* next = TryRemoveCharacter(currentMember);
		currentMember = next;
	}

	currentMember = charList;

	while (currentMember)
	{
		result++;
		currentMember = currentMember->next;
	}

	return result - 1;
}

void DeleteList(CharacterListType* charList)
{
	while (charList)
	{
		CharacterListType* next = charList->next;

		delete charList;
		charList = next;
	}
}

int Solve2(wchar_t* string, int length)
{
	int minValue = length;

	for (wchar_t k = 'a'; k <= 'z'; k++)
	{
		CharacterListType* charList = new CharacterListType();
		CharacterListType* prev = charList;

		charList->character = 0;

		for (int i = 0; i < length; i++)
		{
			if (tolower(string[i]) != k)
			{
				CharacterListType* newCharacter = new CharacterListType();
				newCharacter->character = string[i];

				newCharacter->prev = prev;
				prev->next = newCharacter;

				prev = newCharacter;
			}
		}

		int value = Solve1(charList);

		if (value < minValue)
			minValue = value;

		DeleteList(charList);
	}

	return minValue;
}

void Solve()
{
	CharacterListType* charList = new CharacterListType();
	CharacterListType* prev = charList;

	int length = GetWindowTextLength(editLabel);
	LPWSTR string = new wchar_t[length + 1];
	GetWindowText(editLabel, string, length + 1);

	charList->character = 0;

	for (int i = 0; i < length; i++)
	{
		CharacterListType* newCharacter = new CharacterListType();
		newCharacter->character = string[i];

		newCharacter->prev = prev;
		prev->next = newCharacter;

		prev = newCharacter;
	}

	int result1 = Solve1(charList);

	wchar_t displayBuffer1[1000] = { NULL };
	wchar_t displayBuffer2[1000] = { NULL };

	wsprintf(displayBuffer1, L"Result 1: %d", result1);
	MessageBox(NULL, displayBuffer1, L"Result 1", NULL);

	DeleteList(charList);

	int result2 = Solve2(string, length);

	wsprintf(displayBuffer2, L"Result 2: %d", result2);
	MessageBox(NULL, displayBuffer2, L"Result 2", NULL);

	delete[] string;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		HWND hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ESB_ENABLE_BOTH | ES_MULTILINE, 20, 20, 280,
			40, hWnd, NULL, NULL, NULL);

		SendMessage(hWndEdit, EM_SETLIMITTEXT, 60000, 0);

		editLabel = hWndEdit;

		HWND hwndButton = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed 
			L"Solve",      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			20,         // x position 
			60,         // y position 
			100,        // Button width
			20,        // Button height
			hWnd,     // Parent window
			(HMENU)ID_SOLVE_BUTTON,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
	}
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
			case ID_SOLVE_BUTTON:
				Solve();
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
            // TODO: Add any drawing code that uses hdc here..

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
