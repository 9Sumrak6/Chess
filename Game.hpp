#include <iostream>
#include <vector>
#include <string>

using namespace std;

const string WHITE = "\e[38;5;1m";
const string BLACK = "\e[38;5;0m";

const string WHITE_KING = WHITE + "♚";
const string WHITE_QUEEN = WHITE + "♛";
const string WHITE_ROOK = WHITE + "♜";
const string WHITE_BISHOP = WHITE + "♝";
const string WHITE_KNIGHT = WHITE + "♞";
const string WHITE_SPAWN = WHITE + "♟";

const string BLACK_KING = BLACK + "♚";
const string BLACK_QUEEN = BLACK + "♛";
const string BLACK_ROOK = BLACK + "♜";
const string BLACK_BISHOP = BLACK + "♝";
const string BLACK_KNIGHT = BLACK + "♞";
const string BLACK_SPAWN = BLACK + "♟";

const string RESET = "\e[0m";
const string BLACK_BG1 = "\e[48;5;0m";
const string BLACK_BG = "\e[48;5;8m";
const string WHITE_BG = "\e[48;5;15m";

class Game {
	enum FigureType {
		king,
		queen,
		rook,
		bishop,
		knight,
		spawn,
		number,
		empty
	};

	struct Coord{
		int x;
		int y;
	};

	struct Move {
		Coord c1;
		bool col1;
		Coord c2;
		bool col2;
	};

	struct Figure {
		FigureType type;
		string value;
		bool color;
		bool motion;
	};

	Move move;
	vector<vector<Figure>> field;
	vector<string> movesList;
	
	bool IsCorrect(string s);

	void GetAvailableSpawnMoves(Figure figure, vector<Coord> &moves, int x, int y);
	void GetAvailableRookMoves(Figure figure, vector<Coord> &moves, int x, int y, int d, bool f);
	void GetAvailableKnightMoves(Figure figure, vector<Coord> &moves, int x, int y);
	void GetAvailableBishopMoves(Figure figure, vector<Coord> &moves, int x, int y);
	void GetAvailableQueenMoves(Figure figure, vector<Coord> &moves, int x, int y);
	void GetAvailableKingMoves(Figure figure, vector<Coord> &moves, int x, int y);
	vector<Coord> GetAvailableMoves(Figure figure, int x, int y);
	bool IsAvailableMove();
	
	void GetMove(string &s, bool &who);
	void MakeMove();
	
	void Print();
public:
	Game();
	void Play();
};

bool Game::IsCorrect(string s) {
	return s[0] <= 'H' && s[0] >= 'A' && s[1] < '9' && s[1] > '0' && s[3] <= 'H' && s[3] >= 'A' && s[4] < '9' && s[4] > '0';
}

void Game::GetAvailableSpawnMoves(Figure figure, vector<Coord> &moves, int x, int y) {
	int dy = figure.color ? 1 : -1;

	if (field[y + dy][x].type == empty)
				moves.push_back({x, y + dy});

	if (!figure.motion && (field[y + dy][x].type == empty) && (field[y + 2 * dy][x].type == empty))
				moves.push_back({x, y + 2 * dy});

	if (x < 8 && (field[y + dy][x + 1].type != empty) && !field[y + dy][x + 1].color)
				moves.push_back({x + 1, y + dy});

	if (x > 1 && (field[y + dy][x - 1].type != empty) && !field[y + dy][x - 1].color)
				moves.push_back({x - 1, y + dy});
}

void Game::GetAvailableRookMoves(Figure figure, vector<Coord> &moves, int x, int y, int d, bool f) {
	int i = 1;

	bool col = figure.color;

	bool up = true;
	bool down = true;
	bool right = true;
	bool left = true;

	while (up || down || right || left) {
		if (up && (y + i < 9) && (x + d < 9)) {
			if (field[y + i][x + d].type != empty) {
				if (field[y + i][x + d].color != col)
					moves.push_back({x + d, y + i});

				up = false;
			}
			else
				moves.push_back({x + d, y + i});
		}
		else 
			up = false;

		if (down && (y - i > 0) && (x - d > 0)) {
			if (field[y - i][x - d].type != empty) {
				if (field[y - i][x - d].color != col)
					moves.push_back({x - d, y - i});

				down = false;
			}
			else
				moves.push_back({x - d, y - i});
		}
		else 
			down = false;

		if (right && (x + i < 9) && (y - d > 0)) {
			if (field[y - d][x + i].type != empty) {
				if (field[y - d][x + i].color != col)
					moves.push_back({x + i, y - d});

				right = false;
			}
			else
				moves.push_back({x + i, y - d});
		}
		else 
			right = false;

		if (left && (x - i > 0) && (y + d < 9)) {
			if (field[y + d][x - i].type != empty) {
				if (field[y + d][x - i].color != col)
					moves.push_back({x - i, y + d});

				left = false;
			}
			else
				moves.push_back({x - i, y + d});
		}
		else 
			left = false;

		if (f)
			d++;
		i++;
	}
}

