#include "Character.h"

char Character::Halpha = 'A';
int Character::aliveCount = 0;
Character::Character(void) {//OK
	name = Halpha;
	Halpha++;
	holdCard.clear();
	this->alive = true;
	aliveCount++;
}
void Character::initial(const CharacterData& chaDataTemp) {//OK
	Creature::initialHp(chaDataTemp.hp);
	Creature::shieldToZero();
	Creature::dexterityToZero();
	holdCard.clear();

	for (int i = 0; i < chaDataTemp.totalcard; i++) {
		holdCard.push_back(chaDataTemp.card[i]);
	}
	for (int i = 0; i < chaDataTemp.cardNum; i++) {
		int cardNumero;
		std::cin >> cardNumero;
		holdCard[cardNumero].cardStatus = 0;//在手上
	}
	//check
	/*std::cout << name << ' ' << Creature::getHp() << std::endl;
	for (int i = 0; i < chaDataTemp.totalcard; i++) {
		std::cout << holdCard[i].numero << ' ' <<
			holdCard[i].dexterity << ' ' <<
			holdCard[i].topHalf << ' ' <<
			holdCard[i].lowHalf << ' ' <<
			holdCard[i].cardStatus << endl;
	}*/
	//check end
}

const char& Character::getName(void) {//OK
	return this->name;
}
void Character::visible(void) const{//OK
	Position temp = Creature::getPosition();
	if ((temp.row > 0) && (temp.col > 0)) {
		if (((board[temp.row][temp.col] == '1')||(board[temp.row][temp.col] == name)) && (this->alive)) {
			board[temp.row][temp.col] = name;
		}
		else {
			if (!(this->alive)) {
				board[temp.row][temp.col] = map.initBoard[temp.row][temp.col];
			}
			temp.row = temp.col = -1;
		}
	}
	//outputGamerBoard();
}
void Character::initialHalpha(void) {
	Halpha = 'A';
}
void Character::getHoldCard(void) {//OK
	std::vector <int> hand;
	std::vector <int> discard;
	hand.clear();
	discard.clear();
	for (int i = 0; i < holdCard.size(); i++) {
		if (holdCard[i].cardStatus == 0) {//in hand
			hand.push_back(holdCard[i].numero);
		}
		else if (holdCard[i].cardStatus == 1) {//dicard zone
			discard.push_back(holdCard[i].numero);
		}
	}
	std::cout << "hand: ";
	if (hand.size() > 0) {
		for (int i = 0; i < hand.size() - 1; i++) {
			std::cout << hand[i] << ", ";
		}
		std::cout << hand[hand.size() - 1];
	}
	else {
		std::cout << "none";
	}
	std::cout << "; discard: ";
	if (discard.size() > 0) {
		for (int i = 0; i < discard.size() - 1; i++) {
			std::cout << discard[i] << ", ";
		}
		std::cout << discard[discard.size() - 1];
	}
	else {
		std::cout << "none";
	}
	std::cout << std::endl;
	hand.clear();
	discard.clear();
}
void Character::getChooseCard(void){//OK
	if (alive) {
		if (chooseCard[0] != -1) {
			std::cout << name << " " << holdCard[chooseCard[0]].dexterity
				<< " " << chooseCard[0] << " " << chooseCard[1];
		}
		else {
			std::cout << name << " " << chooseCard[0];
		}
		std::cout << std::endl;
	}
}
bool Character::getWhoseTurn(void) {
	setColor(14);
	std::cout << name << "'s turn: ";
	if (chooseCard[0] != -1) {
		std::cout << "card " << chooseCard[0] << ' ' << chooseCard[1] << std::endl;
		setColor(7);
		return false;
	}
	else {
		std::cout << "99 -1" << std::endl;
		setColor(7);
		return true;
	}

}
void Character::enoughCard(const bool& needOutput) {
	int cardRemain = 0;
	int cardDiscard = 0;
	for (int i = 0; i < holdCard.size(); i++) {
		if (holdCard[i].cardStatus == 0) {
			cardRemain++;
		}
		else if (holdCard[i].cardStatus == 1) {
			cardDiscard++;
		}
	}
	if ((cardRemain < 2)&&(cardDiscard < 2)) {
		Creature::setHp(0);
		die(true, needOutput);
	}
}
void Character::draw(int first, int last) {//抽牌//OK
	int cardRemain = 0;
	for (int i = 0; i < holdCard.size(); i++) {
		if (holdCard[i].cardStatus == 0) {
			cardRemain++;
		}
	}
	if (cardRemain < 2) {
		std::cout << "card are not enough, need to rest!" << std::endl;
	}
	else {
		nowTurn = false;
		int countTemp = 0;
		bool flag;
		do {
			flag = true;
			if (countTemp > 0) {
				std::cout << "card discard, please enter other numero:";
				std::cin >> first >> last;
			}
			else {
				countTemp = 1;
			}
			if ((first < holdCard.size()) && (last < holdCard.size())) {
				if ((holdCard[first].cardStatus != 0)
					|| (holdCard[last].cardStatus != 0)
					|| (first == last)) {
					flag = false;
				}
			}
			else {
				flag = false;
			}
		} while (flag == false);
		while ((holdCard[first].cardStatus != 0) || (holdCard[last].cardStatus != 0)
			|| (first == last)) {
			std::cout << "card discard, please enter other numero:";
			std::cin >> first >> last;
			std::cin.ignore();
		}
		chooseCard[0] = first;
		chooseCard[1] = last;
		dexterity[0] = holdCard[first].dexterity;
		dexterity[1] = holdCard[last].dexterity;
	}
}
void Character::turn(std::string tempStr, Monster *enemy) {//出牌
	if (this->alive) {
		if (chooseCard[0] != -1) {
			int partInt, otherInt;
			char partChar;
			std::stringstream chooseCardStr[2];
			std::stringstream tempSStr(tempStr);
			tempSStr >> partInt >> partChar;
			if (partInt == chooseCard[0]) {
				otherInt = chooseCard[1];
			}
			else {
				otherInt = chooseCard[0];
			}
			if (partChar == 'd') {
				chooseCardStr[0] << holdCard[partInt].lowHalf;
				chooseCardStr[1] << holdCard[otherInt].topHalf;
			}
			else {
				chooseCardStr[0] << holdCard[partInt].topHalf;
				chooseCardStr[1] << holdCard[otherInt].lowHalf;
			}
			holdCard[partInt].cardStatus = 1;
			holdCard[otherInt].cardStatus = 1;
			std::vector <std::string> command;
			std::vector <int> point;
			std::string cTemp;
			int pTemp;
			for (int i = 0; i < 2; i++) {
				while (chooseCardStr[i] >> cTemp >> pTemp) {
					command.push_back(cTemp);
					point.push_back(pTemp);
				}
			}
			setColor(11);
			cout << endl << name << "'s Action:" << endl;
			setColor();
			for (int i = 0; i < command.size(); i++) {
				setColor(11);
				std::cout << command[i] << ' ' << point[i];
				if (i + 1 < command.size()) {
					if (command[i + 1] == "range") {
						std::cout << ", " << command[i + 1] << ' ' << point[i + 1];
						i++;
					}
				}
				std::cout << std::endl;
				setColor();
			}
			cout << "------------------------------------------" << endl;
			chooseCardStr[0].clear();
			chooseCardStr[1].clear();
			int count = 0;
			while (count < command.size()) {
				if (command[count] == "attack") {
					if (count + 1 < command.size()) {
						if (command[count + 1] == "range") {
							if(attack(point[count], point[count + 1], enemy)) {
								outputGamerBoard();
							}
							count++;
						}
						else {
							if (attack(point[count], 1, enemy)) {
								outputGamerBoard();
							}
						}
					}
					else {
						if (attack(point[count], 1, enemy)) {
							outputGamerBoard();
						}
					}
				}
				else if (command[count] == "shield") {
					std::cout << name;
					Creature::shieldPlus(point[count]);
				}
				else if (command[count] == "heal") {
					std::cout << name << " ";
					Creature::heal(point[count]);
					std::cout << ", now hp is " << Creature::getHp() << std::endl;
				}
				else if (command[count] == "move") {
					move(point[count]);
					outputGamerBoard();
				}
				count++;
			}
			command.clear();
			point.clear();
			cout << "------------------------------------------" << endl << endl;
		}
		else {
			cout << "------------------------------------------" << endl;
			rest();
			cout << "------------------------------------------" << endl << endl;
		}
	}
}

