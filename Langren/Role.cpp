#include "stdafx.h"
#include "Role.h"

const char *CRole::names[] = { "村民","狼人","女巫","丘比特","预言家","盗贼","守卫","白痴","猎人" };
vector<int> CRole::GetRecommend(int total, bool enable[])
{
	vector<int> roles(TOTAL);
	int otherCount = 0;
	for (int i = 2; i < TOTAL; i++) {
		roles[i] = enable[i];
		otherCount += enable[i];
	}
	if (enable[THIEF]) {
		total = total + 2;
	}
	switch (total) {
	case 6:
	case 7:
	case 8:
		roles[VILLAGER] = 2;
		break;
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		roles[VILLAGER] = 3;
		break;
	
	case 14:
		roles[VILLAGER] = 4;
		break;
	case 15:
	case 16:
	case 17:
		roles[VILLAGER] = 5;
		break;
	case 18:
	case 19:
	case 20:
		roles[VILLAGER] = 6;
		break;

	}	
	roles[WOLF] = total - roles[VILLAGER] - otherCount;
	return roles;
}

string *CRole::RecommendVectorToString(vector<int> vec) {
	
	string *result = new string();
	for (int i = 0; i < TOTAL; i++) {
		if (vec[i] == 0) {
			*result += string("0") + names[i] + " ";
		}
		else {
			*result += std::to_string(vec[i]) + names[i] + " ";
		}
		
	}
	return result;
}
CRole::CRole()
{
}


CRole::~CRole()
{
}
