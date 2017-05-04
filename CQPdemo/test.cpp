
#include "stdafx.h"
#include "string"
#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <sstream>
using namespace std;

int ac = 0;
void CQ_sendGroupMsg(int ac, int qun, const char* msg) {
	string aa = msg;
	cout << "群发：" << aa << endl;
}

void CQ_sendPrivateMsg(int ac, int qun, const char* msg) {
	string aa = msg;
	cout << "私聊：" << aa << endl;
}




static  string flag[15] = { "3","4","5","6","7","8","9","10","J","Q","K","A","2", "鬼","王" };

const int STATE_WAIT = 0;
const int STATE_START = 1;
const int STATE_BOSSING = 2;
const int STATE_GAMEING = 2;

int findAndRemove(vector<string> &dest, string str) {

	for (unsigned i = 0; i < dest.size(); i++) {
		if (dest.at(i) == str) {
			vector<string>::iterator it = dest.begin() + i;
			dest.erase(it);
			return i;
		}
	}
	return -1;
}

int find(vector<string> &dest, string str) {

	for (unsigned i = 0; i < dest.size(); i++) {
		if (dest.at(i) == str) {
			return i;
		}
	}
	return -1;
}

int findFlag(string str)
{

	for (int i = 0; i < 15; i++) {
		if (flag[i] == str) {
			return i;
		}
	}
	return -1;

}

int desc(int a, int b)
{
	return a>b;
}

int asc(int a, int b)
{
	return a<b;
}

bool compareCard(const string &carda, const string &cardb)
{
	return findFlag(carda) < findFlag(cardb);
}


void trim(string &s)
{

	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}

}

void toUpper(string &str) {
	transform(str.begin(), str.end(), str.begin(), ::toupper);
}


class Player
{
public:
	stringstream msg;
	int64_t number;
	vector<string> card;
	int32_t socre = 5000;
	bool isReady = false;

	void sendMsg();
	void listCards();
	void breakLine();
};



class Desk {
public:
	//string cards[54] = {
	//	"10","j","10","j","10","j","4","5","4","5","4","5",
	//	"2","3","6","7","8","9","q","k","a",
	//	"2","3","6","7","8","9","q","k","a",
	//	"2","3","6","7","8","9","q","k","a",
	//	"2","3","4","5","6","7","8","9","10","j","q","k","a",
	//	"鬼","王"
	//};
	string cards[54] = {
		"2","3","4","5","6","7","8","9","10","J","Q","K","A",
		"2","3","4","5","6","7","8","9","10","J","Q","K","A",
		"2","3","4","5","6","7","8","9","10","J","Q","K","A",
		"2","3","4","5","6","7","8","9","10","J","Q","K","A",
		"鬼","王"
	};
	int64_t number;
	vector<Player*> players;

	int state = 0;
	int lastPlayIndex = -1;//当前谁出得牌
	int nextPlayIndex = -1;//该谁出牌
	int bossIndex = -1;//谁是地主

	vector<string> lastCard;//上位玩家的牌
	string lastCardType = "";//上位玩家得牌类
	vector<int> *lastWeights = new vector<int>;//上位玩家的牌

	stringstream msg;

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
	void play(int64_t playNum, string msg);
	void play(int64_t playNum, vector<string> list);//出牌
	void discard(int64_t playNum);

	void at(int64_t playNum);
	void breakLine();
	int getPlayer(int64_t number);//按qq号获得玩家得索引
	void setNextPlayerIndex();//设置下个出牌得玩家索引
	void listPlayers();
	void listPlayers(int type);
	bool isCanWin(int cardCount, vector<int> *Weights, string type);
	string getMycardType(vector<string> list, vector<int> *Weights);
	void sendMsg();
	void sendPlayerMsg();

};

class Desks {
public:
	vector<Desk*> desks;
	Desk* getOrCreatDesk(int64_t deskNum);
	void game(int64_t deskNum, int64_t playNum, const char *msg);
	int getDesk(int64_t deskNum);
};


