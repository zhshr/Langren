#pragma once
#include "util.h"
#include <vector>
using namespace std;
class CRole
{
public:
	static const int VILLAGER = 0;
	static const int WOLF = 1;
	static const int SORCERER = 2;
	static const int CUPID = 3;
	static const int PROPHET = 4;
	static const int THIEF = 5;
	static const int GUARD = 6;
	static const int IDIOT = 7;
	static const int HUNTER = 8;
	static const int TOTAL = 9;
	static const char *names[];

	static vector<int> GetRecommend(int total, bool enable[]);
	static string *RecommendVectorToString(vector<int> vec);
	CRole();
	~CRole();
};

