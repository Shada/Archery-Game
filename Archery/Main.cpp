#include "stdafx.h"
#pragma comment(lib, "hge.lib")
#pragma comment(lib, "hgehelp.lib")
#include "Bow.h"
#include "Arrow.h"
#include "FileRead.h"
#include "Physics.h"
#include "GameMenu.h"
#include <hgefont.h>

HGE* hge;

const int texSize = 22, nrArrows = 3;

hgeFont *fnt;
HTEXTURE tex[texSize];

hgeSprite *target, *archer, *target_boss, *longbow, *longbowLoaded, *arm, *arrow, *background, *ground, *windArrow,
	*map1, *map2, *map3, *lock, *hole, *scale, *sideArrow, *intitialBackground;

// Handle for a sound effect
HEFFECT snd;

// Some "gameplay" variables and constants

Vektor playerPos = Vektor(0, 80, 600);
Vektor targetPos = Vektor(600, 350, 0);
Vektor target_bossPos = Vektor(0, 1080, 600);
Vektor wind;
Vektor holePos[3];

hgeColor colMap2, colMap3;


float angleUD, angleSTS, scaleFactor, sideArrowX = 600, vSideArrowX = 1.0;
int currentArrow, points;
bool arrowShot, keyState, prevKeyState, showGround, pass, map2Locked = true, map3Locked = true;

GameMenu *gm;

vector<Bow*> bows;
vector<Arrow*> arrows;

Arrow *pArrows[nrArrows];

enum LevelStates
{
	StartScreen,
	FrontView,
	SideView,
	ResultScreen,
	EndScene
};

enum MapState
{
	Map1,
	Map2,
	Map3
};

LevelStates levelState;
MapState state;

void PrepareNextArrow();
void SetValues();
// This function plays collision sound with
// parameters based on sprite position and speed


bool FrameFunc()
{

	// Get the time elapsed since last call of FrameFunc().
	// This will help us to synchronize on different
	// machines and video modes.
	float dt=hge->Timer_GetDelta();
	// Process keys
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	switch(levelState)
	{
	case StartScreen:
		{
			if (hge->Input_GetKeyState(HGEK_1) || (hge->Input_GetKeyState(HGEK_2) && !map2Locked) 
				|| (hge->Input_GetKeyState(HGEK_3) && !map3Locked))
			{
				HTEXTURE tempBackground, tempSurface;
				if(		hge->Input_GetKeyState(HGEK_1)){ state = Map1; tempBackground = tex[0];  tempSurface = tex[8];  scaleFactor = 0.5f;   }
				else if(hge->Input_GetKeyState(HGEK_2)){ state = Map2; tempBackground = tex[10]; tempSurface = tex[11]; scaleFactor = 0.375f; }
				else if(hge->Input_GetKeyState(HGEK_3)){ state = Map3; tempBackground = tex[17]; tempSurface = tex[18]; scaleFactor = 0.250f; }

				ground = new hgeSprite(tempSurface, 0, 0, 1200, 200);
				background = new hgeSprite(tempBackground, 0, 0, 1200, 800);

				levelState = FrontView;

				gm->ChooseBow();
				gm->ChooseArrow();
				for(int i = 0; i < nrArrows; i++)
				{
					pArrows[i] = NULL;
					pArrows[i] = new Arrow(arrows[currentArrowIndex]);
					pArrows[i]->SetPosition(playerPos.x, playerPos.y + 28 * scaleFactor, playerPos.z - 204 * scaleFactor);
				}
			}
			if(map2Locked) map2->SetColor(hgeColorRGB(100,100,100,0).GetHWColor());
			else map2->SetColor(colMap2.GetHWColor()); 
			if(map3Locked) map3->SetColor(hgeColorRGB(100,100,100,0).GetHWColor());
			else map3->SetColor(colMap3.GetHWColor()); 

			if(hge->Input_GetKeyState(HGEK_SPACE)) { map2Locked = false; map3Locked = false; }
			break;
		}
	case FrontView:
		{
			sideArrowX += vSideArrowX;
			if(sideArrowX >= 720) vSideArrowX = -vSideArrowX;
			if(sideArrowX <= 480) vSideArrowX = -vSideArrowX;

			if((keyState = hge->Input_GetKeyState(HGEK_SPACE)) && !prevKeyState)
			{
				angleSTS = (sideArrowX - 600) / 12;
				levelState = SideView;
				showGround = true;
				Sleep(500);
			}
			prevKeyState = keyState;
			break;
		}
	case SideView:
		{
			if(!arrowShot)
			{
				//angle down
				if (hge->Input_GetKeyState(HGEK_DOWN) && angleUD > -70) 
				{
					angleUD -= 0.02F;
					pArrows[currentArrow]->SetPosition(playerPos.x, playerPos.y + 28 * scaleFactor, playerPos.z - 204 * scaleFactor);
					pArrows[currentArrow]->SetAngle(Physics::DegreeToRad(-angleUD));		
				}
				//angle up
				if (hge->Input_GetKeyState(HGEK_UP) && angleUD < 70)
				{
					angleUD += 0.02F;
					pArrows[currentArrow]->SetPosition(playerPos.x, playerPos.y + 28 * scaleFactor, playerPos.z - 204 * scaleFactor);
					pArrows[currentArrow]->SetAngle(Physics::DegreeToRad(-angleUD));		
				}
				//Shoot arrow
				if ((keyState = hge->Input_GetKeyState(HGEK_SPACE)) && !prevKeyState)
				{
					//Set initial arrow speed
					pArrows[currentArrow]->SetVelocity(Physics::ArrowInitialVelocity(bows[currentBowIndex], 
														pArrows[currentArrow]->GetWeight(), angleUD, angleSTS));
					arrowShot = true;
					dt = 0;
				}
			}
			else
			{
				pArrows[currentArrow]->Update(dt);
				if(pArrows[currentArrow]->HitGround())
				{
					PrepareNextArrow();
				}
				if(pArrows[currentArrow]->ReachTarget() && !pass) 
				{
					if(pArrows[currentArrow]->HitTarget())
						PrepareNextArrow();
					else
						pass = true;
				}
			}
			prevKeyState = keyState;
			break;
		}

	case ResultScreen:
		{
			//cheating!!
			if(hge->Input_GetKeyState(HGEK_E)) points += 10;
			Sleep(2000);

			levelState = EndScene;
			break;
		}


	case EndScene:
		{			
			if(hge->Input_GetKeyState(HGEK_ENTER))
			{
				GameMenu::UpdateBList(points);
				GameMenu::UpdateAList(points);

				if(points >= 10)
				{
					if(state == Map2) map3Locked = false;
					else map2Locked = false;
				}

				SetValues();
				levelState = StartScreen;
			}
			
			break;
		}

	}
	return false;
}

