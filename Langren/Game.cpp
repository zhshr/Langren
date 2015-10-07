#include "stdafx.h"
#include "Game.h"
#include <iostream>


CGame::CGame(int64_t fromGroup, int64_t qqId)
{
	this->GroupId = fromGroup;
	this->AdminId = qqId;
	state = STATE_ENROLL;
}


CGame::~CGame()
{
}

bool CGame::ToggleRoleByStr(string str, bool isEnable) {
	for (int i = 0; i < CRole::TOTAL; i++) {
		if (CRole::names[i] == str) {
			enable[i] = isEnable;
			return true;
		}
	}
	return false;
}
void CGame::AutoAdjustRole() {
	for (int i = 0; i < CRole::TOTAL; i++) {
		enable[i] = false;
	}
	enable[CRole::VILLAGER] = true;
	enable[CRole::WOLF] = true;
	enable[CRole::SORCERER] = true;
	enable[CRole::PROPHET] = true;
	int playerCount = players.size();
	if (playerCount >= 7) {
		enable[CRole::CUPID] = true;
	}
	if (playerCount >= 8) {
		enable[CRole::HUNTER] = true;
	}
	if (playerCount >= 10) {
		enable[CRole::IDIOT] = true;
	}
	if (playerCount >= 12) {
		enable[CRole::GUARD] = true;
	}
}
string CGame::PlayersString()
{
	string result;
	playerMap::iterator p;
	int i = 0;
	for (p = players.begin(); p != players.end(); p++) {
		result = result + (std::to_string(++i) + " £º " + std::to_string(p->first) + " êÇ³ÆÎª " + p->second.nickName + "\n");
	}
	return result;
}


int CGame::AddPlayer(int64_t fromQQ, string nickName)
{
	playerMap::iterator p;
	if (nickName.find("[CQ:") != string::npos) {
		return false;
	}
	for (p = players.begin(); p != players.end(); p++) {
		if (p->first == fromQQ) {
			return false;
		}
	}
	player pl;
	pl.nickName = nickName;
	players.insert(pair<int64_t, player>(fromQQ, pl));
	return TRUE;
}
