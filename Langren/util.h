#pragma once

#include <string>
#include "stdafx.h"
#include "cqp.h"


bool ListPlugins(int64_t fromGroup, int64_t fromQQ, const char*msg);
void sendMessage(int64_t fromGroup, int64_t fromQQ, const char*msg);