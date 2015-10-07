#include "stdafx.h"
#include "Langren.h"
#include "util.h"
#include <time.h>
#include <thread>
#include "Role.h"
#include <algorithm>

typedef unordered_map<int64_t, CGame> gameMap;
gameMap gameList;
#define MESSAGE_PREFIX 
// 消息入口
string prefix = "----------印务爹的狼人模块----------\n";

int ProcessMessage(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font)
{
	if (ListPlugins(fromGroup, fromQQ, msg)) {
		return EVENT_IGNORE;
	}
	srand((unsigned int)time(NULL));
	string str = string(msg);
	/*if (str == "禁言测试" && fromGroup != -1) {
		CQ_setGroupBan(ac, fromGroup, fromQQ, 180);
		Sleep(10000);
		CQ_setGroupBan(ac, fromGroup, fromQQ, 0);
		return EVENT_BLOCK;
	}
	*/
	if (fromGroup == -1) {
		//私聊消息

	}
	else {
		//检查此群是否有进行中的游戏
		gameMap::iterator p = gameList.find(fromGroup);
		if (p != gameList.end()) {
			CGame *game = &(p->second);
			switch (game->state) {

			case (CGame::STATE_ENROLL):
				if (str.substr(0, 6) == "！测试" && fromQQ == game->AdminId) {
					int i = std::atoi(str.substr(6, str.length()).c_str());
					addPlayer(game, i);
					return EVENT_BLOCK;
				}
				if (game->AddPlayer(fromQQ, str)) {
					sendQQMessage(fromGroup, fromQQ, (std::to_string(fromQQ) + "报名成功，昵称为" + str).c_str());
				}
				CQ_addLog(ac, CQLOG_INFO, "报名", str.c_str());
				return EVENT_BLOCK;

			case CGame::STATE_ASK_ROLE:
				if (str.substr(0, 6) == "！启用" && fromQQ == game->AdminId) {
					string role = str.substr(6, str.length());
					if (game->ToggleRoleByStr(role, true)) {
						sendQQMessage(fromGroup, fromQQ, (role + "已启用").c_str());
						AskForRoles(game);
						return EVENT_BLOCK;
					}
				}
				if (str.substr(0, 6) == "！禁用" && fromQQ == game->AdminId) {
					string role = str.substr(6, str.length());
					if (game->ToggleRoleByStr(role, false)) {
						sendQQMessage(fromGroup, fromQQ, (role + "已禁用").c_str());
						AskForRoles(game);
						return EVENT_BLOCK;
					}
				}
				if (str == "！确认" && fromQQ == game->AdminId) {
					DispatchRole(game);
					return EVENT_BLOCK;
				}
			}
		}
		else {
			if (str == "！开始") {
				//开始新游戏
				debug("即将进入线程");
				thread *game = new thread(StartGame, fromGroup, fromQQ);
				return EVENT_BLOCK;
			}
		}
	}
	return EVENT_IGNORE;
}
void addPlayer(CGame* game, int count) {
	for (int i = 0; i < count; i++) {
		game->AddPlayer(-(i + 1), std::to_string(-(i + 1)));
	}
}
void StartGame(int64_t groupId, int64_t qqId) {
	debug("已经进入线程");

	//创建游戏实例
	CQ_addLog(ac, CQLOG_INFO, "游戏开始", ("群" + std::to_string(groupId) + "开始狼人游戏").c_str());
	CGame *game = new CGame(groupId, qqId);
	gameList.insert(pair<int64_t, CGame>(groupId, *game));
	string temp = prefix + "检测到开始命令，进入报名模式\n参与的玩家请在1分钟之内回复昵称参与报名\n" + std::to_string(game->AdminId) + "已被设为本局游戏操作者";
	sendQQMessage(groupId, -1, temp.c_str());
	delete game;
	//等待玩家报名
	Sleep(10000);
	gameMap::iterator pl = gameList.find(groupId);
	if (pl != gameList.end()) {
		CGame *game = &(pl->second);
		game->state = game->STATE_DISPATCH_ROLE;
		CQ_addLog(ac, CQLOG_DEBUG, "报名结束", "");
		sendQQMessage(groupId, -1, (prefix + "报名结束，共有如下玩家报名\n" + game->PlayersString()).c_str());
		if (game->players.size() < 6) {
			sendQQMessage(groupId, -1, "报名人数少于6人，游戏无法开始，退出中");
			stopGame(game);
		}
		else {
			
			//修改群名片
			//ModifyCards(game);
			//询问身份设置
			game->AutoAdjustRole();
			game->state = game->STATE_ASK_ROLE;
			AskForRoles(game);
			//进入派发身份阶段
		}
		

	}
	else {
		
	}
	//gameList.erase(pl);
}
string paddeditoa(int64_t i) {
	
	char* temp2 = (char*)malloc(20);
	sprintf_s(temp2, 20, "%02d", i);

	string *temp = new string(temp2);
	free(temp2);
	return *temp;
}

void ModifyCards(CGame* game) {
	debug("准备修改群名片");
	playerMap::iterator p;
	int i = 0;
	for (p = game->players.begin(); p != game->players.end(); p++) {
		CQ_setGroupCard(ac, game->GroupId, p->first, (paddeditoa(++i) + "号" + p->second.nickName).c_str());
	}
}
void AskForRoles(CGame* game) {	
	game->roleCounts = CRole::GetRecommend(game->players.size(), game->enable);
	sendQQMessage(game->GroupId, -1, ("默认角色为" + *(CRole::RecommendVectorToString(game->roleCounts)) + "\n请输入指令：\n·！启用/禁用+角色\n·！确认").c_str());
}
void DispatchRole(CGame* game) {
	int p = 0;
	vector<int> roles(game->players.size());
	for (int i = 0; i < CRole::TOTAL; i++) {
		for (int j = 0; j < game->roleCounts[i]; j++) {
			roles[p++] = i;
		}

	}
	debug("sort end");
	std::random_shuffle(roles.begin(), roles.end());
	string temp;
	playerMap::iterator pl;
	p = 0;
	for (pl = game->players.begin(); pl != game->players.end(); pl++) {
		debug((std::to_string(pl->first)+ " " +std::to_string(roles[p])).c_str());
		temp = temp + std::to_string(pl->first) + pl->second.nickName + " 分配为 " + CRole::names[roles[p++]] + "\n";
		sendQQMessage();
	}
	sendQQMessage(game->GroupId, -1, temp.c_str());
	stopGame(game);
}
void stopGame(CGame* game) {
	int groupId = game->GroupId;
	gameMap::iterator p;
	for (p = gameList.begin(); p != gameList.end(); p++) {
		if (p->first == groupId) {
			gameList.erase(p);
		}
	}	
	sendQQMessage(groupId, -1, "游戏已结束");
}