bool Character::attack(int atkTemp, int rangeTemp, Monster *enemy) {
	// std::cout << "attack, range: " << atkTemp << ' ' << rangeTemp << std::endl;
	bool attackEnemy = false;
	char target;
	bool isFind = true; 
	do {
		cin >> target;
		if (target != '0') {
			if (isupper(target) == true) { // can't attack its friend
				cout << "error target!!!" << endl;
				continue;
			}
			for (int i = 0; i < monsterNum; i++) {
				if (target == enemy[i].name) {
					isFind = true;
					if (inRange(Creature::getPosition(), enemy[i].getPosition(), rangeTemp) && canSee(Creature::getPosition(), enemy[i].getPosition())) {//紅字
						int atkReal = atkTemp - enemy[i].getShield();
						if (atkReal <= 0) { 
							atkReal = 0;
						}
						std::cout << name << " attack " << enemy[i].name << " " << atkTemp << " damage, ";
						enemy[i].setHp(enemy[i].getHp() - atkReal);
						enemy[i].check(true);
						enemy[i].visible(true);
						if (enemy[i].getHp() <= 0) {
							attackEnemy = true;
						}
					}
					else {
						isFind = false; // can't see or reach the target
						break;
					}
				}
			}
			if (isFind == false) cout << "error target!!!" << endl; // invalid monster name
		}
		else {
			isFind = true;
		}
	} while(isFind == false);
	return attackEnemy;
}
void Character::spawn(void) {//OK
	std::cout << "please enter steps: ";
	string steps; cin >> steps;
	Position *movement = new Position[steps.length()]; // space for movements according to keys
	for (int i = 0; i < steps.length(); i++) {
		switch (steps[i]) { // translates keys to movements
		case 'W': case 'w':
			movement[i].row = -1;
			movement[i].col = 0;
			break;
		case 'S': case 's':
			movement[i].row = 1;
			movement[i].col = 0;
			break;
		case 'A': case 'a':
			movement[i].row = 0;
			movement[i].col = -1;
			break;
		case 'D': case 'd':
			movement[i].row = 0;
			movement[i].col = 1;
			break;
		case 'E': case 'e':
			movement[i].row = 0;
			movement[i].col = 0;
			break;
		}
	}
	setPostion(map.spawn[0]);
	for (int i = 0; i < steps.length(); i++) {
		Position temp = getPosition();
		temp.row = temp.row + movement[i].row;
		temp.col = temp.col + movement[i].col;
		if ((board[temp.row][temp.col] == '4') || (board[temp.row][temp.col] == '5')) {
			setPostion(temp);
		}
		else continue;
	}
	Position now = getPosition();
	if (board[now.row][now.col] == '4') { //not spawn point
		board[now.row][now.col] = getName();
		for (int i = 1; i < map.spawn.size();i++) {
			if ((map.spawn[i].row == now.row) && (map.spawn[i].col == now.col)) {
				map.spawn.erase(map.spawn.begin() + i);
			}
		}
	}
	else { //spwan point // move '*' to the next position
		board[now.row][now.col] = getName();
		map.spawn.erase(map.spawn.begin());
		if (map.spawn.size() != 0) {
			board[map.spawn[0].row][map.spawn[0].col] = '5';
		}
	}
}
void Character::move(int step) {//OK
	std::cout << "move: " << step << std::endl;
	std::string commandTemp;
	bool isValid = true;
	std::cout << "please enter steps: ";
	do {
		isValid = true;
		std::cin >> commandTemp;
		if (commandTemp.length() <= step) {
			std::vector <Position> movement;
			movement.push_back(Creature::getPosition());
			//Position* movement = new Position[commandTemp.length()]; // allocate space for movements depend on keys
			for (int i = 0; i < commandTemp.length(); i++) {
				Position temp = movement[movement.size() - 1];
				if ((board[temp.row][temp.col] == '0') || (board[temp.row][temp.col] == '2') 
					|| (islower(board[temp.row][temp.col]))) {
					isValid = false;
					break;
				}
				else {
					switch (commandTemp[i]) { // translates keys to movements
					case 'W': case 'w':
						temp.row -= 1;
						break;
					case 'S': case 's':
						temp.row += 1;
						break;
					case 'A': case 'a':
						temp.col -= 1;
						break;
					case 'D': case 'd':
						temp.col += 1;
						break;
					case 'E': case 'e':
						break;
					default:
						isValid = false;
						break;
					}
					if (isValid) {
						movement.push_back(temp);
					}
				}
			}
			/*for (int i = 0; i < movement.size(); i++) {
				cout << movement[i].row << ' ' << movement[i].col << endl;
			}*/
			Position temp = movement[movement.size() - 1];
			if ((board[temp.row][temp.col] == '2') || (islower(board[temp.row][temp.col]))) {
				isValid = false;
				movement.pop_back();
			}
			temp = movement[movement.size() - 1];
			while ((board[temp.row][temp.col] != name) && (board[temp.row][temp.col] != '1') && (board[temp.row][temp.col] != '3') && (movement.size() > 1)) {
				isValid = false;
				movement.pop_back();
				temp = movement[movement.size() - 1];
			}
			temp = movement[movement.size() - 1];
			//check
			/*for (int i = 0; i < movement.size(); i++) {
				std::cout << movement[i].row << ' ' << movement[i].col << " - ";
			}*/
			//check end
			if (isValid) {
				Position restore = getPosition(); // restore 1 to original position
				board[restore.row][restore.col] = map.initBoard[restore.row][restore.col];
				setPostion(temp); // update character position
				board[temp.row][temp.col] = getName(); // update board with character's new position
			}
			else {
				cout << "error move!!! Please input again: ";
				isValid = false;
			}
			movement.clear();
		}
		else {
			cout << "error move!!! Please input again: ";
			isValid = false;
		}
	} while (isValid == false);
}
bool Character::canShuffle(void) {
	std::cout << "Canshuffle" << std::endl;
	int flagCanShuffle = 0;
	for (int i = 0; i < holdCard.size(); i++) {
		if (holdCard[i].cardStatus == 1) {//dicard zone is not empty
			flagCanShuffle++;
		}
	}
	if (flagCanShuffle >= 2) {//dicard zone is not empty
		this->dexterity[0] = this->dexterity[1] = 99;
		chooseCard[0] = chooseCard[1] = -1;
		nowTurn = false;
		return true;
	}
	else {//dicard zone is empty
		std::cout << "no card in dicard zone, can't shuffle" << std::endl;
		return false;
	}
}
void Character::shuffle() {//洗牌//OK
	std::cout << "shuffle" << std::endl;
	getHoldCard();
	int flagCanShuffle = 0;
	for (int i = 0; i < holdCard.size(); i++) {
		if (holdCard[i].cardStatus == 1) {//dicard zone is not empty
			flagCanShuffle++;
		}
	}
	if (flagCanShuffle >= 2) {//dicard zone is not empty
		nowTurn = false;
		std::cout << "remove card: ";
		int choose;
		std::string inputStr;
		int countTemp = 0;
		bool flag;
		std::cin >> inputStr;
		do{
			do {
				flag = true;
				if (countTemp > 0) {
					std::cout << "card isn't in discard zone, please enter other numero:";
					std::cin >> inputStr;
				}
				else {
					countTemp = 1;
				}
				for (int i = 0; i < inputStr.size(); i++) {
					if (!isdigit(inputStr[i])) {
						flag = false;
					}
				}
				if (flag) {
					choose = std::stoi(inputStr);
				}
			} while (flag == false);
			if (choose < holdCard.size()) {
				for (int i = 0; i < holdCard.size(); i++) {
					if (choose == holdCard[i].numero) {
						choose = i;
						break;
					}
				}
				if (holdCard[choose].cardStatus != 1) {
					flag = false;
				}
			}
			else {
				flag = false;
			}
		} while (flag == false);

		holdCard[choose].cardStatus = 2;//put one card in destroy
		for (int i = 0; i < holdCard.size(); i++) {
			if (holdCard[i].cardStatus == 1) {//other cards return in hand
				holdCard[i].cardStatus = 0;
			}
		}
		this->dexterity[0] = this->dexterity[1] = 99;
		getHoldCard();
	}
	else {//dicard zone is empty
		std::cout << "no card in dicard zone, can't shuffle" << std::endl;
	}
}
void Character::rest() {//OK
	std::cout << name << " ";
	Creature::heal(2);
	std::cout << ", now hp is " << Creature::getHp() << std::endl;
	shuffle();
}

void Character::check(const bool& beAtked) {
	// A-hp: 12, shield: 1
	if (alive) {
		if (!beAtked) {
			cout << name << "-hp: " << Creature::getHp() << ", shield: " << Creature::getShield() << endl;
		}
		else {
			cout << name << " shield " << Creature::getShield()
				<< ", " << name << " remain " << Creature::getHp()
				<< " hp" << std::endl;
		}
		die(false, true);
	}
}
void Character::die(const bool& lackCard, const bool& needOutput) {
	if (Creature::getHp() <= 0) {
		std::cout << name << " is killed!!" << std::endl;
		Creature::die();
		aliveCount--;
		holdCard.clear();
		//if (lackCard) {
			visible();
		//}
		if (needOutput) {
			outputGamerBoard();
		}
		
	}
}
void Character::nextRound(void) {
	if (this->alive) {
		nowTurn = true;
		Creature::shieldToZero();
	}
}
Character::~Character(void) {
	Creature::die();
	holdCard.clear();
	aliveCount = 0;
}
