#include<bangtal.h>
#include<time.h>
#include<stdlib.h>


SceneID scene;
ObjectID play, piece[12];
const char* image_num[12] = {"images/PIECE/1.jpg", "images/PIECE/2.jpg", "images/PIECE/3.jpg", "images/PIECE/4.jpg", 
"images/PIECE/5.jpg", "images/PIECE/6.jpg", "images/PIECE/7.jpg", "images/PIECE/8.jpg", "images/PIECE/9.jpg", 
"images/PIECE/10.jpg", "images/PIECE/11.jpg", "images/PIECE/12.jpg"};
const int piece_X[12] = { 0, 282, 564, 846, 0, 282, 564, 846, 0, 282, 564, 846 };
const int piece_Y[12] = { 466, 466, 466, 466, 233, 233, 233, 233, 0, 0, 0, 0};
int blank;

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

bool movable(int index) {
	if (blank < 0 or blank > 11) 
		return false;
	if (blank % 4 != 0 and blank - 1 == index) 
		return true;
	if (blank % 4 != 3 and blank + 1 == index) 
		return true;
	if (blank - 4 == index) 
		return true;
	if (blank + 4 == index) 
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
		for (int i = 0; i < 10; i++)
			piece_mix();
	}
	else {
		int index = piece_index(object);
		if (movable(index))
			piece_move(index);
	}
	
	
}

int main() {
	
	setMouseCallback(mousecallback);
	
	scene = createScene("scene", "images/castle.jpg");
	
	for (int i = 0; i < 12; i ++) {
		piece[i] = createObject(image_num[i]);
		locateObject(piece[i], scene, piece_X[i], piece_Y[i]);
		showObject(piece[i]);
	}
	blank = 11;

	hideObject(piece[blank]);

	play = createObject("images/play.png");
	locateObject(play, scene, 500, 100);
	showObject(play);
	
	startGame(scene);
}