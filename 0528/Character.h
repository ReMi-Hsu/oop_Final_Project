#pragma once
#//include "Header.h"
#include "Board.h"
#include "Creature.h"
#include <iostream>
#include <sstream>
#include "Monster.h"
#include "Readfile.h"

class Monster;

class Character :public Creature {
private:
	char name;
	std::vector <CharacterCardData> holdCard;
	int chooseCard[2];
	static char Halpha;
public:
	friend class Monster;
	static int aliveCount;
	Character(void);//OK
	void initial(const CharacterData& chaDataTemp);//OK
	
	const char& getName(void);//OK
	void visible(void) const;//OK

	static void initialHalpha(void);
	void getHoldCard(void);//OK
	void getChooseCard(void);//OK
	bool getWhoseTurn(void);

	void enoughCard(const bool& needOutput);
	void draw(int first, int last);//抽牌//OK
	void turn(std::string tempStr, Monster *enemy);//出牌//OK
	bool attack(int atkTemp, int rangeTemp, Monster *enemy);
	void spawn(void);//OK
	void move(int step);//OK
	bool canShuffle(void);
	void shuffle(void);//洗牌//OK``
	void rest(void);//OK
	void check(const bool& beAtked); // check hp and shield;
	void die(const bool& lackCard, const bool& needOutput);
	void nextRound(void);
	~Character(void);
};