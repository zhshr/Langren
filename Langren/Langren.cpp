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
// ��Ϣ���
string prefix = "----------ӡ���������ģ��----------\n";

int ProcessMessage(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font)
{
	if (ListPlugins(fromGroup, fromQQ, msg)) {
		return EVENT_IGNORE;
	}
	srand((unsigned int)time(NULL));
	string str = string(msg);
	/*if (str == "���Բ���" && fromGroup != -1) {
		CQ_setGroupBan(ac, fromGroup, fromQQ, 180);
		Sleep(10000);
		CQ_setGroupBan(ac, fromGroup, fromQQ, 0);
		return EVENT_BLOCK;
	}
	*/
	if (fromGroup == -1) {
		//˽����Ϣ

	}
	else {
		//����Ⱥ�Ƿ��н����е���Ϸ
		gameMap::iterator p = gameList.find(fromGroup);
		if (p != gameList.end()) {
			CGame *game = &(p->second);
			switch (game->state) {

			case (CGame::STATE_ENROLL):
				if (str.substr(0, 6) == "������" && fromQQ == game->AdminId) {
					int i = std::atoi(str.substr(6, str.length()).c_str());
					addPlayer(game, i);
					return EVENT_BLOCK;
				}
				if (game->AddPlayer(fromQQ, str)) {
					sendQQMessage(fromGroup, fromQQ, (std::to_string(fromQQ) + "�����ɹ����ǳ�Ϊ" + str).c_str());
				}
				CQ_addLog(ac, CQLOG_INFO, "����", str.c_str());
				return EVENT_BLOCK;

			case CGame::STATE_ASK_ROLE:
				if (str.substr(0, 6) == "������" && fromQQ == game->AdminId) {
					string role = str.substr(6, str.length());
					if (game->ToggleRoleByStr(role, true)) {
						sendQQMessage(fromGroup, fromQQ, (role + "������").c_str());
						AskForRoles(game);
						return EVENT_BLOCK;
					}
				}
				if (str.substr(0, 6) == "������" && fromQQ == game->AdminId) {
					string role = str.substr(6, str.length());
					if (game->ToggleRoleByStr(role, false)) {
						sendQQMessage(fromGroup, fromQQ, (role + "�ѽ���").c_str());
						AskForRoles(game);
						return EVENT_BLOCK;
					}
				}
				if (str == "��ȷ��" && fromQQ == game->AdminId) {
					DispatchRole(game);
					return EVENT_BLOCK;
				}
			}
		}
		else {
			if (str == "����ʼ") {
				//��ʼ����Ϸ
				debug("���������߳�");
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
	debug("�Ѿ������߳�");

	//������Ϸʵ��
	CQ_addLog(ac, CQLOG_INFO, "��Ϸ��ʼ", ("Ⱥ" + std::to_string(groupId) + "��ʼ������Ϸ").c_str());
	CGame *game = new CGame(groupId, qqId);
	gameList.insert(pair<int64_t, CGame>(groupId, *game));
	string temp = prefix + "��⵽��ʼ������뱨��ģʽ\n������������1����֮�ڻظ��ǳƲ��뱨��\n" + std::to_string(game->AdminId) + "�ѱ���Ϊ������Ϸ������";
	sendQQMessage(groupId, -1, temp.c_str());
	delete game;
	//�ȴ���ұ���
	Sleep(10000);
	gameMap::iterator pl = gameList.find(groupId);
	if (pl != gameList.end()) {
		CGame *game = &(pl->second);
		game->state = game->STATE_DISPATCH_ROLE;
		CQ_addLog(ac, CQLOG_DEBUG, "��������", "");
		sendQQMessage(groupId, -1, (prefix + "��������������������ұ���\n" + game->PlayersString()).c_str());
		if (game->players.size() < 6) {
			sendQQMessage(groupId, -1, "������������6�ˣ���Ϸ�޷���ʼ���˳���");
			stopGame(game);
		}
		else {
			
			//�޸�Ⱥ��Ƭ
			//ModifyCards(game);
			//ѯ���������
			game->AutoAdjustRole();
			game->state = game->STATE_ASK_ROLE;
			AskForRoles(game);
			//�����ɷ���ݽ׶�
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
	debug("׼���޸�Ⱥ��Ƭ");
	playerMap::iterator p;
	int i = 0;
	for (p = game->players.begin(); p != game->players.end(); p++) {
		CQ_setGroupCard(ac, game->GroupId, p->first, (paddeditoa(++i) + "��" + p->second.nickName).c_str());
	}
}
void AskForRoles(CGame* game) {	
	game->roleCounts = CRole::GetRecommend(game->players.size(), game->enable);
	sendQQMessage(game->GroupId, -1, ("Ĭ�Ͻ�ɫΪ" + *(CRole::RecommendVectorToString(game->roleCounts)) + "\n������ָ�\n��������/����+��ɫ\n����ȷ��").c_str());
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
		temp = temp + std::to_string(pl->first) + pl->second.nickName + " ����Ϊ " + CRole::names[roles[p++]] + "\n";
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
	sendQQMessage(groupId, -1, "��Ϸ�ѽ���");
}