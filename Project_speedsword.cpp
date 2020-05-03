#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneID scene1, scene2;
ObjectID startbutton, restartbutton, endbutton, normal, blue, red, yellow, heart1, heart2, heart3;
ObjectID enemy[150] = { 0, };
TimerID check;
SoundID bgm, finalHit, finalHit1, normalHit, normalHit1, normalHit2, normalHit3, normalHit4;
int arr[150] = { 0, }, Elife[150] = { 0, }, locate[150] = { 0, };
int count = 0, clear = 0, life = 3, location = 35, NhitCount = 0, FhitCount = 0;
char name[30] = { 0, };
float duration = 0.6f;
bool bgmloop = true;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {

	ObjectID object = createObject(image);

	locateObject(object, scene, x, y);

	if (shown) {
		showObject(object);
	}

	return object;
}

void patternMaker() {		// 적 특성 저장할 정수 배열 만들기

	int i, x, y, temp;

	srand(time(NULL));

	//1~4까지는 목숨하나 몹 차례대로 N,B,R,Y
	//5~8까지는 목숨 여러개 몹 차례대로 N,B,R,Y

	for (i = 0; i < 100; i++) {
		if (i < 15) arr[i] = 1;
		else if (i < 30) arr[i] = 2;
		else if (i < 45) arr[i] = 3;
		else if (i < 60) arr[i] = 4;
		else if (i < 70) arr[i] = 5;
		else if (i < 80) arr[i] = 6;
		else if (i < 90) arr[i] = 7;
		else if (i < 100) arr[i] = 8;
		else if (i < 120) arr[i] = 0;
	}
	
	for (i = 0; i < 300; i++) {
		x = rand() % 120;
		y = rand() % 120;

		if (x != y) {
			temp = arr[x];
			arr[x] = arr[y];
			arr[y] = temp;
		}
	}
}

void enemyLife() {			// 적 목숨 랜덤 설정

	srand(time(NULL));

	for (int i = 0; i < 120; i++) {
		if (arr[i] == 5 || arr[i] == 6 || arr[i] == 7 || arr[i] == 8) {
			Elife[i] = rand() % 3 + 2;
		}
	}
}

void createEnemy() {		// 적 오브젝트 생성

	int i;

	for (i = 0; i < 120; i++) {

		if (arr[i] != 0) {
			sprintf_s(name, "images/%d.png", arr[i]);
			enemy[i] = createObject(name, scene2, 610, 720, false);
		}
	}
}

void ending(bool success) {		//게임 클리어 후 처리

	if (success == true) {
		showMessage("성공!");
		bgmloop = true;
	}
	else {
		stopTimer(check);
		showMessage("실패...");
		bgmloop = false;
		for (int i = 0; i < 120; i++) {
			if(arr[i] != 0) hideObject(enemy[i]);
		}
	}

	showObject(restartbutton);
	showObject(endbutton);

}

void minusHeart() {			// 오답 시 목숨 감소
	life--;
	if (life == 2) hideObject(heart3);
	else if (life == 1) hideObject(heart2);
	else if (life == 0) {
		hideObject(heart1);
		ending(false);
	}
}

void zeroCheck() {			// 다음 오브젝트가 없을 때 변수 clear 증가 함수
	if (clear != 120 && arr[clear] == 0) {
		clear++;
		if (arr[clear] != 0) return;
		zeroCheck();
	}
}

void normalHitPlay() {		// 평타 효과음 재생
	switch (NhitCount) {
	case 0:
		playSound(normalHit, false);
		NhitCount++;
		break;
	case 1:
		playSound(normalHit1, false);
		NhitCount++;
		break;
	case 2:
		playSound(normalHit2, false);
		NhitCount++;
		break;
	case 3:
		playSound(normalHit3, false);
		NhitCount++;
		break;
	case 4:
		playSound(normalHit4, false);
		NhitCount = 0;
		break;
	}
}

void finalHitPlay() {		// 막타 효과음 재생
	switch (FhitCount) {
	case 0:
		playSound(finalHit, false);
		FhitCount++;
		break;
	case 1:
		playSound(finalHit1, false);
		FhitCount = 0;
		break;
	}
}


