#include "ajikugames.h"
#include "DxLib.h"
#include<math.h>
#include <time.h>


void logo::DrawLogo(int& index, int& framecount, int& scenestate, int frame, int lenght) {
	if (!CheckSoundMem(logosound)) PlaySoundMem(logosound, DX_PLAYTYPE_BACK);
	framecount++;
	if (framecount >= 5) {
		index++;
		if (index >= 4) index = 0;
		framecount = 0;
	}
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 1.5, 0, logo[index], true);
	if (frame > lenght * 2) scenestate++;
}

int heldFlag(int key)
{
	int VALUE;
	if (key) VALUE = TRUE;
	else VALUE = FALSE;
	return VALUE;
}

int buttonFlag(int key, int oldkey) {
	int flag = FALSE;
	if (key && !oldkey) flag = TRUE;
	else if (key == oldkey) flag = FALSE;

	return flag;
}

void timerFlag(bool flag, bool& secondflag, int& timer, int settimer)
{
	if (flag) secondflag = TRUE, timer = settimer;
	if (secondflag) timer--;
	if (timer <= 0) secondflag = FALSE;
}

void coyoteFlag(bool& flag, int key, int oldkey, int& flagtimer, int coyotetime)
{
	if (key && !oldkey) flag = TRUE, flagtimer = coyotetime;
	if (flagtimer > 0) flagtimer--;
	if (flagtimer <= 0) flag = FALSE;
}

// Fade Functions
void FadeFromBlack(int& pal, int duration) {
	int faderate = 255 / duration;
	if (pal < 0) pal = 0;
	pal += faderate;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
	if (pal >= 255) pal = 255;
}

void FadeToBlack(int& pal, int duration)
{
	int faderate = 255 / duration;
	if (pal >= 255) pal = 255; 
	pal -= faderate;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);
	if (pal < 0) pal = 0;
}

// Math
int BiggerNum(int num1, int num2)
{
	int num;
	if (num1 >= num2) num = num1;
	else if (num1 < num2) num = num2;
	return num;
}

float BiggerNum(float num1, float num2)
{
	float num;
	if (num1 >= num2) num = num1;
	else if (num1 < num2) num = num2;
	return num;
}

int SmallerNum(int num1, int num2)
{
	int num;
	if (num1 <= num2) num = num1;
	else if (num1 > num2) num = num2;
	return num;
}

float SmallerNum(float num1, float num2)
{
	float num;
	if (num1 <= num2) num = num1;
	else if (num1 > num2) num = num2;
	return num;
}

bool SameNumber(int num1, int num2)
{
	bool flag = FALSE;
	if (num1 == num2) flag = TRUE;
	else flag = FALSE;

	return flag;
}

int Middle(int num1, int num2)
{
	int value = (num1 + num2) / 2;
	return value;
}

int SmallerAbsolute(int n1, int n2)
{
	if (abs(n1) <= abs(n2)) return n1;
	else if (abs(n1) > abs(n2)) return n2;
}

int randomInt(int min, int max)
{
	int dif = max - min + 1;
	int value = rand() % dif + min;
	return value;
}

// Draw Functions

void DrawCamStatic(int image, int x, int y, int camx, int camy, int shakex, int shakey)
{
	DrawGraph(x - camx + shakex, y - camy + shakey, image, true);
}

void DrawStatic(int xpos, int ypos, int image)
{
	DrawRotaGraph(xpos, ypos, 1, 0, image, true);
}

void DrawReferencePoint(int x, int y, int camx, int camy, int size, int shakex, int shakey) {
	DrawCircle(x - camx + shakex, y - camy + shakey, size, GetColor(255, 100, 200), true);
}

void DrawColorPoint(int x, int y, int camx, int camy, int size, int red, int green, int blue, int shakex, int shakey) {
	DrawCircle(x - camx + shakex, y - camy + shakey, size, GetColor(red, green, blue), true);
}