static Desks datas;


void Player::sendMsg()
{
	string tmp = this->msg.str();
	if (tmp.empty()) {
		return;
	}
	int length = tmp.length();
	if (tmp[length - 2] == '\r' && tmp[length - 1] == '\n') {
		tmp = tmp.substr(0, length - 2);
	}
	CQ_sendPrivateMsg(ac, this->number, tmp.data());
	this->msg.str("");
}

void Desk::at(int64_t playNum)
{
	this->msg << "[CQ:at,qq=" << playNum << "]";
}

void Desk::breakLine()
{
	this->msg << "\r\n";
}

int Desk::getPlayer(int64_t number) {
	for (unsigned i = 0; i < players.size(); i++) {
		if (players[i]->number == number) {
			return i;
		}
	}
	return -1;
}


void Desk::listPlayers()
{
	this->msg << "玩家:";
	this->breakLine();
	this->listPlayers(1);
}

void Desk::listPlayers(int type)
{

	bool hasType = (type & 1) == 1;
	bool hasOwn = ((type >> 1) & 1) == 1;

	for (unsigned i = 0; i < this->players.size(); i++) {
		this->msg << i + 1 << ":";
		if (hasType) {
			this->msg << "[" << (i == this->bossIndex && state != STATE_GAMEING ? "地主" : "农民") << "]";
		}

		this->msg << "[CQ:at,qq=" << this->players[i]->number << "]";
		if (hasOwn) {
			if (this->bossIndex == this->nextPlayIndex) {//如果是地主赢了
				this->msg << "[" << (i == this->bossIndex ? "胜利" : "失败") << "]";
			}
			else {
				this->msg << "[" << (i == this->bossIndex ? "失败" : "胜利") << "]";
			}
		}

		this->breakLine();
	}
}

bool Desk::isCanWin(int cardCount, vector<int> *weights, string type)
{

	if (type == "" || this->lastCardType == "王炸") {
		return false;
	}

	if (this->lastCardType == "") {
		return true;
	}

	if (type == "王炸") {
		return true;
	}
	if (type == "炸弹" && type != this->lastCardType) {
		return true;
	}


	if (type == this->lastCardType && cardCount == this->lastCard.size()) {

		for (unsigned i = 0; i < weights->size(); i++) {
			if (weights[i] > this->lastWeights[i]) {
				return true;
			}
		}

	}


	return false;
}



