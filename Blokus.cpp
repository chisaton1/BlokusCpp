#include<iostream>
#include<map>
using namespace std;

#define ROW 14
#define COL 14

class Pieces;
class Player;

int board[ROW][COL];
//map<char, Pieces> piecesMap;
map<char, int> charToInt;
map<char, Pieces> makePieces();
//Player player1;
//Player player2;

/* Print the board on the display. */
void showBoard() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			/*if (board[j][i] == 0) {
				cout << ". ";
			}
			else if (board[j][i] == 1) {
				cout << "  ";
			}
			else {//board == 2;
				cout << "¡ ";
			}*/
			cout << board[i][j];
			cout << " ";
		}
		cout << endl;
	}
}

class Coordinate {
public:
	int x;
	int y;

	Coordinate() {}
	Coordinate(int coordX, int coordY) {
		x = coordX;
		y = coordY;
	}
};

class Pieces {
public:
	int units;
	Coordinate c[5];

	Pieces(int unitSize, Coordinate c1, Coordinate c2,
		Coordinate c3, Coordinate c4, Coordinate c5) {
		units = unitSize;
		c[0] = c1;
		c[1] = c2;
		c[2] = c3;
		c[3] = c4;
		c[4] = c5;
	}
};

class Player {
public:
	int id;
	bool firstPut;
	map<char, Pieces> piecesMap = makePieces();
	
	Player() {}
	Player(int idNumber) {
		id = idNumber;
		firstPut = true;
	}
};

bool canPut(int *tmpRow, int *tmpCol, int currentPlayerId) { // TODO check to work correctly
	bool available = false;
	for (int i = 0; i < 5; i++) {
		if (board[tmpRow[i]][tmpCol[i]] != 0) return false; // the space is used already
		if (tmpRow[i] == -10 || tmpCol[i] == -10) return available; // finish to check all coodinates
		if (tmpRow[i] < 0 || tmpRow[i] > 13 || tmpCol[i] < 0 || tmpCol[i] > 13) return false; // if the coodinate isn't in the board
		for (int a = -1; a <= 1; a++) { // a can be -1, 0, and 1
			for (int b = -1; b <= 1; b++) { // b can be -1, 0, and 1
				if (a == 0 && b == 0) continue; // don't need to check the coodinate itself
				int row = tmpRow[i] + a;
				int col = tmpCol[i] + b;
				if (row < 0 || row > 13 || col < 0 || col > 13) continue; // becasue of no space, don't need to check
				if (a == 0 || b == 0) { // check the space which is N, S, E, or W of the coodinate
					if (board[row][col] == currentPlayerId) {
						return false;
					}
				} else { // check the corners of the coodinate
					if (board[row][col] == currentPlayerId) {
						available = true;
					}
				}
			}
		}
	}
	return available;
}

bool canFirstPut(int *tmpRow, int *tmpCol, int id) {
	int row, col;
	if (id == 1) {
		col = 4; //x
		row = 9; //y
	} else { // when id = 2
		col = 9;
		row = 4;
	}
	for (int i = 0; i < 5; i++) {
		if (row == tmpRow[i] && col == tmpCol[i]) return true;
	}
	return false;
}

