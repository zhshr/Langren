#pragma once
#include "stdafx.h"
#include <map>
#include "cqp.h"
#include "Role.h"
#include <unordered_map>

using namespace std;

struct player {
	string nickName;
	int role;
	bool police;
};
typedef unordered_map<int64_t, player> playerMap;
class CGame
{
public:
	static const int STATE_ENROLL = 1;
	static const int STATE_ASK_ROLE = 2;
	static const int STATE_DISPATCH_ROLE = 3;
	
	int64_t GroupId;
	int64_t AdminId;
	playerMap players;
	bool enable[CRole::TOTAL];
	vector<int> roleCounts;
	string PlayersString();
	int state;

	CGame(int64_t fromGroup, int64_t fromQQ);
	~CGame();	
	int AddPlayer(int64_t fromQQ, string nickName);
	void AutoAdjustRole();
	bool ToggleRoleByStr(string str, bool isEnable);
};