string Desk::getMycardType(vector<string> list, vector<int> *weights)
{
	int cardCount = list.size();
	sort(list.begin(), list.end(), compareCard);

	if (cardCount == 2 && findFlag(list[0]) + findFlag(list[1]) == 29) {//王炸
		return "王炸";
	}

	vector<string> cards;
	vector<int> counts;

	for (unsigned i = 0; i < list.size(); i++) {
		int index = find(cards, list[i]);
		if (index == -1) {
			cards.push_back(list[i]);
			counts.push_back(1);
		}
		else {
			counts[index] = counts[index] + 1;
		}
	}

	int max = counts[0];//存放大值
	int min = counts[0];//存放小值
	int cardGroupCout = cards.size();
	int tmp;
	for (unsigned i = 0; i < counts.size(); i++) {
		tmp = counts[i];
		if (tmp > max) {
			max = tmp;
		}
		if (tmp < min) {
			min = tmp;
		}
	}

	vector<int> tmpCount(counts);
	sort(tmpCount.begin(), tmpCount.end(), desc);
	if (cardCount == 1) {//单牌
		weights->push_back(findFlag(cards[0]));
		return "单牌";
	}
	if (cardCount == 2 && max == 2) {//对子
		weights->push_back(findFlag(cards[0]));
		return "对子";
	}
	if (cardCount == 3 && max == 3) {//三张
		weights->push_back(findFlag(cards[0]));
		return "三张";
	}
	if (cardCount == 4 && max == 4) {//炸弹
		weights->push_back(findFlag(cards[0]));
		return "炸弹";
	}

	if (cardCount == 4 && max == 3) {//3带1

		for (unsigned i = 0; i < tmpCount.size(); i++) {
			int tmp = tmpCount[i];
			for (unsigned m = 0; m < counts.size(); m++) {
				if (counts[m] == tmp) {
					weights->push_back(findFlag(cards[m]));
					counts[m] = -1;
				}
			}
		}

		return "3带1";
	}

	if (cardCount == 5 && max == 3 && min == 2) {//3带2
		for (unsigned i = 0; i < tmpCount.size(); i++) {
			int tmp = tmpCount[i];
			for (unsigned m = 0; m < counts.size(); m++) {
				if (counts[m] == tmp) {
					weights->push_back(findFlag(cards[m]));
					counts[m] = -1;
				}
			}
		}

		return "3带2";
	}

	if (cardCount == 6 && max == 4) {//4带2
		for (unsigned i = 0; i < tmpCount.size(); i++) {
			int tmp = tmpCount[i];
			for (unsigned m = 0; m < counts.size(); m++) {
				if (counts[m] == tmp) {
					weights->push_back(findFlag(cards[m]));
					counts[m] = -1;
				}
			}
		}

		return "4带2";
	}

	if (cardGroupCout > 2 && max == 2 && min == 2
		&& findFlag(cards[0]) == findFlag(cards[cardGroupCout - 1]) - cardGroupCout + 1
		&& findFlag(cards[cardGroupCout - 1]) < 13
		) {//连对
		for (unsigned i = 0; i < tmpCount.size(); i++) {
			int tmp = tmpCount[i];
			for (unsigned m = 0; m < counts.size(); m++) {
				if (counts[m] == tmp) {
					weights->push_back(findFlag(cards[m]));
					counts[m] = -1;
				}
			}
		}

		return "连对";
	}

	if (cardGroupCout > 4 && max == 1 && min == 1
		&& findFlag(cards[0]) == findFlag(cards[cardGroupCout - 1]) - cardGroupCout + 1
		&& findFlag(cards[cardGroupCout - 1]) < 13
		) {//顺子
		for (unsigned i = 0; i < tmpCount.size(); i++) {
			int tmp = tmpCount[i];
			for (unsigned m = 0; m < counts.size(); m++) {
				if (counts[m] == tmp) {
					weights->push_back(findFlag(cards[m]));
					counts[m] = -1;
				}
			}
		}

		return "顺子";
	}

	//飞机
	int  planeCount = 0;
	for (unsigned i = 0; i < counts.size() && counts[i] >= 3; i++, planeCount++);
	if (planeCount>1) {
		string tmp;
		if (cardCount == planeCount * 3) {
			tmp = "飞机";
		}
		else if (cardCount == planeCount * 4) {
			tmp = "单翅飞机";
		}
		else if (cardCount == planeCount * 5 && min == 2) {
			tmp = "双翅飞机";
		}

		for (int i = 0; i < planeCount; i++) {
			int tmp = tmpCount[i];
			for (unsigned m = 0; m < counts.size(); m++) {
				if (counts[m] == tmp) {
					weights->push_back(findFlag(cards[m]));
					counts[m] = -1;
				}
			}
		}

		sort(weights->begin(), weights->end(), desc);

		int weightscount = weights->size();

		if (weights->at(0) - weightscount + 1 != weights->at(weightscount - 1)) {
			return "";
		}

		return tmp;
	}
	return "";
}





void Desk::sendMsg()
{

	string tmp = this->msg.str();
	if (tmp.empty()) {
		return;
	}
	int length = tmp.length();
	if (tmp[length - 2] == '\r' && tmp[length - 1] == '\n') {
		tmp = tmp.substr(0, length - 2);
	}
	CQ_sendGroupMsg(ac, this->number, tmp.data());
	this->msg.str("");
}

void Desk::sendPlayerMsg()
{
	for (unsigned i = 0; i < this->players.size(); i++) {
		players[i]->sendMsg();
	}
}



