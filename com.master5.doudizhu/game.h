
#include "stdafx.h"
#include "string"
#ifdef _DEBUG 
#else
#include "cqp.h"
#endif 
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <tchar.h>  
using namespace std;
static const wstring  cardDest[54] = {
	L"鬼",L"王",
	L"2",L"3",L"4",L"5",L"6",L"7",L"8",L"9",L"10",L"J",L"Q",L"K",L"A",
	L"2",L"3",L"4",L"5",L"6",L"7",L"8",L"9",L"10",L"J",L"Q",L"K",L"A",
	L"2",L"3",L"4",L"5",L"6",L"7",L"8",L"9",L"10",L"J",L"Q",L"K",L"A",
	L"2",L"3",L"4",L"5",L"6",L"7",L"8",L"9",L"10",L"J",L"Q",L"K",L"A"

	//"2","3","3","3","4","4","4","5","5","5","6","6","7","7","8","8","9","10","J","Q","K","A","鬼","王",
	//"2","9","10","J","Q","K","A",
	//"2","6","7","8","9","10","J","Q","K","A",
	//"2","3","4","5","6","7","8","9","10","J","Q","K","A",
};

static const wstring flag[15] = { L"3",L"4",L"5",L"6",L"7",L"8",L"9",L"10",L"J",L"Q",L"K",L"A",L"2",L"鬼",L"王" };

const int STATE_WAIT = 0;
const int STATE_START = 1;
const int STATE_BOSSING = 2;
const int STATE_GAMEING = 3;

const wstring configPath = L".\\app\\com.master5.doudizu\\config.ini";
const wstring configDir = L".\\app\\com.master5.doudizu\\";

class Util {
public:
	static int AC;
	static void testMsg(int64_t playNum, int64_t desknum, const char * str);
	static void sendGroupMsg(int64_t groupid, const char *msg);
	static void sendPrivateMsg(int64_t groupid, const char *msg);
	static int  findAndRemove(vector<wstring> &dest, wstring str);
	static int  find(vector<wstring> &dest, wstring str);
	static int  findFlag(wstring str);
	static int  desc(int a, int b);
	static int  asc(int a, int b);
	static bool  compareCard(const wstring &carda, const wstring &cardb);
	static void  trim(wstring &s);
	static void  toUpper(wstring &str);
	static void setAC(int32_t ac);
	static string wstring2string(wstring wstr);
	static wstring string2wstring(string str);
	static void mkdir();
};
class Config {
public:
	static int64_t readAdmin();
	static bool writeAdmin(int64_t playerNum);
	static int64_t readScore(int64_t playerNum);
	static bool getScore(int64_t playerNum);
	static int addScore(int64_t playerNum, int score);
	static bool IAmAdmin(int64_t playerNum);
	static bool resetGame(int64_t playNum);
	static wstring readString();
private:
	static bool writeScore(int64_t playerNum, int score);
};

class Player
{
public:
	Player();
	wstringstream msg;
	int64_t number;
	vector<wstring> card;
	bool isReady;
	bool isOpenCard;//明牌状态
	bool isSurrender;//投降状态
	void sendMsg();
	void listCards();
	void breakLine();
};



class Desk {
public:

	Desk();
	int score;
	wstring cards[54];
	int64_t number;
	vector<Player*> players;

	int whoIsWinner;
	int state;
	int lastPlayIndex;//当前谁出得牌
	int currentPlayIndex;//该谁出牌
	int bossIndex;//谁是地主

	vector<wstring> lastCard;//上位玩家的牌
	wstring lastCardType;//上位玩家得牌类
	vector<int> *lastWeights;//上位玩家的牌的权重

	wstringstream msg;

	void join(int64_t playNum);
	void startGame();
	static void gameOver(int64_t number);
	void exit(int64_t playNum);
	void commandList();

	void shuffle();//洗牌
	void deal();//发牌
	void creataBoss();//抢地主
	void getBoss(int64_t playerNum);
	void dontBoss(int64_t playerNum);
	void sendBossCard();
	void play(int64_t playNum, wstring msg);
	void play(int64_t playNum, vector<wstring> list);//出牌
	void discard(int64_t playNum);
	void surrender(int64_t playNum);//投降
	void openCard(int64_t playNum);//明牌

	void getPlayerInfo(int64_t playNum);
	void getScore(int64_t playNum);

	void at(int64_t playNum);
	void breakLine();
	int getPlayer(int64_t number);//按qq号获得玩家得索引
	void setNextPlayerIndex();//设置下个出牌得玩家索引
	void listPlayers();
	void listPlayers(int type);
	bool isCanWin(int cardCount, vector<int> *Weights, wstring type);
	wstring getMycardType(vector<wstring> list, vector<int> *Weights);
	void sendMsg();
	void sendPlayerMsg();
	void listCardsOnDesk(Player* player);

};

class Desks {
public:
	vector<Desk*> desks;
	Desk* getOrCreatDesk(int64_t deskNum);
	static void game(int64_t deskNum, int64_t playNum, const char *msg);
	static bool game(int64_t playNum, const char *msg);
	int getDesk(int64_t deskNum);
	void listDesks();
};