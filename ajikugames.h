#pragma once

const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 800;

const int WORLD_WIDTH = 1600;
const int WORLD_HEIGHT = 800;

const int TILE_VERTICAL_AMMOUNT = WIN_HEIGHT / 32;
const int TILE_HORIZONTAL_AMMOUNT = WIN_WIDTH / 32;

const int TILE_SIZE = 32;

const enum directions {
	up,
	down,
	left,
	right,
	up_left,
	up_right,
	down_left,
	down_right,
};

class logo
{
public:
	int logo[4];
	int index;
	int framecount;
	int logosound;

	void DrawLogo(int& index, int& framecount, int& scenestate, int frame, int lenght);
};

// Flags
int heldFlag(int key);
int buttonFlag(int key, int oldkey);
void timerFlag(bool flag, bool& secondflag, int& timer, int settimer);
void coyoteFlag(bool& flag, int key, int oldkey, int& flagtimer, int coyotetime);

// Math
int BiggerNum(int num1, int num2);
float BiggerNum(float num1, float num2);
int SmallerNum(int num1, int num2);
float SmallerNum(float num1, float num2);
bool SameNumber(int num1, int num2);
int Middle(int num1, int num2);
int SmallerAbsolute(int n1, int n2);
int randomInt(int min, int max);


// Fade Functions
void FadeFromBlack(int& pal, int duration);
void FadeToBlack(int& pal, int duration);

// Draw Functions
void DrawCamStatic(int image, int x, int y, int camx, int camy, int shakex, int shakey);
void DrawStatic(int xpos, int ypos, int image);
void DrawReferencePoint(int x, int y, int camx, int camy, int size, int shakex, int shakey);
void DrawColorPoint(int x, int y, int camx, int camy, int size, int red, int green, int blue, int shakex, int shakey);
void DrawReferenceBox(int x, int y, int camx, int camy, int sizeX, int sizeY, int shakex, int shakey);
void DrawAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, int shakex, int shakey, bool reverseflag);
void DrawRotaAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, float angle, int shakex, int shakey);
void DrawReferenceLine(int x1, int y1, int x2, int y2, int camx, int camy, int R, int G, int B, int size, int shakex, int shakey);
void DrawPercentBar(int startX, int startY, int endX, int endY, int camX, int camY, int min, int max, int current, int R, int G, int B, bool horizontalflag);

// Hitbox Related Functions
int checkHit(int playerX, int playerY, int objX, int objY, int playerSize, int objSize);
int checkSquareHit(int x, int y, int sizeX, int sizeY, int playerX, int playerY, int playerSize);
int checkDist(int playerX, int playerY, int objX, int objY);
int checkDistLine(int var, int var2);
void circleHitBox(int& playerX, int& playerY, int objX, int objY, int playerSize, int objSize);
void circleHitBoxReversed(int& playerX, int& playerY, int objX, int objY, int playerSize, int objSize);
void squareHitBox(int x, int y, int sizeX, int sizeY, int& playerX, int& playerY, int playerSize, int prevX, int prevY);


// Scripts
void SimpleFollowScript(int& x, int& y, int chaseX, int chaseY, int speed);
void SimpleRunAwayScript(int& x, int& y, int runX, int runY, int speed);
void AjikuLineAlg(int x1, int y1, int x2, int y2);
int CountAjikuChainAlg(int x1, int y1, int x2, int y2);
bool LifeofSightHorizontal(int playerX, int playerY, int playerSize, int targetX, int targetY, int targetSize /* If the target has vision, so does player.*/);


// Essentials
void MoveCamera(int& camX, int& camY, int followX, int followY, int MAXcamX, int MAXcamY);
void UpdatePosXY(int x, int y, int& px, int& py);
void Shake(bool flag, bool& shakeflag, int& shakecount, int duration, int& shakex, int& shakey);

