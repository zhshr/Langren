#include "stdafx.h"
#include "util.h"
#include <iostream>
//#define TEST

using namespace std;
void debug(const char* msg) {
#ifdef TEST
	cout << msg << "\n";
#else
	CQ_addLog(ac, CQLOG_DEBUG, CQAPPNAME, msg);
#endif	
}
void sendQQMessage(int64_t fromGroup, int64_t fromQQ, const char*msg) {
	if (fromGroup == -1) {
#ifdef TEST
#else
		CQ_sendPrivateMsg(ac, fromQQ, msg);
#endif
		
	}
	else {
#ifdef TEST
#else
		CQ_sendGroupMsg(ac, fromGroup, msg);
#endif
		
	}
}
bool ListPlugins(int64_t fromGroup, int64_t fromQQ, const char*msg) {
	string str = string(msg);
#ifdef TEST
#else
	CQ_addLog(ac, CQLOG_DEBUG, "��ʾ�б�", CQAPPNAME);
#endif
	
	if (str == "��ʾ����б�") {
		sendQQMessage(fromGroup, fromQQ, CQAPPNAME);
		return true;
	}
	else {
		return false;
	}
}