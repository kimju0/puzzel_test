#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#include<bangtal.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>


SceneID scene;
ObjectID play, restart, init_board[12], piece[12];
const char* image_num[12] = {"images/PIECE/1.jpg", "images/PIECE/2.jpg", "images/PIECE/3.jpg", "images/PIECE/4.jpg",
"images/PIECE/5.jpg", "images/PIECE/6.jpg", "images/PIECE/7.jpg", "images/PIECE/8.jpg", "images/PIECE/9.jpg", 
"images/PIECE/10.jpg", "images/PIECE/11.jpg", "images/PIECE/12.jpg"};
const int piece_X[12] = { 0, 282, 564, 846, 0, 282, 564, 846, 0, 282, 564, 846 };
const int piece_Y[12] = { 466, 466, 466, 466, 233, 233, 233, 233, 0, 0, 0, 0};
int blank, increase = 0;
TimerID timer, stoptimer, visibletimer;
int mixCount;
char t[5];


int piece_index(ObjectID object) {
	for (int i = 0; i < 12; i++) {
		if (piece[i] == object) return i;
	}
	return -1;
}

void piece_move(int index) {
	ObjectID t = piece[blank];
	piece[blank] = piece[index];
	piece[index] = t;

	locateObject(piece[blank], scene, piece_X[blank], piece_Y[blank]);
	locateObject(piece[index], scene, piece_X[index], piece_Y[index]);

	blank = index;

}

bool completed() {
	for (int i = 0; i < 12; i++) {
		if (piece[i] != init_board[i]) return false;
	}
	showObject(restart);
	return true;
}

bool movable(int index) {
	if (blank < 0 or blank > 11) 
		return false;
	if (index < 0 or index > 11)
		return false;
	if (blank % 4 != 0 and blank - 1 == index) 
		return true;
	if (blank % 4 != 3 and blank + 1 == index) 
		return true;
	if (blank / 4 != 0 and blank - 4 == index) 
		return true;
	if (blank / 4 != 3 and blank + 4 == index) 
		return true;
	
	return false;
}

void piece_mix() {
	int index;

	do {
		switch (rand() % 4) {
			case 0: index = blank - 1; break;
			case 1: index = blank + 1; break;
			case 2: index = blank - 4; break;
			case 3: index = blank + 4; break;
		}
	} while (!movable(index));
	piece_move(index);
}

void mousecallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == play) {
		mixCount = 10;
		startTimer(timer);
		hideObject(play);
		startTimer(stoptimer);
	}
	if (object == restart) {
		mixCount = 10;
		startTimer(timer);
		hideObject(restart);
		setTimer(visibletimer, 0.0f);
		startTimer(stoptimer);
	}
	else {
		int index = piece_index(object);
		if (movable(index)) {
			piece_move(index);
			if (completed()) {
				sprintf(t, "complete \n you spend %d sec", increase);
				showMessage(t);
				increase = 0;
				stopTimer(stoptimer);
			}
		}
			
	}
}

void timercallback(TimerID timer) {
	if (timer == timer) {
		mixCount--;
		if (mixCount > 0) {
			piece_mix();

			startTimer(timer);
		}
	}
	if (timer == stoptimer) {
		increase ++;
		setTimer(visibletimer, increase);
		showTimer(visibletimer);
		startTimer(stoptimer);
		setTimer(stoptimer, 1.0f);
	}
}

int main() {
	
	setMouseCallback(mousecallback);
	setTimerCallback(timercallback);

	scene = createScene("scene", "images/castle.jpg");
	
	for (int i = 0; i < 12; i ++) {
		piece[i] = createObject(image_num[i]);
		init_board[i] = piece[i];

		locateObject(piece[i], scene, piece_X[i], piece_Y[i]);
		showObject(piece[i]);
	}
	blank = 11;

	hideObject(piece[blank]);

	play = createObject("images/play.png");
	locateObject(play, scene, 500, 100);
	showObject(play);

	restart = createObject("images/restart.png");
	locateObject(restart, scene, 500, 100);
	
	timer = createTimer(0.f);
	stoptimer = createTimer(1.f);
	visibletimer = createTimer(0.f);

	startGame(scene);
}