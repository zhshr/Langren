// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LANGREN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LANGREN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once
#ifdef LANGREN_EXPORTS
#define LANGREN_API __declspec(dllexport)
#else
#define LANGREN_API __declspec(dllimport)
#endif

#include "stdafx.h"
#include "Game.h"
#include <unordered_map>

using namespace std;

int ProcessMessage(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font);
void StartGame(int64_t groupId, int64_t qqId);
void addPlayer(CGame* game, int count);
void ModifyCards(CGame* game);
void AskForRoles(CGame* game);
void DispatchRole(CGame* game);
void stopGame(CGame* game);
