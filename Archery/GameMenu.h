#pragma once

#include "stdafx.h"
#include "Bow.h"
#include "Arrow.h"

class GameMenu
{
private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	int RunWindow();
public:
	GameMenu(vector<Bow*> &bows, vector<Arrow*> &arrows);
	
	
	void ChooseBow();
	
	void ChooseArrow();

	
	static void UpdateBList(int score);
	static void UpdateAList(int score);

	~GameMenu();
};