void DrawReferenceBox(int x, int y, int camx, int camy, int sizeX, int sizeY, int shakex, int shakey) {
	int x1 = x - camx, y1 = y - camy;
	DrawBox(x1 + shakex, y1 + shakey, x1 + sizeX + shakex, y1 + sizeY + shakey, GetColor(255, 255, 255), true);
}

void DrawAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, int shakex, int shakey, bool reverseflag) {
	framecount++;
	if (index < firstframe) index = firstframe;
	if (index > firstframe + ammountofframes && loopflag) index = firstframe;
	if (framecount >= framerate) {
		framecount = 0;
		index++;
		if (loopflag && index >= firstframe + ammountofframes) index = firstframe;
		else if (!loopflag && index >= firstframe + ammountofframes) index = firstframe + ammountofframes - 1;
	}
	DrawRotaGraph(x - camX + shakex, y - camY + shakey, 1, 0, image[index], true, reverseflag);
}

void DrawRotaAnimation(int x, int y, int image[], int& index, int& framecount, int framerate, int firstframe, int ammountofframes, int loopflag, int camX, int camY, float angle, int shakex, int shakey) {
	framecount++;
	if (index < firstframe) index = firstframe;
	if (index > firstframe + ammountofframes) index = firstframe;
	if (framecount >= framerate) {
		framecount = 0;
		index++;
		if (loopflag) if (index >= firstframe + ammountofframes) index = firstframe;
		else if (!loopflag) if (index >= firstframe + ammountofframes) index = firstframe + ammountofframes;
	}
	DrawRotaGraph(x - camX + shakex, y - camY + shakey, 1, angle, image[index], true);
}

void DrawReferenceLine(int x1, int y1, int x2, int y2, int camx, int camy, int R, int G, int B, int size, int shakex, int shakey) {
	DrawLine(x1 - camx + shakex, y1 - camy + shakey, x2 - camx + shakex, y2 - camy + shakey, GetColor(R, G, B), size);
}

void DrawPercentBar(int startX, int startY, int endX, int endY, int camX, int camY, int min, int max, int current, int R, int G, int B, bool horizontalflag)
{
	float maxvalue = max - min;
	float percent = (current - min) / maxvalue;
	int size = 0;

	if (horizontalflag) {
		size = endX - startX;
		DrawBox(startX - camX, startY - camY, startX + size - camX, endY - camY, GetColor(80, 20, 60), true);
		DrawBox(startX - camX, startY - camY, startX + size * percent - camX, endY - camY, GetColor(R, G, B), true);
		DrawBox(startX - camX, startY - camY, startX + size - camX, endY - camY, GetColor(0, 0, 0), false);
		
	}
	else {
		int size = endY - startY;
		DrawBox(startX - camX, startY - camY, endX - camX, startY + size * percent - camY, GetColor(R, G, B), true);
		DrawBox(startX - camX, startY - camY, endX- camX, startY + size - camY, GetColor(0, 0, 0), false);
	}
}


// Hitbox Related Functions

int checkHit(int playerX, int playerY, int objX, int objY, int playerSize, int objSize) {
	int flag = FALSE;
	float a = pow(playerX - objX, 2);
	if (a < 0) a = -a;
	float b = pow(playerY - objY, 2);
	if (b < 0) b = -b;
	float c = sqrt(a + b);
	if (c <= playerSize + objSize) flag = TRUE;
	else flag = FALSE;

	return flag;
}

int checkDist(int playerX, int playerY, int objX, int objY) {
	float a = pow(playerX - objX, 2);
	if (a < 0) a = -a;
	float b = pow(playerY - objY, 2);
	if (b < 0) b = -b;
	float c = sqrt(a + b);
	round(c);

	return c;
}

int checkDistLine(int var, int var2)
{
	int value = abs(var - var2);
	return value;
}