void Game::GetAvailableKnightMoves(Figure figure, vector<Coord> &moves, int x, int y) {
	for (int dy = 1, dx = 2; dy < 3; dy++, dx--) {
		if ((y + dy < 9) && (x + dx < 9) && ((field[y + dy][x + dx].color != figure.color) || (field[y + dy][x + dx].type == empty)))
			moves.push_back({x + dx, y + dy});

		if ((y + dy < 9) && (x - dx > 0) && ((field[y + dy][x - dx].color != figure.color) || (field[y + dy][x - dx].type == empty)))
			moves.push_back({x - dx, y + dy});

		if ((y - dy > 0) && (x + dx < 9) && ((field[y - dy][x + dx].color != figure.color) || (field[y - dy][x + dx].type == empty)))
			moves.push_back({x + dx, y - dy});

		if ((y - dy > 0) && (x - dx > 0) && ((field[y - dy][x - dx].color != figure.color ) || (field[y - dy][x - dx].type == empty)))
			moves.push_back({x - dx, y - dy});
	}
}

void Game::GetAvailableBishopMoves(Figure figure, vector<Coord> &moves, int x, int y) {
	GetAvailableRookMoves(figure, moves, x, y, 1, true);
}

void Game::GetAvailableQueenMoves(Figure figure, vector<Coord> &moves, int x, int y) {
	GetAvailableRookMoves(figure, moves, x, y, 0, false);
	GetAvailableBishopMoves(figure, moves, x, y);
}

void Game::GetAvailableKingMoves(Figure figure, vector<Coord> &moves, int x, int y) {
	for (int i = 0; i < 3; i++) {
		if ((x + i - 1 == 9) || (x + i - 1 == 0))
			continue;

		if ((y + 1 < 9) && ((field[y + 1][x + i - 1].color != figure.color) || (field[y + 1][x - i - 1].type == empty)))
			moves.push_back({x + i - 1, y + 1});
		if ((field[y][x + i - 1].color != figure.color) || (field[y][x + i - 1].type == empty))
			moves.push_back({x + i - 1, y});
		if ((y - 1 > 0) && ((field[y - 1][x + i - 1].color != figure.color) || (field[y - 1][x - i - 1].type == empty)))
			moves.push_back({x + i - 1, y - 1});
	}

	if (!figure.motion) {
		if (!field[y][8].motion)
			if (field[y][x + 1].type == empty && field[y][x + 2].type == empty)
				moves.push_back({x + 2, y});

		if (!field[y][1].motion)
			if (field[y][x - 1].type == empty && field[y][x - 2].type == empty && field[y][x - 3].type == empty)
				moves.push_back({x - 2, y});
	}
}

vector<Game::Coord> Game::GetAvailableMoves(Figure figure, int x, int y) {
	vector<Coord> moves;

	switch(figure.type) {
		case spawn:
			GetAvailableSpawnMoves(figure, moves, x, y);
			break;
		case rook:
			GetAvailableRookMoves(figure, moves, x, y, 0, false);
			break;
		case knight:
			GetAvailableKnightMoves(figure, moves, x, y);
			break;
		case bishop:
			GetAvailableBishopMoves(figure, moves, x, y);
			break;
		case queen:
			GetAvailableQueenMoves(figure, moves, x, y);
			break;
		case king:
			GetAvailableKingMoves(figure, moves, x, y);
			break;
	}

	return moves;
}

bool Game::IsAvailableMove() {
	vector<Coord> m = GetAvailableMoves(field[move.c1.y][move.c1.x], move.c1.x, move.c1.y);

	for (int i = 0; i < m.size(); i++)
		if (move.c2.x == m[i].x && move.c2.y == m[i].y)
			return true;

	return false;
}

void Game::GetMove(string &s, bool &who) {
	string m;

	if (who)
		m = "White move. ";
	else
		m = "Black move. ";

	cout << m << "Enter move: ";
	getline(cin, s);

	int x = s[0] - 'A' + 1;
	int y = s[1] - '0';
	int x1 = s[3] - 'A' + 1;
	int y1 = s[4] - '0';

	move = {{x, y}, field[y][x].color, {x1, y1}, field[y1][x1].color};

	while (s.length() != 5 || !IsCorrect(s) || !IsAvailableMove() || (field[y][x].color != who)) {
		cout << "Enter row again: ";
		getline(cin, s);

		x = s[0] - 'A' + 1;
		y = s[1] - '0';
		x1 = s[3] - 'A' + 1;
		y1 = s[4] - '0';

		move = {{x, y}, field[y][x].color, {x1, y1}, field[y1][x1].color};
	}

	movesList.push_back(s);

	who = !who;
}

