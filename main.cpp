#include "DxLib.h"
#include "ajikugames.h"
#include "LiveEntity.h"
#include <stdlib.h>
#include <time.h>
#include "PARTICLE.h"

const char TITLE[] = "GC1B_02_イサザリック_Mushy-Mush-Mush";

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	SetWindowSizeChangeEnableFlag(TRUE, TRUE);
	SetMainWindowText(TITLE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32); // 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1);
	SetBackgroundColor(20, 20, 30);	// 画面の背景色を設定するx
	if (DxLib_Init() == -1) { return -1; }
	SetDrawScreen(DX_SCREEN_BACK);
	//SetMouseDispFlag(FALSE);
	// ESSENTIAL VARIABLES
	int frame = 0, pal = 0;
	int fadefromblackflag = FALSE, fadetoblackflag = FALSE;
	int shakecount = 0; 
	bool shakeflag = FALSE;
	bool flag = FALSE;
	int shakex = 0;
	int shakey = 0;
	int mousex = 0, mousey = 0;
	srand(time(NULL));
	// AJKGames Variables
	logo AJKGames;
	AJKGames.framecount = 0, AJKGames.index = 0, AJKGames.logosound = LoadSoundMem("ajikugamessound.wav");
	LoadDivGraph("ajikugameslogo.png", 4, 2, 2, 640/2, 256/2, AJKGames.logo, true);
	

	// Player Related

	// Images
	int engineLOGO = LoadGraph("Dandy-OEngine.png", true);
	int sprites[28];
	LoadDivGraph("Resource\\mushymushmushallsprites.png",
		28, 7, 4, 32, 32, sprites, true);
	int title = LoadGraph("Resource\\title.png");
	int selector = LoadGraph("Resource\\selector.png");
	int keys[8];
	LoadDivGraph("Resource\\keys.png", 8, 2, 4, 64, 64, keys, true);
	int winpng = LoadGraph("Resource\\winscreen.png");

	// Attacks / Skills

	// Cam
	int camX = 0, camY = 0;

	const enum scenstates {
		openinglogo,
		enginelogo,
		menu,
		reset,
		load,
		gameplay,
		win,
		lose,
	};

	enum enemystates {
		idle,
		walkleft,
		walkright,
		shootleft,
		shootright,
		falling,
		dead,
	};

	enum ball_states {
		inactive,
		shot,
		destroyed,
	};

	int scenestate = 0; // CHANGE THIS FOR DEBUGGING --------------------------------------------------------------------------------------


	// Keybind Flags
	bool upflag = FALSE, downflag = FALSE, leftflag = FALSE, rightflag = FALSE;
	bool jumpflag = FALSE, shootflag = FALSE;
	bool moveflag = FALSE, groundedflag = FALSE;
	int jumpflagcoyotetimer = 0;
	int shootflagcoyotetimer = 0;
	bool shootingflag = FALSE;
	int shootingtimer = 0;

	char key[256] = { 0 };
	char prevkey[256] = { 0 };

	// Sounds
	int deathbeep = LoadSoundMem("Resource\\deathbeep.wav");
	int losebeep = LoadSoundMem("Resource\\losebeep.wav");
	int shootbeep = LoadSoundMem("Resource\\shootbeep.wav");
	int beep = LoadSoundMem("Resource\\menubeep.wav");


	// Test
	LiveEntity green;
	LiveEntity red[10];

	LiveEntity green_ball[20];
	LiveEntity red_ball[10][20];

	PARTICLE pa[20][4];

	// Extras
	const char map[25][50] = { 
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1, },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1, },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1, },
	{ 1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1, },
	{ 1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, },
	};
	int rando[25][50];
	int gravity = 16;
	bool jumpingflag = FALSE;
	int jumptimer = 16;
	bool stopleft = FALSE, stopright = TRUE;
	int dev = 0;
	int selectorvar = 550;
	int deadam = 0;

	// Finals
	int enemynum = 0;
	int ballnum = 0;
	int enemyindex = 0;
	int pindex = 0;

	// ゲームループ
	while (true) {
		GetHitKeyStateAll(key);
		ClearDrawScreen();


		//---------  MATH & LOGIC & KEYBINDS  ----------//
		bool resetflag = buttonFlag(key[KEY_INPUT_R], prevkey[KEY_INPUT_R]);

		//---------  SCENESTATE SWITCH  ----------//

		switch (scenestate) {
		case openinglogo:
			if (frame < 170) FadeFromBlack(pal, 60);
			if (frame >= 170) FadeToBlack(pal, 60);
			AJKGames.DrawLogo(AJKGames.index, AJKGames.framecount, scenestate, frame, 110);
			break; // This break the opening logo

		case enginelogo:
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			DrawCamStatic(engineLOGO, WIN_WIDTH / 2 - 540 / 2, WIN_HEIGHT / 2 - 252 / 2, 0, 0, 0, 0);
			if (frame >= 400) scenestate++;
			break; // This break the opening logo

		case menu:

#pragma region
			upflag = buttonFlag(key[KEY_INPUT_UP], prevkey[KEY_INPUT_UP]);
			leftflag = buttonFlag(key[KEY_INPUT_DOWN], prevkey[KEY_INPUT_DOWN]);
			shootflag = buttonFlag(key[KEY_INPUT_SPACE], prevkey[KEY_INPUT_SPACE]);

			if (upflag) selectorvar = 550, PlaySoundMem(beep, DX_PLAYTYPE_BACK);;
			if (leftflag) selectorvar = 680, PlaySoundMem(beep, DX_PLAYTYPE_BACK);;

			DrawStatic(400, 400, title);
			DrawStatic(400, selectorvar, selector);

			if (shootflag && selectorvar == 550) scenestate++;
			else if (shootflag && selectorvar == 680) return 0;

#pragma endregion

			break;

		case reset:
			scenestate = load;
			pal = 0;
			break;

		case load:
			for (int j = 0; j < 25; j++) {
				for (int k = 0; k < 50; k++) {
					rando[j][k] = rand() % 6 + 21;
				}
			}
			green.x = 64 + 16;
			green.y = 64 + 16;

			for (int r = 0; r < 10; r++) {
				red[r].x = randomInt(32 * 4, WORLD_WIDTH - 64);
				red[r].y = 32*3;
				red[r].state = idle;
			}
			
			scenestate = gameplay;
			break;

		case gameplay:
			pal += 3;
			if (pal >= 255) pal = 255;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
			// Shake
			shakex = 0;
			shakey = 0;
			
			// Basic Necessities (Movement, AI, Hitboxes)\
			
			// Buttons
			leftflag = heldFlag(key[KEY_INPUT_LEFT]);
			rightflag = heldFlag(key[KEY_INPUT_RIGHT]);
			coyoteFlag(jumpflag, key[KEY_INPUT_Z], prevkey[KEY_INPUT_Z], jumpflagcoyotetimer, 4);
			if (!groundedflag) jumpflag = FALSE;
			if (groundedflag && jumpflagcoyotetimer > 0) jumpflag = TRUE;
			timerFlag(jumpflag, jumpingflag, jumptimer, 16);
			shootflag = buttonFlag(key[KEY_INPUT_X], prevkey[KEY_INPUT_X]);
			timerFlag(shootflag, shootingflag, shootingtimer, 6);

			

			// Movement
			// Gravity
			green.y += gravity;
			for (int r = 0; r < 10; r++) red[r].y += gravity;

			green.accelerationSpeed = 4;
			if (rightflag) green.x += green.accelerationSpeed, stopright = TRUE, stopleft = FALSE;
			if (leftflag) green.x -= green.accelerationSpeed, stopright = FALSE, stopleft = TRUE;
			if (jumpingflag) green.y -= jumptimer * 3;
			if (leftflag || rightflag || !groundedflag) moveflag = TRUE;
			else moveflag = FALSE;

			// Ball
			for (int b = 0; b < 10; b++) {
				switch (green_ball[b].state) {
				case inactive:
					if (shootflag) green_ball[b].state = shot, shootflag = FALSE, PlaySoundMem(shootbeep, DX_PLAYTYPE_BACK);;
					UpdatePosXY(green.x, green.y, green_ball[b].x, green_ball[b].y);
					if (stopright) green_ball[b].substate = right;
					else if (stopleft) green_ball[b].substate = left;
					break;
				case shot:
					if (green_ball[b].substate == right) green_ball[b].x += 6;
					else if (green_ball[b].substate == left) green_ball[b].x -= 6;
					green_ball[b].substatetimer++;
					if (green_ball[b].substatetimer >= 60) green_ball[b].state = destroyed, green_ball[b].substatetimer = 0;
					break;
				case destroyed:
					green_ball[b].substatetimer++;
					if (green_ball[b].substatetimer >= 40) green_ball[b].substatetimer = 0, green_ball[b].state = inactive;
					break;
				}
			}
			
			// Update Enemies
			for (int r = 0; r < 10; r++) {

				int enemySpeed = 2;

				if (red[r].state != dead) {
					red[r].statetimer++;
					if (red[r].statetimer >= 60) red[r].state = randomInt(0, 2), red[r].statetimer = 0;

					if (!SameNumber(red[r].x, red[r].px)) red[r].state = falling;

					if (LifeofSightHorizontal(green.x, green.y, 12, red[r].x, red[r].y, 12) && checkDist(green.x, green.y, red[r].x, red[r].y) <= 400) {
						if (green.x >= red[r].x) red[r].state = shootright;
						else red[r].state = shootleft;
					}



					switch (red[r].state) {
					case idle:
						red[r].substatetimer++;
						if (red[r].substatetimer >= 60) red[r].substatetimer = 0;
						break;
					case walkleft:
						red[r].substate = left;
						red[r].x -= enemySpeed;
						if (SameNumber(red[r].x, red[r].px)) red[r].state = walkright;
						break;
					case walkright:
						red[r].substate = right;
						red[r].x += enemySpeed;
						if (SameNumber(red[r].x, red[r].px)) red[r].state = walkleft;
						break;
					case shootleft:
						red[r].substatetimer++;
						if (red[r].substatetimer >= 6) red[r].state = idle;
						if (red[r].substatetimer >= 60) red[r].substatetimer = 0;

						break;
					case shootright:
						red[r].substatetimer++;
						if (red[r].substatetimer >= 6) red[r].state = idle;
						if (red[r].substatetimer >= 60) red[r].substatetimer = 0;

						break;
					case dead:
						break;
					}

					// Enemy balls
					for (int b = 0; b < 10; b++) {
						switch (red_ball[r][b].state) {
						case inactive:
							if (red[r].state == shootright || red[r].state == shootleft) red_ball[r][b].state = shot, PlaySoundMem(shootbeep, DX_PLAYTYPE_BACK);;
							UpdatePosXY(red[r].x, red[r].y, red_ball[r][b].x, red_ball[r][b].y);
							if (red[r].x <= green.x) red_ball[r][b].substate = right;
							else red_ball[r][b].substate = left;
							break;
						case shot:
							if (red_ball[r][b].substate == right) red_ball[r][b].x += 6;
							else if (red_ball[r][b].substate == left) red_ball[r][b].x -= 6;
							red_ball[r][b].substatetimer++;
							if (red_ball[r][b].substatetimer >= 60) red_ball[r][b].state = destroyed, red_ball[r][b].substatetimer = 0;
							break;
						case destroyed:
							red_ball[r][b].substatetimer++;
							if (red_ball[r][b].substatetimer >= 40) red_ball[r][b].substatetimer = 0, red_ball[r][b].state = inactive;
							break;
						}

						if (red_ball[r][b].state == shot && checkHit(red_ball[r][b].x, red_ball[r][b].y, green.x, green.y, 12, 8)) {
							scenestate = lose;
							ballnum = b;
							enemynum = r;
							enemyindex = red[r].index;
							pindex = green.index;
							PlaySoundMem(losebeep, DX_PLAYTYPE_BACK);
						}
					}


					// Hitbox with Balls
					for (int b = 0; b < 10; b++) {
						if (green_ball[b].state == shot && checkHit(red[r].x, red[r].y, green_ball[b].x, green_ball[b].y, 12, 8)) {
							green_ball[b].state = destroyed;
							red[r].state = dead;
							flag = TRUE;
							PlaySoundMem(deathbeep, DX_PLAYTYPE_BACK);
						}
					}
				}


				else if (red[r].state == dead) {
					deadam++;
				}


				if (red[r].state != dead && checkHit(red[r].x, red[r].y, green.x, green.y, 12, 12)) scenestate = lose, PlaySoundMem(losebeep, DX_PLAYTYPE_BACK);
			}
			Shake(flag, shakeflag, shakecount, 10, shakex, shakey);
			flag = false;
			

			// Map
			for (int j = 0; j < 25; j++) {
				for (int k = 0; k < 50; k++) {
					if (map[j][k]) {
						squareHitBox(k * 32, j * 32, 32, 32, green.x, green.y, 12, green.px, green.py);
						for (int r = 0; r < 10; r++) squareHitBox(k * 32, j * 32, 32, 32, red[r].x, red[r].y, 12, red[r].px, red[r].py);
					} 
				}
			}
			if (SameNumber(green.y, green.py)) groundedflag = TRUE, jumpingflag = FALSE;
			else groundedflag = FALSE;
			
			UpdatePosXY(green.x, green.y, green.px, green.py);
			for (int r = 0; r < 10; r++) UpdatePosXY(red[r].x, red[r].y, red[r].px, red[r].py);

			MoveCamera(camX, camY, green.x, green.y, WORLD_WIDTH - camX, 0);


			// Draw ------------------------------------------------------------------------------------------------------------------------------------
			// Map
			for (int j = 0; j < 25; j++) {
				for (int k = 0; k < 50; k++) {
					if (map[j][k]) DrawCamStatic(sprites[rando[j][k]], k * 32, j * 32, camX, 0, shakex, shakey);
				}
			}

			dev = 5;

			// Draw Enemies
			for (int r = 0; r < 10; r++) {
				switch (red[r].state) {
			case idle:
				if (red[r].substate == left) DrawAnimation(red[r].x, red[r].y - dev, sprites, red[r].index, red[r].framecount, 10, 7, 1, true, camX, camY, shakex, shakey, true);
				else DrawAnimation(red[r].x, red[r].y - dev, sprites, red[r].index, red[r].framecount, 10, 7, 1, true, camX, camY, shakex, shakey, false);
				break;
			case walkleft:
				DrawAnimation(red[r].x, red[r].y - dev, sprites, red[r].index, red[r].framecount, 
					10, 8, 2, true, camX, camY, shakex, shakey, true);
				break;
			case walkright:
				DrawAnimation(red[r].x, red[r].y - dev, sprites, red[r].index, red[r].framecount, 
					10, 8, 2, true, camX, camY, shakex, shakey, false);
				break;
			case shootleft:
				DrawAnimation(red[r].x, red[r].y - dev, sprites, red[r].index, red[r].framecount, 
					3, 10, 2, true, camX, camY, shakex, shakey, true);
				break;
			case shootright:
				DrawAnimation(red[r].x, red[r].y - dev, sprites, red[r].index, red[r].framecount, 
					3, 10, 2, true, camX, camY, shakex, shakey, false);
				break;
			case falling:
				if (red[r].substate == left) DrawAnimation(red[r].x, red[r].y - dev, sprites, red[r].index, red[r].framecount, 
					10, 12, 1, true, camX, camY, shakex, shakey, true);
				else DrawAnimation(red[r].x, red[r].y - dev, sprites, red[r].index, red[r].framecount, 
					10, 12, 1, true, camX, camY, shakex, shakey, false);
				break;
			}

				// Enemy Balls
				if (red[r].state != dead) for (int b = 0; b < 10; b++) {
					switch (red_ball[r][b].state) {
					case inactive:
						break;
					case shot:
						DrawAnimation(red_ball[r][b].x, red_ball[r][b].y - dev * 3, sprites, 
							red_ball[r][b].index, red_ball[r][b].framecount, 1, 13, 1, true, camX, camY, shakex, shakey, false);
						break;
					case destroyed:
						break;
					}
				}

			}

			// Draw Player
			if (shootingflag && stopright)
				DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 3, 3, 2, true, camX, camY, shakex, shakey, false);
			else if (shootingflag && stopleft) 
				DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 3, 3, 2, true, camX, camY, shakex, shakey, true);
			else if (!groundedflag) 
				DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 1, 5, 1, true, camX, camY, shakex, shakey, false);
			else if (rightflag) 
				DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 10, 1, 2, true, camX, camY, shakex, shakey, false);
			else if (leftflag) 
				DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 10, 1, 2, true, camX, camY, shakex, shakey, true);
			else if (stopright) 
				DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 1, 0, 1, true, camX, camY, shakex, shakey, false);
			else if (stopleft) 
				DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 1, 0, 1, true, camX, camY, shakex, shakey, true);


			// Draw Balls
			// Ball
			for (int b = 0; b < 10; b++) {
				switch (green_ball[b].state) {
				case inactive:
					break;
				case shot:
					for (int p = 0; p < 4; p++) {
						pa[b][p].x = green_ball[b].x;
						pa[b][p].y = green_ball[b].y;
					}
					DrawAnimation(green_ball[b].x, green_ball[b].y - dev*3, sprites, green_ball[b].index, green_ball[b].framecount, 1, 6, 1, true, camX, camY, shakex, shakey, false);
					break;
				case destroyed:
					for (int p = 0; p < 4; p++) {
						switch (p) {
						case 0:
							pa[b][p].x -= randomInt(1, 3);
							pa[b][p].y -= randomInt(1, 3);
							DrawBox(pa[b][p].x - camX + shakex, pa[b][p].y - camY + shakey, 
								pa[b][p].x - camX + shakex + randomInt(5, 8), 
								pa[b][p].y - camY + shakey + randomInt(5, 8), 
								GetColor(255, 255, 255), true);
							break;
						case 1:
							pa[b][p].x += randomInt(1, 3);
							pa[b][p].y -= randomInt(1, 3);
							DrawBox(pa[b][p].x - camX + shakex, pa[b][p].y - camY + shakey,
								pa[b][p].x - camX + shakex + randomInt(5, 8),
								pa[b][p].y - camY + shakey + randomInt(5, 8), 
								GetColor(255, 255, 255), true);
							break;
						case 2:
							pa[b][p].x -= randomInt(1, 3);
							pa[b][p].y += randomInt(1, 3);
							DrawBox(pa[b][p].x - camX + shakex, pa[b][p].y - camY + shakey,
								pa[b][p].x - camX + shakex + randomInt(5, 8),
								pa[b][p].y - camY + shakey + randomInt(5, 8), 
								GetColor(255, 255, 255), true);
							break;
						case 3:
							pa[b][p].x += randomInt(1, 3);
							pa[b][p].y += randomInt(1, 3);
							DrawBox(pa[b][p].x - camX + shakex, pa[b][p].y - camY + shakey,
								pa[b][p].x - camX + shakex + randomInt(5, 8),
								pa[b][p].y - camY + shakey + randomInt(5, 8), 
								GetColor(255, 255, 255), true);
							break;
						}
					}

					break;
				}
			}


			// Draw UI
			for (int k = 0; k < 4; k++) DrawStatic(64 + k * 64 + 16, 64, keys[k]);

			// LOSE&WIN GAME HERE ------------------------------------------------------------------------
			if (deadam >= 10) {
				DrawStatic(WIN_WIDTH / 2, WIN_HEIGHT / 2, winpng);
				pindex = green.index;
			}
			deadam = 0;
		
			break; // This breaks gameplay
			

			case lose:
				if (stopright)
					DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 1, pindex, 1, true, camX, camY, shakex, shakey, false);
				else if (stopleft)
					DrawAnimation(green.x, green.y - dev, sprites, green.index, green.framecount, 1, pindex, 1, true, camX, camY, shakex, shakey, true);

				DrawAnimation(red_ball[enemynum][ballnum].x, red_ball[enemynum][ballnum].y - dev * 3, sprites,
					red_ball[enemynum][ballnum].index, red_ball[enemynum][ballnum].framecount, 1, 13, 1, true, camX, camY, shakex, shakey, false);

				if (red[enemynum].substate == left) DrawAnimation(red[enemynum].x, red[enemynum].y - dev, sprites, red[enemynum].index, 
					red[enemynum].framecount, 10, enemyindex, 1, true, camX, camY, shakex, shakey, true);
				else DrawAnimation(red[enemynum].x, red[enemynum].y - dev, sprites, red[enemynum].index,
					red[enemynum].framecount, 10, enemyindex, 1, true, camX, camY, shakex, shakey, false);

				DrawString(WIN_WIDTH / 3, 200, "もう一回？（Rキーでリトライ）", GetColor(255, 255, 255));
				
				break;
		}

		// DEBUG GOES HERE
		if (resetflag) scenestate = reset;
		
		//---------  ESSENTIALS  ---------//
		GetHitKeyStateAll(prevkey);
		frame++;
		ScreenFlip();
		WaitTimer(20); // 20 miliseconds = 60 FPS
		if (ProcessMessage() == -1) {
			break;
		}
	}
	DxLib_End(); // If this gives an error, restart Visual Studio.
	return 0;
}