void circleHitBox(int& playerX, int& playerY, int objX, int objY, int playerSize, int objSize) {
	int dist = checkDist(playerX, playerY, objX, objY);
	if (dist < playerSize + objSize) {
		int disp = playerSize + objSize - dist;
		if (objX >= playerX) playerX -= disp;
		if (objX <= playerX) playerX += disp;
		if (objY >= playerY) playerY -= disp;
		if (objY <= playerY) playerY += disp;
		// NOT PERFECT YET
	}
}

void circleHitBoxReversed(int& playerX, int& playerY, int objX, int objY, int playerSize, int objSize) {
	int dist = checkDist(playerX, playerY, objX, objY);
	if (dist > playerSize + objSize) {
		float disp = playerSize + objSize - dist - 1;
		if (objX >= playerX) playerX -= disp;
		if (objX <= playerX) playerX += disp;
		if (objY >= playerY) playerY -= disp;
		if (objY <= playerY) playerY += disp;
		// NOT PERFECT YET
	}
}

void squareHitBox(int x, int y, int sizeX, int sizeY, int& playerX, int& playerY, int playerSize, int prevX, int prevY) {
	int x2 = x + sizeX;
	int y2 = y + sizeY;

	int boxcenterX = Middle(x, x + sizeX);
	int boxcenterY = Middle(y, y + sizeY);

	int up = playerY - playerSize;
	int down = playerY + playerSize;
	int left = playerX - playerSize;
	int right = playerX + playerSize;

	int vertflag = FALSE;
	int horiflag = FALSE;

	if (right > x && left < x2) vertflag = TRUE;
	if (down > y && up < y2) horiflag = TRUE;

	// Prev
	int prevup = prevY - playerSize;
	int prevdown = prevY + playerSize;
	int prevleft = prevX - playerSize;
	int prevright = prevX + playerSize;

	int prevvertflag = FALSE;
	int prevhoriflag = FALSE;

	if (prevright > x && prevleft < x2) prevvertflag = TRUE;
	if (prevdown > y && prevup < y2) prevhoriflag = TRUE;

	int devX1 = x - prevright,
		devX2 = x2 - prevleft;

	int devY1 = y - prevdown,
		devY2 = y2 - prevup;

	int devX = SmallerAbsolute(devX1, devX2);
	int devY = SmallerAbsolute(devY1, devY2);

	// Calculations
	if (vertflag && horiflag) {
		if (prevvertflag) playerY = prevY + devY;
		else if (prevhoriflag) playerX = prevX + devX;
	}
}

int checkSquareHit(int x, int y, int sizeX, int sizeY, int playerX, int playerY, int playerSize) {
	int x2 = x + sizeX;
	int y2 = y + sizeY;

	int up = playerY - playerSize;
	int down = playerY + playerSize;
	int left = playerX - playerSize;
	int right = playerX + playerSize;

	int horflag = FALSE;
	int verflag = FALSE;

	if (right >= x && left <= x2) horflag = TRUE;
	if (down >= y && up <= y2) verflag = TRUE;

	int flag = FALSE;
	if (horflag && verflag) flag = TRUE;
	return flag;
}


// Scripts

void SimpleFollowScript(int& x, int& y, int chaseX, int chaseY, int speed) {
	if (x < chaseX) x += speed;
	else if (x > chaseX) x -= speed;
	if (y < chaseY) y += speed;
	else if (y > chaseY) y -= speed;
}

void SimpleRunAwayScript(int& x, int& y, int runX, int runY, int speed) {
	if (x < runX) x -= speed;
	else if (x > runX) x += speed;
	if (y < runY) y -= speed;
	else if (y > runY) y += speed;
}