bool putPiece(Pieces *piece, int col, int row, int derection, Player *currentPlayer) {
	int thisRow, thisCol;
	int tmpRow[5] = { -10,-10,-10,-10,-10 };
	int tmpCol[5] = { -10,-10,-10,-10,-10 };
	for (int i = 0; i < piece->units; i++) {
		// check derection of the piece and rotate it
		if (derection == 0) {
			thisCol = piece->c[i].x + col;
			thisRow = piece->c[i].y + row;
		}
		else if (derection == 1) {
			thisCol = -piece->c[i].x + col;
			thisRow = piece->c[i].y + row;
		}
		else if (derection == 2) {
			thisCol = -(piece->c[i].y) + col;
			thisRow = piece->c[i].x + row;
		}
		else if (derection == 3) {
			thisCol = piece->c[i].y + col;
			thisRow = piece->c[i].x + row;
		}
		else if (derection == 4) {
			thisCol = -piece->c[i].x + col;
			thisRow = -piece->c[i].y + row;
		}
		else if (derection == 5) {
			thisCol = piece->c[i].x + col;
			thisRow = -piece->c[i].y + row;
		}
		else if (derection == 6) {
			thisCol = piece->c[i].y + col;
			thisRow = -piece->c[i].x + row;
		}
		else if (derection == 7) {
			thisCol = -piece->c[i].y + col;
			thisRow = -piece->c[i].x + row;
		}
		tmpRow[i] = thisRow;
		tmpCol[i] = thisCol;
	}
	int currentPlayerId = currentPlayer->id;
	if (currentPlayer->firstPut) {
		if (canFirstPut(tmpRow, tmpCol, currentPlayerId)) { // check first put
			for (int a = 0; a < piece->units; a++) { // put
				board[tmpRow[a]][tmpCol[a]] = currentPlayerId;
			}
			currentPlayer->firstPut = false;
			//currentPlayer.setFirstPutDone();
			return true;
		}
	} else { // not first put
		if (canPut(tmpRow, tmpCol, currentPlayerId)) {
			for (int a = 0; a < piece->units; a++) { // put
				board[tmpRow[a]][tmpCol[a]] = currentPlayerId;
			}
			return true;
		}
	}
		cout << "Can't put the piece in the indicated place" << endl;
		return false;
}

map<char, Pieces> makePieces() {
	Coordinate c0m2(0, -2);
	Coordinate c0m1(0, -1);
	Coordinate cm10(-1, 0);
	Coordinate cm11(-1, 1);
	Coordinate cm1m1(-1, -1);
	Coordinate c00(0, 0);
	Coordinate c01(0, 1);
	Coordinate c02(0, 2);
	Coordinate c10(1, 0);
	Coordinate c20(2, 0);
	Coordinate c11(1, 1);
	Pieces a(1, c00, c00, c00, c00, c00); // (0,0)
	Pieces b(2, c00, c01, c00, c00, c00); // (0,0),(0,1)
	Pieces c(3, c0m1, c00, c01, c00, c00); // (0,-1),(0,0),(0,1)
	Pieces d(3, c0m1, c00, c10, c00, c00); // (0,-1),(0,0),(1,0)
	Pieces e(4, c0m1, c00, c01, c02, c00); // (0,-1),(0,0),(0,1),(0,2)
	Pieces f(4, c0m1, c00, c01,cm11, c00); // (0,-1),(0,0),(0,1),(-1,1)
	Pieces g(4, c0m1, c00, c10, c01, c00); // (0,-1),(0,0),(1,0),(0,1)
	Pieces h(4, c00, c01, c10, c11, c00); // (0,0),(0,1),(1,0),(1,1)
	Pieces i(4,cm10, c00, c01, c11, c00); // (-1,0),(0,0),(0,1),(1,1)
	Pieces j(5, c0m2, c0m1, c00, c01, c02); // (0,-2),(0,-1),(0,0),(0,1),(0,2)
	Pieces k(5, c0m2, c0m1, c00, c01, cm11); // (0,-2),(0,-1),(0,0),(0,1),(-1,1)
	Pieces l(5, c0m2, c0m1, c00, cm10, cm11); // (0,-2),(0,-1),(0,0),(-1,0),(-1,1)
	Pieces m(5, c0m1, c00, c01, cm10,cm11); // (0,-1),(0,0),(0,1),(-1,0),(-1,1)
	Pieces n(5, cm1m1, c0m1,c00, c01, cm11); // (-1,-1),(0,-1),(0,0),(0,1),(-1,1)
	Pieces o(5, c0m1, c00, c10, c01, c02); // (0,-1),(0,0),(1,0),(0,1),(0,2)
	Pieces p(5, c0m1, c00, c01, c11, cm11); // (0,-1),(0,0),(0,1),(1,1),(-1,1)
	Pieces q(5, c0m2, c0m1, c00, c10, c20); // (0,-2),(0,-1),(0,0),(1,0),(2,0)
	Pieces r(5, cm1m1, c0m1, c00, c10, c11); // (-1,-1),(0,-1),(0,0),(1,0),(1,1)
	Pieces s(5, cm1m1, cm10, c00, c10, c11); // (-1,-1),(-1,0),(0,0),(1,0),(1,1)
	Pieces t(5, cm1m1, cm10, c00, c10, c01); // (-1,-1),(-1,0),(0,0),(1,0),(0,1)
	Pieces u(5, cm10, c0m1, c00, c01, c10); // (-1,0),(0,-1),(0,0),(0,1),(1,0)
	map<char, Pieces> piecesMap;
	piecesMap.insert(map<char, Pieces>::value_type('a', a));
	piecesMap.insert(map<char, Pieces>::value_type('b', b));
	piecesMap.insert(map<char, Pieces>::value_type('c', c));
	piecesMap.insert(map<char, Pieces>::value_type('d', d));
	piecesMap.insert(map<char, Pieces>::value_type('e', e));
	piecesMap.insert(map<char, Pieces>::value_type('f', f));
	piecesMap.insert(map<char, Pieces>::value_type('g', g));
	piecesMap.insert(map<char, Pieces>::value_type('h', h));
	piecesMap.insert(map<char, Pieces>::value_type('i', i));
	piecesMap.insert(map<char, Pieces>::value_type('j', j));
	piecesMap.insert(map<char, Pieces>::value_type('k', k));
	piecesMap.insert(map<char, Pieces>::value_type('l', l));
	piecesMap.insert(map<char, Pieces>::value_type('m', m));
	piecesMap.insert(map<char, Pieces>::value_type('n', n));
	piecesMap.insert(map<char, Pieces>::value_type('o', o));
	piecesMap.insert(map<char, Pieces>::value_type('p', p));
	piecesMap.insert(map<char, Pieces>::value_type('q', q));
	piecesMap.insert(map<char, Pieces>::value_type('r', r));
	piecesMap.insert(map<char, Pieces>::value_type('s', s));
	piecesMap.insert(map<char, Pieces>::value_type('t', t));
	piecesMap.insert(map<char, Pieces>::value_type('u', u));
	return piecesMap;
}