void Desk::shuffle() {
	srand((unsigned)time(NULL));
	for (unsigned i = 0; i < 54; i++) {
		swap(this->cards[i], this->cards[rand() % 54]);
	}
}

void Desk::creataBoss() {

	state = STATE_BOSSING;

	int index = rand() % 3;

	this->bossIndex = index;
	this->nextPlayIndex = index;
	this->at(this->players[index]->number);
	this->msg << "你是否要抢地主";
	this->breakLine();
}

void Desk::getBoss(int64_t playerNum)
{
	int index = this->getPlayer(playerNum);
	if (this->state == STATE_BOSSING && this->nextPlayIndex == index) {

		this->bossIndex = index;
		sendBossCard();
	}
}

void Desk::dontBoss(int64_t playerNum)
{
	int index = this->getPlayer(playerNum);
	if (this->state == STATE_BOSSING && this->nextPlayIndex == index) {

		this->setNextPlayerIndex();

		if (this->nextPlayIndex == this->bossIndex) {
			this->sendBossCard();
		}
		else {
			this->msg << "[CQ:at,qq=" << this->players[index]->number << "] "
				<< "不抢，"
				<< "[CQ:at,qq=" << this->players[nextPlayIndex]->number << "] "
				<< "你是否要抢地主";
		}
	}
}

void Desk::sendBossCard()
{
	Player *playerBoss = players[this->bossIndex];

	this->msg << "[CQ:at,qq=" << playerBoss->number << "] "
		<< "是地主底牌是："
		<< "[" << this->cards[53] << "]"
		<< "[" << this->cards[52] << "]"
		<< "[" << this->cards[51] << "]"
		<< "请出牌";
	this->breakLine();

	for (int i = 0; i < 3; i++) {
		playerBoss->card.push_back(cards[53 - i]);
	}
	sort(playerBoss->card.begin(), playerBoss->card.end(), compareCard);

	playerBoss->msg << "你是地主，收到底牌：";
	for (unsigned m = 0; m < playerBoss->card.size(); m++) {
		playerBoss->msg << "[" << playerBoss->card.at(m) << "]";
	}
	playerBoss->breakLine();

	state = STATE_GAMEING;
}

void Desk::play(int64_t playNum, string msg)
{

	int length = msg.length();
	char *msgs = new char[length];
	strncpy(msgs, msg.data(), length);
	vector<string> msglist;

	stringstream data;
	string msgFindTmp;
	for (int i = 2; i < length; i++) {

		data << msgs[i];
		msgFindTmp = msgs;
		int gIndex = msgFindTmp.find("鬼");
		int wIndex = msgFindTmp.find("王");
		int tenIndex = msgFindTmp.find("10");
		if (i == gIndex || i == wIndex || i == tenIndex) {
			data << msgs[i + 1];
			msgs[i++] = -1;
			msgs[i] = -1;
		}

		msglist.push_back(data.str());
		data.str("");
	}

	this->play(playNum, msglist);

}

void Desk::play(int64_t playNum, vector<string> list)
{

	int playIndex = this->getPlayer(playNum);

	if (playIndex == -1 || playIndex != this->nextPlayIndex || this->state != STATE_GAMEING) {
		return;
	}
	Player *player = this->players[playIndex];
	vector<string> mycardTmp(player->card);

	int cardCount = list.size();

	for (int i = 0; i < cardCount; i++) {
		if (findAndRemove(mycardTmp, list[i]) == -1) {
			this->msg << "你就没有你出的牌，会不会玩？";
			return;
		}
	}

	vector<int> *weights = new vector<int>;

	string type = this->getMycardType(list, weights);

	bool isCanWin = this->isCanWin(cardCount, weights, type);

	if (isCanWin) {
		if (mycardTmp.size() == 0) {//赢了。
			this->msg << "游戏结束";
			this->breakLine();
			this->listPlayers(3);

			this->msg << (this->bossIndex == this->nextPlayIndex ? "地主赢了" : "农民赢了");
			this->gameOver(this->number);
			return;
		}
		player->card = mycardTmp;
		this->lastWeights = weights;
		this->lastCard = list;
		this->lastCardType = type;
		this->lastPlayIndex = this->nextPlayIndex;


		player->listCards();

		if (player->card.size() < 3) {
			this->at(this->lastPlayIndex);
			this->msg << "仅剩下" << player->card.size() << "张牌";
			this->breakLine();
		}

		this->msg << "上回合：" << this->lastCardType;
		for (unsigned m = 0; m < this->lastCard.size(); m++) {
			this->msg << "[" << this->lastCard.at(m) << "]";
		}
		this->breakLine();
		this->setNextPlayerIndex();
		this->at(this->players[this->nextPlayIndex]->number);
		this->msg << "请出牌";
		this->breakLine();
	}
	else {
		this->msg << "别瞎几把出牌";
		this->breakLine();
	}
}

