#pragma once
#include "stdafx.h"
#include <map>

using namespace std;

struct player {
	int role;
	bool police;
};
class CGame
{
public:
	CGame(int64_t fromGroup);
	~CGame();
	int64_t GroupId;
	map<int64_t, player> players;
};