void setCharToInt() {
	charToInt.insert(map<char, int>::value_type('a', 10));
	charToInt.insert(map<char, int>::value_type('b', 11));
	charToInt.insert(map<char, int>::value_type('c', 12));
	charToInt.insert(map<char, int>::value_type('d', 13));
}

int checkInputInt(char colOrRow) {
	auto itr = charToInt.find(colOrRow);
	if (itr != charToInt.end()) { // if the input is a, b, c, or d
		return itr->second; //returns corresponding int
	} else {
		int num = (int)(colOrRow - '0'); // change char to int
		if (num < 0 || num > 9) { // if input is not a to e and not 0 to 9
			return -1;
		}
		return num; // the num is one of the 1 to 9
	}
}

int main() {
	int turn = 1;
	Player player1(1);
	Player player2(2);
	Player *currentPlayer = &player1;
	//makePieces();
	setCharToInt();
	char fourLetters[10];
	while (1) { // TODO check when the game will finish
		if (turn % 2 == 1) {
			currentPlayer = &player1;
		} else {
			currentPlayer = &player2;
		}
		cout << "Turn: " << turn << endl;
		cout << "Player" << currentPlayer->id << "'s turn" << endl;
		int row, col;
			cout << "Please input 4 cahacters" << endl;
			cin >> fourLetters;

			if (fourLetters[4] != '\0') { // If input string has length of 4
				cout << "imput four letters" << endl;
				continue;
			}
			col = checkInputInt(fourLetters[0]);
			row = checkInputInt(fourLetters[1]);
			if (col < 0) {
				cout << "input a correct col" << endl;
				continue;
			}
			if (row < 0) {
				cout << "input a correct row" << endl;
				continue;
			}

		int direction = (int)(fourLetters[3] - '0');
		if (direction < 0 || direction > 7) {
			cout << "Input a correct direction" << endl;
			continue;
		}

		bool canPutPiece = false;
		auto itr = currentPlayer->piecesMap.find(fourLetters[2]);
		if (itr != currentPlayer->piecesMap.end()) { // if the piece (character) exists
			canPutPiece = putPiece(&itr->second, col, row, direction, currentPlayer);
		} else {
			cout << "You don't have the piece" << endl;
		}
		showBoard();
		if (canPutPiece) {
			turn++; // if the player can put a piece, go to next turn
			currentPlayer->piecesMap.erase(itr); // delete the used piece
		}
	}
	return 0;
}
