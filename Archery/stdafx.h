#ifndef _STDAFX__H
#define _STDAFX__H

#include <hge.h>
#include <hgesprite.h>
#include <hgecolor.h>
#include <hgeparticle.h>

#include <ctime>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <cstring>
#include <cmath>


using namespace std;

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	700

#define hInst	GetModuleHandle(NULL)

#define GRAV	982

#define NULL 0

#define SAFE_DELETE(x) { if(x) { delete x; x = NULL; } }

using std::vector;

extern HGE* hge;
extern int currentBowIndex;
extern int currentArrowIndex;
extern float scaleFactor;
extern int points;

#endif