void Game::MakeMove() {
	if (!field[move.c1.y][move.c1.x].motion)
		field[move.c1.y][move.c1.x].motion = true;

	if ((field[move.c1.y][move.c1.x].type == spawn) && (move.c2.y == 8)) {
		string s;
		string value = field[move.c1.y][move.c1.x].color ? WHITE : BLACK;

		cout << "Choose the figure that the spawn will turn into (queen, rook, knight or bishop): ";
		cin >> s;

		while (s != "rook" && s != "knight" && s != "bishop" && s != "queen") {
			cout << "Choose the figure that the spawn will turn into again: ";
			cin >> s;
		}
		
		if (s == "rook"){
			field[move.c1.y][move.c1.x].type = rook;
			field[move.c1.y][move.c1.x].value = value + "♜";
		}
		else if (s == "knight") {
			field[move.c1.y][move.c1.x].type = knight;
			field[move.c1.y][move.c1.x].value = value + "♞";
		}
		else if (s == "bishop"){
			field[move.c1.y][move.c1.x].type = bishop;
			field[move.c1.y][move.c1.x].value = value + "♝";
		}
		else if (s == "queen"){
			field[move.c1.y][move.c1.x].type = queen;
			field[move.c1.y][move.c1.x].value = value + "♛";
		}
	}

	if (field[move.c1.y][move.c1.x].type == king && abs(move.c1.x - move.c2.x) == 2) {
		if (move.c2.x > move.c1.x) {
			field[move.c1.y][6] = field[move.c1.y][8];
			field[move.c1.y][8] = {empty, " ", false, true};
		}
		else {
			field[move.c1.y][4] = field[move.c1.y][8];
			field[move.c1.y][1] = {empty, " ", false, true};
		}
	}

	field[move.c2.y][move.c2.x] = field[move.c1.y][move.c1.x];
	field[move.c1.y][move.c1.x] = {empty, " ", false, true};
}

void Game::Print(){
	for (int i = 8; i >= 0; i--) {
		cout << BLACK_BG1 << field[i][0].value << " " << RESET;
		
		for (int j = 1; j < 9; j++) {
			if (i == 0)
				cout << BLACK_BG1;
			else		
				cout << ((i + j) % 2 ? BLACK_BG : WHITE_BG);

			cout << field[i][j].value << " " << RESET;
		}

		cout << endl;
	}
}

Game::Game() {
	field = vector<vector<Figure>>(9, vector<Figure>(9));

	field[0][0] = {empty, " ", false, false};

	field[1][1] = {rook, WHITE_ROOK, true, false};
	field[1][2] = {knight, WHITE_KNIGHT, true, false};
	field[1][3] = {bishop, WHITE_BISHOP, true, false};
	field[1][4] = {queen, WHITE_QUEEN, true, false};
	field[1][5] = {king, WHITE_KING, true, false};
	field[1][6] = {bishop, WHITE_BISHOP, true, false};
	field[1][7] = {knight, WHITE_KNIGHT, true, false};
	field[1][8] = {rook, WHITE_ROOK, true, false};

	field[8][1] = {rook, BLACK_ROOK, false, false};
	field[8][2] = {knight, BLACK_KNIGHT, false, false};
	field[8][3] = {bishop, BLACK_BISHOP, false, false};
	field[8][4] = {queen,BLACK_QUEEN, false, false};
	field[8][5] = {king, BLACK_KING, false, false};
	field[8][6] = {bishop, BLACK_BISHOP, false, false};
	field[8][7] = {knight, BLACK_KNIGHT, false, false};
	field[8][8] = {rook, BLACK_ROOK, false, false};

	for (int i = 1; i < 9; i++) {
		field[i][0] = {number, to_string(i), false, false};
		field[0][i] = {number, string(1, 'A' + i - 1), false, false};
		field[2][i] = {spawn, WHITE_SPAWN, true, false};
		field[7][i] = {spawn, BLACK_SPAWN, false, false};
	}

	for (int i = 3; i < 7; i++)
		for (int j = 1; j < 9; j++)
			field[i][j] = {empty, " ", false, false};
}

void Game::Play() {
	string s;
	bool who = true;

	system("clear");
	Print();

	while(true){
		GetMove(s, who);
		MakeMove();
		system("clear");
		Print();
	}
}