void Desk::discard(int64_t playNum)
{
	if (this->nextPlayIndex != this->getPlayer(playNum) || this->state != STATE_GAMEING) {
		return;
	}

	if (this->nextPlayIndex == this->lastPlayIndex) {
		this->msg << "过过过过你妹，会不会玩，你不能过牌了";
		return;
	}


	this->msg << "上回合：" << this->lastCardType;
	for (unsigned m = 0; m < this->lastCard.size(); m++) {
		this->msg << "[" << this->lastCard.at(m) << "]";
	}
	this->breakLine();
	this->msg << "上位玩家：过牌";
	this->breakLine();
	this->setNextPlayerIndex();
	this->at(this->players[this->nextPlayIndex]->number);
	this->msg << "请出牌";
	this->breakLine();
}

void Desk::gameOver(int64_t number)
{
	int index = datas.getDesk(number);
	if (index == -1) {
		return;
	}
	vector<Desk*>::iterator it = datas.desks.begin() + index;
	datas.desks.erase(it);
	CQ_sendGroupMsg(ac, number, "游戏结束");
}

void Desk::exit(int64_t number)
{
	if (this->state == STATE_WAIT) {
		int index = this->getPlayer(number);
		if (index != -1) {
			vector<Player*>::iterator it = this->players.begin() + index;
			this->players.erase(it);
			this->msg << "退出成功";
			this->breakLine();
		}

	}
	else {
		this->msg << "游戏已经开始不能退出";
		this->breakLine();
	}
}

void Desk::commandList()
{
	int i = 1;
	this->msg
		<< i++ << " " << "上桌：加入游戏"
		<< i++ << " " << "出：出牌 比如 出23456"
		<< i++ << " " << "过：过牌"
		<< i++ << " " << "抢地主 | 不抢：是否抢地主"
		<< i++ << " " << "开始游戏：是否开始游戏"
		<< i++ << " " << "下桌：退出游戏，只能在准备环节使用"
		<< i++ << " " << "玩家列表：当前在游戏中得玩家信息"
		<< i++ << " " << "记牌器：显示已经出过的牌";
	this->breakLine();
}

void Desk::setNextPlayerIndex()
{
	this->nextPlayIndex = this->nextPlayIndex == 2 ? 0 : this->nextPlayIndex + 1;

	if (this->nextPlayIndex == this->lastPlayIndex) {
		this->lastCard.clear();
		this->lastWeights->clear();
		this->lastCardType = "";
	}


}

void Desk::deal() {
	unsigned i, k, j;

	for (i = k = 0; i < 3; i++) {
		Player *player = players[i];

		for (j = 0; j < 17; j++) {
			player->card.push_back(cards[k++]);
		}

		sort(player->card.begin(), player->card.end(), compareCard);

		for (unsigned m = 0; m < player->card.size(); m++) {
			player->msg << "[" << player->card.at(m) << "]";
		}

		player->breakLine();
	}

}

int Desks::getDesk(int64_t deskNum) {

	for (unsigned i = 0; i < this->desks.size(); i++) {
		if (this->desks[i]->number == deskNum) {
			return i;
		}
	}

	return -1;
}