void AjikuLineAlg(int x1, int y1, int x2, int y2)
{
	int am = 0;

	float dx = abs(x1 - x2);
	if (dx == 0) dx = 1;
	float dy = abs(y1 - y2);
	if (dy == 0) dy = 1;


	float fx = dx / BiggerNum(dx, dy);
	float fy = dy / BiggerNum(dx, dy);

	float x, y;

	x = x1;
	y = y1;

	while (x < x2) {
		DrawBox(x, y, x + 2, y + 2, GetColor(255, 255, 255), true);
		x += fx;
		if (y2 >= y1) y += fy;
		else y -= fy;

		am++;
	}

	while (x > x2) {
		DrawBox(x, y, x + 2, y + 2, GetColor(255, 255, 255), true);
		x -= fx;
		if (y2 >= y1) y += fy;
		else y -= fy;

		am++;
	}

	if (x == x2) {
		while (y < y2) {
			DrawBox(x, y, x + 2, y + 2, GetColor(255, 255, 255), true);
			y += fy;

			am++;
		}

		while (y > y2) {
			DrawBox(x, y, x + 2, y + 2, GetColor(255, 255, 255), true);
			y -= fy;

			am++;
		}
	}
}

int CountAjikuChainAlg(int x1, int y1, int x2, int y2) {
	int am = 0;

	float dx = abs(x1 - x2);
	if (dx == 0) dx = 1;
	float dy = abs(y1 - y2);
	if (dy == 0) dy = 1;

	
	float fx = dx / BiggerNum(dx, dy);
	float fy = dy / BiggerNum(dx, dy);

	float x, y;

	x = x1;
	y = y1;

	while (x < x2) {
		DrawBox(x, y, x + 2, y + 2, GetColor(255, 255, 255), true);
		x += fx;
		if (y2 >= y1) y += fy;
		else y -= fy;

		am++;
	}

	while (x > x2) {
		DrawBox(x, y, x + 2, y + 2, GetColor(255, 255, 255), true);
		x -= fx;
		if (y2 >= y1) y += fy;
		else y -= fy;

		am++;
	}

	if (x == x2) {
		while (y < y2) {
			DrawBox(x, y, x + 2, y + 2, GetColor(255, 255, 255), true);
			y += fy;

			am++;
		}

		while (y > y2) {
			DrawBox(x, y, x + 2, y + 2, GetColor(255, 255, 255), true);
			y -= fy;

			am++;
		}
	}

	return am;
}

bool LifeofSightHorizontal(int playerX, int playerY, int playerSize, int targetX, int targetY, int targetSize)
{
	int flag = FALSE;

	if (playerX - playerSize >= targetX - targetSize && playerX - playerSize <= targetX + targetSize ||
		playerX + playerSize >= targetX - targetSize && playerX + playerSize <= targetX + targetSize) {
		flag = TRUE;
	}

	if (playerY - playerSize >= targetY - targetSize && playerY - playerSize <= targetY + targetSize ||
		playerY + playerSize >= targetY - targetSize && playerY + playerSize <= targetY + targetSize) {
		flag = TRUE;
	}

	return flag;
}




// Essentials
void MoveCamera(int& camX, int& camY, int followX, int followY, int MAXcamX, int MAXcamY) {

	camX = followX - WIN_WIDTH / 2;
	camY = followY - WIN_HEIGHT / 2;

	
	if (camX >= MAXcamX) camX = MAXcamX;
	else if (camX <= 0) camX = 0;

	if (camY >= MAXcamY) camY = MAXcamY;
	else if (camY <= 0) camY = 0;

}

void UpdatePosXY(int x, int y, int& px, int& py)
{
	px = x;
	py = y;
}

void Shake(bool flag, bool& shakeflag, int& shakecount, int duration, int& shakex, int& shakey) {
	if (flag && !shakeflag) {
		shakeflag = TRUE;
		shakecount = duration;
	}
	else if (shakeflag) {
		shakecount--;
		shakex = rand() % 10 - 5;
		shakey = rand() % 10 - 5;
		if (shakecount <= 0) shakeflag = FALSE;
	}
	if (!shakeflag) shakecount = 0, shakex = 0, shakey = 0;
}




