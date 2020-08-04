#include "Board.h"
char** board;

void iniGamerBoard(void){
	board = new char*[map.rowSize];// new a gameboard
	for (int i = 0; i < map.rowSize; i++) {
		board[i] = new char[map.colSize];
		for (int j = 0; j < map.colSize; j++) {// initialize gameboard with 0
			board[i][j] = '0';
		}
	}
}

void findRoom(int level, std::vector<Position>& door) {
	//initiale isValid
	bool** isValid = new bool*[map.rowSize];
	for (int i = 0; i < map.rowSize; i++) {
		isValid[i] = new bool[map.colSize];
		for (int j = 0; j < map.colSize; j++) {// initialize gameboard with 0
			isValid[i][j] = true;
		}
	}
	//initiale isValid end

	if (level == 1) { // level 1 from spawn point
		Position start;
		start = map.spawn[0];
		reFindRoom(isValid, start, door);
		board[map.spawn[0].row][map.spawn[0].col] = '5';
		for (int i = 1; i < 4; i++) {
			board[map.spawn[i].row][map.spawn[i].col] = '4';
		}
	}
	else {// find 3//3的上下左右 要是true 1
		//check door
		/*for (int i = 0; i < door.size(); i++) {
			std::cout << board[door[i].row][door[i].col] << ' ' << door[i].row << ' ' << door[i].col << ',';
		}*/
		//check door end
		for (int i = 0; i < door.size(); i++) {
			if (isupper(board[door[i].row][door[i].col])!=0) {//character on door
				//open this door
				Position around[4];
				around[0].row = door[i].row - 1; around[0].col = door[i].col;//up
				around[1].row = door[i].row + 1; around[1].col = door[i].col;//down
				around[2].row = door[i].row; around[2].col = door[i].col - 1;//left
				around[3].row = door[i].row; around[3].col = door[i].col + 1;//right
				for (int j = 0; j < 4; j++) {
					if ((board[around[j].row][around[j].col] == '0') && (map.initBoard[around[j].row][around[j].col] == '1')) {
						map.initBoard[door[i].row][door[i].col] = board[door[i].row][door[i].col] = '1';
						door[i].row = door[i].col = -1;
						reFindRoom(isValid, around[i], door);
					}
				}
			}
		}
		int i = 0;
		while (i < door.size()) {
			if ((door[i].col == -1) && (door[i].row==-1)) {
				door.erase(door.begin() + i);
			}
			else {
				i++;
			}
		}
	}
	//check door
		/*for (int i = 0; i < door.size(); i++) {
			std::cout << door[i].row << ' ' << door[i].col << ',';
		}*/
	//check door end

	//check board
	/*for (int i = 0; i < map.rowSize; i++) {
		for (int j = 0; j < map.colSize; j++) {
			cout << board[i][j];
		}
		cout << endl;
	}
	for (int i = 0; i < door.size(); i++) {
		std::cout << door[i].row << ' ' << door[i].col << ',';
	}
	cout << endl;*/
	//check board end

	//delete isValid
	for (int i = 0; i < map.rowSize; i++) {
		delete[] isValid[i];
	}
	delete[] isValid;
	//delete isValid end
}

void reFindRoom(bool** isValid, Position start, std::vector<Position>& door) {
	int around[4][3];
	around[0][0] = (int)map.initBoard[start.row - 1][start.col]; // up (value)
	around[0][1] = start.row - 1; around[0][2] = start.col; // up (index)

	around[1][0] = (int)map.initBoard[start.row + 1][start.col]; // down
	around[1][1] = start.row + 1; around[1][2] = start.col;

	around[2][0] = (int)map.initBoard[start.row][start.col - 1]; // left
	around[2][1] = start.row; around[2][2] = start.col - 1;

	around[3][0] = (int)map.initBoard[start.row][start.col + 1]; // right
	around[3][1] = start.row; around[3][2] = start.col + 1;

	//check
	/*for (int i = 0; i < 4; i++) {
		std::cout << around[i][1] << ' ' << around[i][2] << ":" << around[i][0] << std::endl;
	}*/
	//check end

	for (int i = 0; i < 4; i++) {
		if (isValid[around[i][1]][around[i][2]] == false) continue;
		// if the element is 1 (space), store 1 in board and call recursion
		else if (around[i][0] == (int)'1') {
			board[around[i][1]][around[i][2]] = '1';
			isValid[around[i][1]][around[i][2]] = false;
			Position next;
			next.row = around[i][1];
			next.col = around[i][2];
			reFindRoom(isValid, next, door); // recurison
		}
		// if the element is 2 (obstacle), store 2 in board and call recursion
		else if (around[i][0] == (int)'2') {
			board[around[i][1]][around[i][2]] = '2';
			isValid[around[i][1]][around[i][2]] = false;
			Position next;
			next.row = around[i][1];
			next.col = around[i][2];
			reFindRoom(isValid, next, door); // recurison
		}
		// if the element is 3 (door), store 3 in board and return
		else if (around[i][0] == (int)'3') {
			Position temp;
			temp.row = around[i][1];
			temp.col = around[i][2];
			bool duplicate = false;//改
			for (int j = 0; j < door.size(); j++) {
				if (temp == door[j]) {
					duplicate = true;
				}
			}
			if (!duplicate) {
				door.push_back(temp);
				board[around[i][1]][around[i][2]] = '3';
			}//改
			isValid[around[i][1]][around[i][2]] = false;
			// how to store the position of doors??
			continue;
		}
		// if the element is 0 (wall), return
		else{
			isValid[around[i][1]][around[i][2]] = false;
			continue;
		}
	}
	return;
}

void outputGamerBoard(void) {
	for (int j = 0; j < map.rowSize; j++) { // initialize gameboard with 0
		for (int k = 0; k < map.colSize; k++) {
			if (board[j][k] == '1') {
				cout << " ";
			}
			else if (board[j][k] == '2') {
				setColor(14);
				cout << '#';
				setColor();
			}
			else if (board[j][k] == '3') {
				setColor(10);
				_setmode(_fileno(stdout), _O_U16TEXT);
				std::wcout << L"\u2338";//door
				_setmode(_fileno(stdout), _O_TEXT);
				setColor();
			}
			else if (board[j][k] == '5') {//spwan point
				setColor();
				cout << "*";
				setColor();
			}
			else if (board[j][k] == '4') {//spwan place
				cout << "-";
			}
			else if (board[j][k] == '0') {//wall
				setColor(119);
				std::cout << " ";
				//cout << " ";
				setColor();
			}
			else if(islower(board[j][k])){//monster
				setColor(12);
				cout << board[j][k];
				setColor();
			}
			else{//chatacter
				setColor(11);
				cout << board[j][k];
				setColor();
			}
		}
		cout << endl;
	}
}

void delGamerBoard(void) {
	for (int i = 0; i < map.rowSize; i++) {
		delete[] board[i];
		delete[] map.initBoard[i];
	}
	delete[] board;
	map.spawn.clear();
	map.deploy.clear();
}
bool completeBoard(void) {
	bool complete = true;
	for (int i = 0; i < map.rowSize; i++) {
		for (int j = 0; j < map.colSize; j++) {
			if (map.initBoard[i][j] == '1') {
				if ((board[i][j] == '1') || (isupper(board[i][j]))) {
				}
				else {
					complete = false;
				}
			}
		}
	}
	return complete;
}