void judge(int num1, int num2) {	// 클릭 시 정,오답 확인

	if (clear == 120) return;
	else if (life == 0) return;
	else if (arr[clear] == num1) {
		hideObject(enemy[clear]);
		clear++;
		zeroCheck();
		normalHitPlay();
		if (clear == 120) ending(true);
	}
	else if (arr[clear] == num2) {
		Elife[clear]--;
		if (Elife[clear] != 0) normalHitPlay();
		if (Elife[clear] == 0) {
			hideObject(enemy[clear]);
			clear++;
			zeroCheck();
			finalHitPlay();
			if (clear == 120) ending(true);
		}
	}
	else {
		minusHeart();
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startbutton) {
		enterScene(scene2);

		patternMaker();
		enemyLife();
		createEnemy();

		zeroCheck();

		playSound(bgm, true);

		setTimer(check, duration);
		startTimer(check);
	}

	else if (object == restartbutton) {
		for (int i = 0; i < 150; i++) enemy[i] = 0;
		for (int i = 0; i < 150; i++) arr[i] = 0;
		for (int i = 0; i < 150; i++) Elife[i] = 0;
		for (int i = 0; i < 150; i++) locate[i] = 0;
		for (int i = 0; i < 30; i++) name[i] = 0;
		count = 0;
		clear = 0;
		life = 3;

		patternMaker();
		enemyLife();
		createEnemy();

		hideObject(restartbutton);
		hideObject(endbutton);

		showObject(heart1);
		showObject(heart2);
		showObject(heart3);

		zeroCheck();

		if (bgmloop == false) {
			stopSound(bgm);
			playSound(bgm, true);
		}
		

		setTimer(check, duration);
		startTimer(check);
	}

	else if (object == endbutton) {

		endGame();
	}

	else if (object == normal) {

		judge(1, 5);
	}

	else if (object == blue) {

		judge(2, 6);
	}

	else if (object == red) {

		judge(3, 7);
	}

	else if (object == yellow) {

		judge(4, 8);
	}
}

void timerCallback(TimerID timer) {

	if (arr[count] != 0) {
		showObject(enemy[count]);
	}

	for (int i = 0; i < count + 1; i++) {
		if (arr[i] != 0 && i >= clear) {
			locate[i] += location;
			locateObject(enemy[i], scene2, 610, 720 - locate[i]);
			if (locate[i] > 640) {
				life = 0;
				hideObject(heart1);
				hideObject(heart2);
				hideObject(heart3);
				ending(false);
				return;
			}
		}
	}

	count++;

	if (clear != 120) {
		setTimer(check, duration);
		startTimer(check);
	}
}

int main() {

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene1 = createScene("스피드 스워드", "images/cover.png");
	scene2 = createScene("적을 모두 무찌르세요!", "images/cover1.png");

	startbutton = createObject("images/start.png", scene1, 610, 70, true);
	restartbutton = createObject("images/restart.png", scene2, 610, 400, false);
	endbutton = createObject("images/end.png", scene2, 610, 350, false);

	heart1 = createObject("images/heart.png", scene2, 830, 650, true);
	scaleObject(heart1, 0.05f);
	heart2 = createObject("images/heart.png", scene2, 900, 650, true);
	scaleObject(heart2, 0.05f);
	heart3 = createObject("images/heart.png", scene2, 970, 650, true);
	scaleObject(heart3, 0.05f);

	normal = createObject("images/N.png", scene2, 370, 40, true);
	blue = createObject("images/B.png", scene2, 470, 40, true);
	red = createObject("images/R.png", scene2, 730, 40, true);
	yellow = createObject("images/Y.png", scene2, 830, 40, true);

	bgm = createSound("sound/BGM.wav");
	normalHit = createSound("sound/normalHit.wav");
	normalHit1 = createSound("sound/normalHit1.wav");
	normalHit2 = createSound("sound/normalHit2.wav");
	normalHit3 = createSound("sound/normalHit3.wav");
	normalHit4 = createSound("sound/normalHit4.wav");
	finalHit = createSound("sound/finalHit.wav");
	finalHit1 = createSound("sound/finalHit1.wav");

	check = createTimer(duration);

	startGame(scene1);

}