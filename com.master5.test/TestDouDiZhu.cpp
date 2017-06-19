#include "stdafx.h"
#include "../com.master5.doudizhu/game.h"
#include <fstream>
using namespace std;

void testGame() {

	wstring info = L"你好";
	int64_t desknum = 10000000;
	int64_t playNum = 111111;
	Desks::game(desknum, playNum, "玩家列表");

	for (int i = 0; i < 3; i++) {
		Desks::game(desknum, playNum + i, "上桌");
	}

	Desks::game(desknum, playNum, "开始游戏");

	while (true) {
		char str[30];
		cin.getline(str, 30);
		Util::testMsg(desknum, playNum, str);

	}

}

int main() {
	//Desks::game(33333, "增加积分22222");
	//DeleteFile(L".\\com.master5.doudizu\\a.txt");
	//Desks::game(2222,33333, "获取积分");
	
	//
	testGame();
	system("pause");



	return 0;
};

