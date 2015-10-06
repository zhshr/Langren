// Langren.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Langren.h"
#include "cqp.h"
#include "util.h"

int ProcessPrivate(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {
	if (ListPlugins(-1, fromQQ, msg)) {
		return EVENT_IGNORE;
	}

	return EVENT_IGNORE;
}
int ProcessGroup(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	if (ListPlugins(fromGroup, fromQQ, msg)) {
		return EVENT_IGNORE;
	}
	return EVENT_IGNORE;
}
