// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Langren/Game.h"
#include "../Langren/Role.h"
#include <stdio.h>
#include <iostream>
#include <map>


void testCGame() {
	map<int64_t, CGame> gameList;
	CGame *game = new CGame(123,222);
	gameList.insert(pair<int64_t, CGame>(123, *game));
	delete game;
	CGame *another = &(gameList.begin()->second);
	gameList.begin()->second.AddPlayer(234, "bbb");
	another->AddPlayer(1234, "abcd");
	cout << ("报名结束，共有如下玩家报名" + (another->PlayersString())).c_str();
}
void testCRole() {
	vector<int> test(9);
	CRole::RecommendVectorToString(test);
}
int _tmain(int argc, _TCHAR* argv[])
{

	testCRole();
	getchar();
	return 0;
}

