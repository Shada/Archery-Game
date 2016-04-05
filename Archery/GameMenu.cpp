#include "GameMenu.h"

HWND hLabel, hListBox, hButton;
char *text;
string result, name;
extern vector<Bow*> bows; extern vector<Arrow*> arrows;
bool bow = false, arrow = false;
int currentBowIndex = 0, currentArrowIndex = 0, index, length, availableBows = 0, availableArrows = 0;

GameMenu::GameMenu(vector<Bow*> &b, vector<Arrow*> &a)
{
	bows = b;
	arrows = a;
}
GameMenu::~GameMenu(){}

void GameMenu::ChooseBow()
{
	bow = true;
	GameMenu::RunWindow();
	bow = false;
}
void GameMenu::ChooseArrow()
{
	arrow = true;
	GameMenu::RunWindow();
	arrow = false;
}


int GameMenu::RunWindow()
{
	HINSTANCE hInstance = hInst;
	
	MSG  msg;    
	WNDCLASS wc = {0};
	wc.lpszClassName = TEXT("Archery");
	wc.hInstance     = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc   = (WNDPROC)WndProc;
	wc.hCursor       = LoadCursor(0, IDC_ARROW);

	RegisterClass(&wc);
	CreateWindow(wc.lpszClassName, TEXT("Decision box"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		150, 150, 230, 150, 0, 0, hInstance, 0);

	while(GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}

void GameMenu::UpdateBList(int score)
{
	while(score >= 7)
	{
		if(availableBows == (int)bows.size()) break;
		score -= 7;
		availableBows++;
	}
	string sBuffer;
	char * buffer = new char[100];
	for(int c = 0; c < availableBows; c++)
	{
		sBuffer = bows[c]->GetName();
		SendMessage(hListBox, LB_INSERTSTRING, NULL, (LPARAM)(strcpy(buffer, sBuffer.c_str())));
	}	
}
void GameMenu::UpdateAList(int score)
{
	while(score >= 9)
	{
		if(availableArrows == (int)arrows.size()) break;
		score -= 9;
		availableArrows++;
	}
	
	string sBuffer;
	char * buffer = new char[100];
	for(int c = 0; c < availableArrows; c++)
	{
		sBuffer = arrows[c]->GetName();
		SendMessage(hListBox, LB_INSERTSTRING, NULL, (LPARAM)(strcpy(buffer, sBuffer.c_str())));
	}
}


LRESULT CALLBACK GameMenu::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)  
	{
	case WM_CREATE:
	{
		hListBox = CreateWindowEx(WS_EX_CLIENTEDGE
		, "LISTBOX", "listbox"
		, WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL
		, 7 , 35, 200, 270
		, hwnd, (HMENU)1, hInst, NULL);

		hLabel = CreateWindowExW(WS_EX_CLIENTEDGE
		, L"STATIC", NULL
		, WS_CHILD | WS_VISIBLE
		, 14, 7, 182, 21
		, hwnd, NULL, hInst, NULL);

		hButton = CreateWindowExW(WS_EX_CLIENTEDGE,
		L"button", L"Select"
		, WS_CHILD | WS_VISIBLE
		, 70 , 301, 80, 30
		, hwnd, (HMENU)2, hInst, NULL);

		if(bow)
			UpdateBList(0);
		if(arrow)
			UpdateAList(0);

		ShowWindow(hButton, SW_SHOW);
		ShowWindow(hListBox, SW_SHOW);

		SetWindowPos(hwnd, NULL, 0, 0, 230, 375, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		if(bow)
			SetWindowTextW(hLabel, L"  List of all avaliable bows");
		else if(arrow)
			SetWindowTextW(hLabel, L" List of all avaliable arrows");

		UpdateWindow(hButton);
	}

	case WM_COMMAND:
	{
		switch(wParam)
		{
		case 2:
			{
				index = (int) SendMessage(hListBox, LB_GETCURSEL, 0, 0);

				length = SendMessage(hListBox, LB_GETTEXTLEN, (WPARAM)index, 0);
				text = new char[length + 1];
				
				SendMessage(hListBox, LB_GETTEXT, (WPARAM)index, (LPARAM)text); 

				if(bow)
				{
					for(int c = 0; c < (int)bows.size(); c++)
					{
						if(bows[c]->Search((string)text))
						{
							currentBowIndex = c;
							break;
						}
					}
				}

				else if(arrow)
				{
					for(int c = 0; c < (int)arrows.size(); c++)
					{
						if(arrows[c]->Search((string)text))
						{
							currentArrowIndex = c;
							break;
						}
					}
				}

				result = (string)text;
				
				HWND pHwnd;
	            pHwnd = FindWindow("Archery", NULL);
	            SendMessage(pHwnd, WM_CLOSE, 0, 0);
	            break;
			}
		}
		break;
	}
	
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			
			break;
		} 
	case WM_ACTIVATE:
		{
			break;
		}
	}
	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}