void Desk::join(int64_t playNum)
{

	int playIndex = this->getPlayer(playNum);

	if (playIndex != -1) {
		this->msg << "[CQ:at,qq=" << playNum << "] 已经加入游戏";
		this->breakLine();
		return;
	}

	if (this->players.size() == 3) {
		this->msg << "[CQ:at,qq=" << playNum << "] 人数已满";
		this->breakLine();
		return;
	}

	Player *player = new Player;
	player->number = playNum;
	this->players.push_back(player);

	this->msg << "[CQ:at,qq=" << playNum << "] "
		<< "加入成功，已有玩家:" << this->players.size() << "个";
	this->breakLine();
	if (this->players.size() == 3) {
		this->msg << "请输入[开始游戏]";
		this->breakLine();
	}
}

Desk* Desks::getOrCreatDesk(int64_t deskNum) {

	Desk *desk = NULL;
	int deskIndex = getDesk(deskNum);
	if (deskIndex == -1) {//没有桌子
		desk = new Desk;
		desk->number = deskNum;
		desks.push_back(desk);
	}
	else {
		desk = datas.desks[deskIndex];
	}

	return desk;
}
void Desk::startGame() {
	if (this->players.size() == 3 && this->state == STATE_WAIT) {
		this->state = STATE_START;
		this->msg << "游戏开始";
		this->breakLine();

		this->listPlayers();

		this->shuffle();

		this->deal();

		this->creataBoss();
	}
	else {
		this->msg << "没有足够的玩家或者已经开始游戏";
		this->breakLine();
		this->listPlayers();
	}
}

void Desks::game(int64_t deskNum, int64_t playNum, const char* msgArray) {

	string msg = msgArray;
	trim(msg);
	toUpper(msg);

	Desk *desk = datas.getOrCreatDesk(deskNum);

	if (msg == "上桌" || msg == "加入游戏" || msg == "JOIN") {
		desk->join(playNum);
	}
	else if (msg.find("出") == 0 || msg.find("出牌") == 0) {//出牌阶段
		desk->play(playNum, msg);
	}
	else if (msg == "过" || msg == "过牌" || msg == "不出" || msg == "不要" || msg == "PASS") {//跳过出牌阶段
		desk->discard(playNum);
	}
	else if (msg == "退出游戏" || msg == "退桌" || msg == "下桌") {//结束游戏
		desk->exit(playNum);
	}
	else if (msg == "命令列表") {
		desk->commandList();
	}
	else if ((msg == "结束游戏" || msg == "GAMEOVER") && playNum == 895252155) {//结束游戏
		desk->gameOver(deskNum);
		return;
	}
	else if (msg == "玩家列表") {
		desk->listPlayers();
	}
	else if (msg == "开始游戏") {
		desk->startGame();
	}
	else if (msg == "抢地主" || msg == "抢") {
		desk->getBoss(playNum);
	}
	else if (msg == "不抢") {
		desk->dontBoss(playNum);
	}
	else if (msg == "记牌器") {
		desk->msg << "未开发完成";
	}

	desk->sendMsg();
	desk->sendPlayerMsg();
}

void Player::listCards()
{
	for (unsigned m = 0; m < this->card.size(); m++) {
		this->msg << "[" << this->card.at(m) << "]";
	}
	this->breakLine();

}

void Player::breakLine()
{
	this->msg << "\r\n";
}


int main() {


	string info = "你好";
	int64_t desknum = 10000000;
	int64_t playNum = 111111;
	datas.game(desknum, playNum, "玩家列表");

	for (int i = 0; i < 3; i++) {
		datas.game(desknum, playNum + i, "上桌");
	}

	datas.game(desknum, playNum, "开始游戏");

	while (true) {
		char str[30];
		cin.getline(str, 30);

		int index = datas.desks[datas.getDesk(desknum)]->nextPlayIndex;
		datas.game(desknum, playNum + index, str);

	}

	system("pause");
	return 0;
};

