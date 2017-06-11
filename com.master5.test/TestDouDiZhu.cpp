#pragma comment(lib,"json_vc71_libmtd.lib")
#include "stdafx.h"
#include "../com.master5.doudizhu/game.h"
#include <fstream>  
//包含rapidjson必要头文件,rapidjson文件夹拷贝到工程目录，或者设置include路径，或者加入到工程树  
#include "../com.master5.doudizhu/rapidjson/document.h"  
#include "../com.master5.doudizhu/rapidjson/prettywriter.h"  
#include "../com.master5.doudizhu/rapidjson/stringbuffer.h"  
using namespace std;
using namespace rapidjson;

void testGame() {

	string info = "你好";
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
		cout << str << endl;

		Util::testMsg(desknum, playNum, str);

	}

}

void rapidjson_test_char()
{
	// 1. Parse a JSON string into DOM.
	const char* json1 = "{\"project\":\"rapidjson\",\"stars\":10}";
	Document d;
	d.Parse(json1);

	// 2. Modify it by DOM.
	Value& s = d["stars"];
	s.SetInt(s.GetInt() + 1);

	// 3. Stringify the DOM
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	// Output {"project":"rapidjson","stars":11}

	std::cout << buffer.GetString() << std::endl;

	printf("\n");
	//解析第二个例子
	//{"dictVersion": 1,"key": "word","value": "test"}
	const char* json2 = "{\"dictVersion\":1,\"key\":\"word\",\"value\":\"test\"}";
	d.Parse(json2);
	if (d.HasParseError())
	{
		printf("Parse error!!\n");
		return;
	}

	if (d.HasMember("dictVersion"))
	{
		printf("The dictVersion is:%d\n", d["dictVersion"].GetInt());
	}

	if (d.HasMember("key"))
	{
		printf("The key is:%s\n", d["key"].GetString());
	}

	if (d.HasMember("value"))
	{
		printf("The value is:%s\n", d["value"].GetString());
	}
}


int main() {

	testGame();
	//rapidjson_test_char();
	system("pause");



	return 0;
};

