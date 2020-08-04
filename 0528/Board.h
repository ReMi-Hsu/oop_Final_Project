#pragma once
#include "Readfile.h"
#include <fcntl.h>
#include <io.h>
#include<windows.h>
void iniGamerBoard(void);
void findRoom(int level, std::vector<Position>& door);
void reFindRoom(bool** isValid, Position start, std::vector<Position>& door);
void outputGamerBoard(void);
void delGamerBoard(void);
bool completeBoard(void);