void PrepareNextArrow()
{
	arrowShot = false;
	pass = false;
	showGround = false;

	//stop the arrow
	pArrows[currentArrow]->Stop();

	holePos[currentArrow] = Vektor(600 + pArrows[currentArrow]->xhit, 0, 350 + pArrows[currentArrow]->zhit);

	//go to next arrow
	currentArrow++;
	angleUD = 0.0f;
	angleSTS = 0.0f;

	//set new wind
	wind = Physics::Wind();
	Sleep(500);

	//no more arrows -> Endscene
	if(currentArrow >= nrArrows) { levelState = ResultScreen; return; }
	//go to frontview
	levelState = FrontView;
}

void SetValues()
{
	points = 0;
	angleUD = 0.0f;
	angleSTS = 0.0f;
	currentArrow = 0;

	arrowShot = false;
	showGround = true;
	prevKeyState = false;
}

// This function will be called by HGE when
// the application window should be redrawn.
// Put your rendering code here.
bool RenderFunc()
{
	// Begin rendering quads.
	// This function must be called
	// before any actual rendering.
	hge->Gfx_BeginScene();
	
	// Clear screen with black color
	hge->Gfx_Clear(0);
	
	if(levelState != StartScreen && levelState != EndScene)
		background->Render(0, 0);
	else
		intitialBackground->RenderEx(100, 200, 0.0f, 1.0f, 0.0f);	
	
	switch(levelState)
	{
	case StartScreen:
		{
			showGround = false;
			map1->RenderEx(800, 50, 0.0f, 0.3f, 0.0f);
			map2->RenderEx(800, 250, 0.0f, 0.3f, 0.0f);
			map3->RenderEx(800, 450, 0.0f, 0.3f, 0.0f);
			if(map2Locked) lock->RenderEx(940, 300, 0.0f, 0.1f, 0.0f);
			if(map3Locked) lock->RenderEx(940, 500, 0.0f, 0.1f, 0.0f);
			break;
		}
	case FrontView:
		{
			windArrow->RenderEx(950, 100, Physics::WindAngle(wind), 0.6f, 0.0f);
			fnt->printf(950, 160, HGETEXT_CENTER, "%g m/s", Physics::Round(wind.Length()));
			target->Render(targetPos.x, targetPos.y);
			for(int i = 0; i < 3; i++) 
			{
				if(pArrows[i]->hit)
					hole->RenderEx(holePos[i].x, holePos[i].z, 0.0f, 0.2f, 0.0f);
			}
			scale->RenderEx(480, 550, 0.0f, 0.8f, 0.0f);
			sideArrow->RenderEx(sideArrowX, 570, 0.0f, 0.7f, 0.0f);
			
			break;
		}
	case SideView:
		{
			
			//skriv ut text
			fnt->printf(60,10,HGETEXT_LEFT, "Current Arrow:%f\nLength:%f\nHeight:%f\nDepth:%f\nAngle:%f", 
										(float)currentArrow, (pArrows[currentArrow]->GetPosition().y - 80) / (200 * scaleFactor),
										(550 - pArrows[currentArrow]->GetPosition().z) / (200 * scaleFactor), 
										pArrows[currentArrow]->GetPosition().x / (200 * scaleFactor),
										angleUD);
	
			//render archer.
			archer->RenderEx(playerPos.y, playerPos.z, 0.0f, scaleFactor, 0.0F);
			arm->RenderEx(playerPos.y + 36 * scaleFactor, playerPos.z - 210 * scaleFactor, Physics::DegreeToRad(-angleUD), scaleFactor, 0.0F);
	
	
			//render the arrow that have been shot, and is loaded.
			for(int i = 0; i < currentArrow + 1; i++)
			{
				pArrows[i]->Render(arrow);
			}

			//render boss
			target_boss->RenderEx(target_bossPos.y, target_bossPos.z, 0.0f, scaleFactor * 2, 0.0f);


			//render bow
			if(!arrowShot)
				longbowLoaded->RenderEx(playerPos.y + 44 * scaleFactor + ((arm->GetWidth() - 27) * scaleFactor) * sin(Physics::DegreeToRad(90 + angleUD) ), 
										playerPos.z - 204 * scaleFactor + ((arm->GetWidth() - 22) * scaleFactor) * cos(Physics::DegreeToRad(90 + angleUD) ), 
										Physics::DegreeToRad(-angleUD), scaleFactor, 0.0f);
			else
				longbow->RenderEx(playerPos.y + 42 * scaleFactor + ((arm->GetWidth() - 25) * scaleFactor) * sin(Physics::DegreeToRad(90 + angleUD)), 
										playerPos.z - 198 * scaleFactor + ((arm->GetWidth() - 22) * scaleFactor) * cos(Physics::DegreeToRad(90 + angleUD)), 
										Physics::DegreeToRad(-angleUD), scaleFactor, 0.0f);
			break;

		}
	case ResultScreen:
		{
			target->Render(targetPos.x, targetPos.y);
			for(int i = 0; i < 3; i++) 
			{
				if(pArrows[i]->hit)
					hole->RenderEx(holePos[i].x, holePos[i].z, 0.0f, 0.2f, 0.0f);
			}
			break;
		}

	case EndScene:
		{
			fnt->printf(700, 50, HGETEXT_LEFT, "You scored %i points which means you have: ", points);
			if(points > 9 && state == Map1 && map2Locked)
			{
				if(points > 13)
					fnt->printf(700, 100, HGETEXT_LEFT, "Unlocked 2 new bows \n1 new arrow \nand 1 new map!!\n   Well done! ");
				else
					fnt->printf(700, 100, HGETEXT_LEFT, "Unlocked 1 new bow \n1 new arrow \nand 1 new map!!\n   Not bad at all! ");
			}
			else if(points > 9 && state == Map2 && map3Locked)
			{
				if(points > 13)
					fnt->printf(700, 100, HGETEXT_LEFT, "Unlocked 2 new bows \n1 new arrow \nand 1 new map!!\n   Well done! ");
				else
					fnt->printf(700, 100, HGETEXT_LEFT, "Unlocked 1 new bow \n1 new arrow \nand 1 new map!!\n   Not bad at all! ");
			}
			else if(points > 8)
			{
				if (points > 13)
					fnt->printf(700, 100, HGETEXT_LEFT, "Unlocked 2 new bows \n1 new arrow\n   Well done! ");
				else
					fnt->printf(700, 100, HGETEXT_LEFT, "Unlocked 1 new bow \nand 1 new arrow\n   Nice! ");
			}
			else if(points > 6)
				fnt->printf(700, 100, HGETEXT_LEFT, "Unlocked 1 new bow\n   Good! ");
			else
				fnt->printf(700, 100, HGETEXT_LEFT, "Not actually unlocked anything new\n   Too bad! ");
			//Render points and achievments?
		}
	};
	if(showGround) ground->Render(0, 600);
	// End rendering and update the screen
	hge->Gfx_EndScene();

	// RenderFunc should always return false
	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// Here we use global pointer to HGE interface.
	// Instead you may use hgeCreate() every
	// time you need access to HGE. Just be sure to
	// have a corresponding hge->Release()
	// for each call to hgeCreate()
	hge = hgeCreate(HGE_VERSION);

	// Set up log file, frame function, render function and window title
	hge->System_SetState(HGE_LOGFILE, "hge_tut02.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Little Archer Boy");

	// Set up video mode
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_SHOWSPLASH, false);


	if(hge->System_Initiate())
	{
		// Load bows and arrows from files
		FileRead::Read(bows, arrows);
		scaleFactor = 0.5f;
		for(int i = 0; i < nrArrows; i++)
		{
			pArrows[i] = new Arrow(arrows[0]);
			pArrows[i]->SetPosition(playerPos.x, playerPos.y + 28 * scaleFactor, playerPos.z - 204 * scaleFactor);
			
		}

		state = Map1;
		levelState = StartScreen;
		GameMenu::UpdateBList(100);
		GameMenu::UpdateAList(999);
		// Load textures
		tex[0] = hge->Texture_Load("Background.png");
		tex[1] = hge->Texture_Load("target.png");
		tex[2] = hge->Texture_Load("boss.png");
		tex[3] = hge->Texture_Load("longbow1.png");
		tex[4] = hge->Texture_Load("longbow1_loaded.png");
		tex[5] = hge->Texture_Load("arm.png");
		tex[6] = hge->Texture_Load("archer_no_arm.png");
		tex[7] = hge->Texture_Load("arrow.png");
		tex[8] = hge->Texture_Load("grass.png");
		tex[9] = hge->Texture_Load("direction_arrow.png");
		tex[10] = hge->Texture_Load("stormy_background.png");
		tex[11] = hge->Texture_Load("stone.png");
		tex[12] = hge->Texture_Load("map1.png");
		tex[13] = hge->Texture_Load("map2.png");
		tex[14] = hge->Texture_Load("map3.png");
		tex[15] = hge->Texture_Load("archery_background.png");
		tex[16] = hge->Texture_Load("lock.png");
		tex[17] = hge->Texture_Load("sand_background.png");
		tex[18] = hge->Texture_Load("dirt.png");
		tex[19] = hge->Texture_Load("hole.png");
		tex[20] = hge->Texture_Load("side_to_side.png");
		tex[21] = hge->Texture_Load("scale.png");
		
		map1 = new hgeSprite(tex[12], 0, 0, 1180, 662);
		map2 = new hgeSprite(tex[13], 0, 0, 1180, 662);
		map3 = new hgeSprite(tex[14], 0, 0, 1180, 662);
		intitialBackground = new hgeSprite(tex[15], 0, 0, 558, 432);
		lock = new hgeSprite(tex[16], 0, 0, 1000, 1000);
		hole = new hgeSprite(tex[19], 0, 0, 79, 79);
		sideArrow = new hgeSprite(tex[20], 0, 0, 14, 116);
		scale = new hgeSprite(tex[21], 0, 0, 300, 50);
		background = new hgeSprite(tex[0], 0, 0, 1200, 800);
		target = new hgeSprite(tex[1], 0, 0, 600, 600);
		target_boss = new hgeSprite(tex[2], 0, 0, 150, 222);
		longbow = new hgeSprite(tex[3], 0, 0, 45, 315);
		longbowLoaded = new hgeSprite(tex[4], 0, 0, 140, 275);
		arm = new hgeSprite(tex[5], 0, 0, 120, 47);
		archer = new hgeSprite(tex[6], 0, 0, 177, 360);
		arrow = new hgeSprite(tex[7], 0, 0, 183, 16);
		ground = new hgeSprite(tex[8], 0, 0, 1200, 300);
		windArrow = new hgeSprite(tex[9], 0, 0, 208, 74);


		//set hotspots
		sideArrow->SetHotSpot(sideArrow->GetWidth() / 2, 0);
		hole->SetHotSpot(hole->GetWidth() / 2, hole->GetHeight() / 2);
		target->SetHotSpot(target->GetWidth() / 2, target->GetHeight() / 2);
		arrow->SetHotSpot(25, arrow->GetHeight() / 2);
		archer->SetHotSpot(90, archer->GetHeight());
		windArrow->SetHotSpot(101, 35);
		arm->SetHotSpot(5, arm->GetHeight() / 2);
		longbow->SetHotSpot(longbow->GetWidth() - 5, longbow->GetHeight() / 2);
		longbowLoaded->SetHotSpot(longbowLoaded->GetWidth() - 5, longbowLoaded->GetHeight() / 2);
		target_boss->SetHotSpot(7, target_boss->GetHeight());

		//original colors
		colMap2 = map2->GetColor();
		colMap3 = map3->GetColor();

		//load font
		fnt = new hgeFont("font1.fnt");
		hgeColorRGB col = hgeColorRGB(255,100,150,0);
		fnt->SetColor(col.GetHWColor());
		fnt->SetScale(0.8f);
		
		gm = new GameMenu(bows, arrows);

		// Let's rock now!
		hge->System_Start();

		// Free loaded texture and sound
		for(int i = 0; i < texSize; i++)
		{
			hge->Texture_Free(tex[i]);
		}
	}
	else MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}