#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define width 65  // width  -
#define height 25 // height |

// Rocket struct
typedef struct {
	int x, y;
	int w;
}SRacket;


// Ball struct
typedef struct {
	float x, y;
	int ix, iy;
	float alfa;
	float speed;
}SBall;

char mas[height][width+1]; // Field array
int HitCnt = 0;
int MaxHitCnt = 0;

SRacket racket;            // Making a racket
SBall ball;           	   // Ball creation

//ball movement
void MoveBall(float x, float y) {
	ball.x = x;
	ball.y = y;
	ball.iy = (int)round(ball.y);
	ball.ix = (int)round(ball.x);
}


//ball initialization
void InitBall() {
	MoveBall(2, 2);
	ball.alfa = -1;
	ball.speed = 0.5;
}


// placing a ball in an array
void PutBall() {
	mas[ball.iy][ball.ix] = '*';
}


// "game"
void AutoMoveBall() {
	if (ball.alfa < 0) ball.alfa += M_PI * 2;
	if (ball.alfa > M_PI * 2) ball.alfa -= M_PI * 2;
	SBall bl = ball;
	MoveBall(ball.x + cos(ball.alfa) * ball.speed, ball.y + sin(ball.alfa) * ball.speed);
	if ((mas[ball.iy][ball.ix] == '#') || (mas[ball.iy][ball.ix] == '@')) {
		if (mas[ball.iy][ball.ix] == '@') {
			HitCnt++;
		}
		if ((ball.ix != bl.ix) && (ball.iy != bl.iy)) {
			if (mas[bl.iy][bl.ix] == mas[ball.iy][ball.ix])
				bl.alfa = bl.alfa + M_PI;
			else {
				if (mas[bl.iy][bl.ix] == '#') 
					bl.alfa = (2 * M_PI - bl.alfa) + M_PI;
				else 
					bl.alfa = (2 * M_PI - bl.alfa);
				
			}
		}
		else if (ball.iy == bl.iy)
			bl.alfa = (2 * M_PI - bl.alfa) + M_PI;
		else
			bl.alfa = (2 * M_PI - bl.alfa);
		ball = bl;
		AutoMoveBall();
	}
}


//racket initialization
void InitRacket(){
	racket.w = 7;
	racket.x = (width - racket.w) / 2;
	racket.y = height-1;
}


// placing a racket in an array
void PutRacket(){
	for (int i = racket.x; i < racket.x + racket.w; i++) {
		mas[racket.y][i] = '@';
	}
}


// racket movement
void MoveRacket(int x) {
	racket.x = x;
	if (racket.x < 1) {
		racket.x = 1;
	}
	if (racket.x + racket.w >= width) {
		racket.x = width - racket.w - 1;
	}
}


// field initialization
void InitField() {
	
	for (int i = 0; i < width; i++) {
		mas[0][i] = '#';
	}
	mas[0][width] = '\0';
	strncpy(mas[1], mas[0], width + 1);
	for (int i = 1; i < width-1; i++) {
		mas[1][i] = ' ';
	}
	for (int i = 2; i < height; i++) {
		strncpy(mas[i], mas[1], width+1);
	}
}


// display field
void ShowField() {
	for (int i = 0; i < height; i++) {
		printf("%s", mas[i]);
		if (i == 3) printf("     Hit  %i    ", HitCnt);
		if (i == 4) printf("     Max  %i    ", MaxHitCnt);
		if (i < height-1 ) {
			printf("\n");
		}
		
	}
}


//a carriage return
void setcur(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , coord);
}
int main() {
	// hide console cursor
	HANDLE  handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);

	BOOL run = FALSE;
	InitRacket();
	InitBall();
	do {
		setcur(0, 0);

		if (run) AutoMoveBall();
		if (ball.iy > height) {
			run = FALSE;
			if (HitCnt > MaxHitCnt)
				MaxHitCnt = HitCnt;
			HitCnt = 0;
		}
		InitField();
		PutBall();
		PutRacket();
		ShowField();
		if (GetKeyState('A') < 0) MoveRacket(racket.x - 1);
		if (GetKeyState('D') < 0) MoveRacket(racket.x + 1);
		if (GetKeyState(VK_SPACE) < 0) run = TRUE;
		if (!run)
		MoveBall(racket.x + racket.w / 2, racket.y - 1);
		
		Sleep(11);
	} while (GetKeyState(VK_ESCAPE) >= 0);
	return 0;
}
