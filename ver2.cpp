#include "bits/stdc++.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#define pii pair<int, int>
#define fi first
#define se second
#define WIN INT_MAX/5
#define DANGER 1e6 // live3 or dead 4, you must solve it first
#define WATCHOUT 1e5 // live2 or dead 3, be careful about that
#define Nah 100 // dead2 or only 1
using namespace std;


enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player, people, computer, state_val, firststep = 0;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
int score[SIZE][SIZE];

struct node {
	int x, y, val;
	node(int _x, int _y, int who) : x(_x), y(_y), val(0) {
		if (who == people) val = -INT_MAX;
		else val = INT_MAX;
	}; 
};

void read_board(std::ifstream& fin) {
	int b = 0, w = 0;
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
			if (board[i][j] == BLACK) b++;
			if (board[i][j] == WHITE) w++;
        }
    }
	if (!b && !w) people = 1, computer = 2;
	else if (b > w) people = 2, computer = 1;
	else people = 1, computer = 2;
	if (b + w < 2) firststep = 1;
}

bool valid(int a, int b) {
	for (int i = max(0, a-1); i < min(SIZE, a+2); ++i) {
		for (int j = max(0, b-1); j < min(SIZE, b+2); ++j) {
			if (board[i][j] != EMPTY) return true;
		}
	}
	return false;
}

vector< pii > find_next_move() {
	vector< pii > re_vec;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == EMPTY) {
				if (valid(i, j))
					re_vec.push_back( make_pair(i, j) );
			}
		}
	}
	return re_vec;
}

int dont_judge(int cnt, int live, int who) {
	if (cnt == 5) return 1000000;
	if (who == people) {
		if (live == 2) live = 1;
		else if (live == 1) cnt--, live = 2;
	}
	if (cnt == 1 && live == 2) return 3;
	if (cnt == 1 && live == 1) return 1;
	if (cnt == 2 && live == 2) return 50/2;
	if (cnt == 2 && live == 1) return 10/2;
	if (cnt == 3 && live == 2) return 1000/3;
	if (cnt == 3 && live == 1) return 100/3;
	if (cnt == 4 && live == 2) return 5000/4;
	if (cnt == 4 && live == 1) return 1000/4;
	return 0;
}

bool shorting(int i, int j, int who, int& cnt, int& live) {
	if (board[i][j] == who) {cnt++;}
	else if (board[i][j] == EMPTY) {
		live++; 
		return false;
	}
	else return false;
	return true;
}

int count_point(int a, int b, int who) {
	int cnt = 1, re_val = 0, live = 0;
	for (int i = a+1; i < SIZE; ++i) {
		if (shorting(i, b, who, cnt, live)) continue;
		else break;
	}
	for (int i = a-1; i >= 0; --i) {
		if (shorting(i, b, who, cnt, live)) continue;
		else break;
	} re_val += dont_judge(cnt, live, who), cnt = 1, live = 0;
	for (int j = b+1; j < SIZE; ++j) {
		if (shorting(a, j, who, cnt, live)) continue;
		else break;
	}
	for (int j = b-1; j >= 0; --j) {
		if (shorting(a, j, who, cnt, live)) continue;
		else break;
	} re_val += dont_judge(cnt, live, who), cnt = 1, live = 0;
	for (int i = 1; i < SIZE; ++i) {
		if (a+i < SIZE && b+i < SIZE) {
			if (shorting(a+i, b+i, who, cnt, live)) continue;
			else break;
		}
	}
	for (int i = 1; i < SIZE; ++i) {
		if (a-i >= 0 && b-i >= 0) {
			if (shorting(a-i, b-i, who, cnt, live)) continue;
			else break;
		}
	} re_val += dont_judge(cnt, live, who), cnt = 1, live = 0;
	for (int i = 1; i < SIZE; ++i) {
		if (a-i >= 0 && b+i < SIZE) {
			if (shorting(a-i, b+i, who, cnt, live)) continue;
			else break;
		}
	}
	for (int i = 1; i < SIZE; ++i) {
		if (a+i < SIZE && b-i >= 0) {
			if (shorting(a+i, b-i, who, cnt, live)) continue;
			else break;
		}
	} re_val += dont_judge(cnt, live, who);
	return re_val;
}

int find_score(int who) {
	int state_val = 0;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == who) {
				state_val += count_point(i, j, who);
			} else if (board[i][j] == EMPTY && valid(i, j)) {
				int cnt = 0, a = i, b = j, live = 0;

				while (++a < SIZE) { if (board[a][b] == who) cnt++; else break; }
				if (a < SIZE && board[a][b] == EMPTY) live++;
				a = i;
				while (--a >= 0) { if (board[a][b] == who) cnt++; else break; }
				if (a >= 0 && board[a][b] == EMPTY) live++;
				if (cnt >= 5) cnt = 4; state_val += dont_judge(cnt, live, who);

				cnt = 0, a = i, b = j, live = 0;

				while (++b < SIZE){ if (board[a][b] == who) cnt++; else break; }
				if (b < SIZE && board[a][b] == EMPTY) live++;
				b = j;
				while (--b >= 0) { if (board[a][b] == who) cnt++; else break; }
				if (b >= 0 && board[a][b] == EMPTY) live++;
				if (cnt >= 5) cnt = 4;state_val += dont_judge(cnt, live, who);

				cnt = 0, a = i, b = j, live = 0;

				while (++a < SIZE && ++b < SIZE){ if (board[a][b] == who) cnt++; else break; }
				if (a < SIZE && b < SIZE && board[a][b] == EMPTY) live++;
				a = i, b = j;
				while (--a >= 0 && --b >= 0) { if (board[a][b] == who) cnt++; else break; }
				if (a >= 0 && b >= 0 && board[a][b] == EMPTY) live++;
				if (cnt >= 5) cnt = 4;state_val += dont_judge(cnt, live, who);

				cnt = 0, a = i, b = j, live = 0;

				while (++a < SIZE && --b >= 0){ if (board[a][b] == who) cnt++; else break; }
				if (a < SIZE && b >= 0 && board[a][b] == EMPTY) live++;
				a = i, b = j;
				while (--a >= 0 && ++b < SIZE){ if (board[a][b] == who) cnt++; else break; }
				if (a >= 0 && b < SIZE && board[a][b] == EMPTY) live++;
				if (cnt >= 5) cnt = 4;state_val += dont_judge(cnt, live, who);

			}
		}
	}
	return state_val;
}

node alpha_beta(int depth, int alpha, int beta, int who) {
	node nownode(-2, -2, who);
	if (!depth) {
		/*for (int i = 0; i < SIZE; ++i) {
				for (int j = 0; j < SIZE; ++j) {
					cout << board[i][j] << " \n"[j==SIZE-1];
				}
			}*/
		int player_score = find_score(people), computer_score = find_score(computer);
		nownode.val = player_score - computer_score;
		//cout << "check : " << nownode.val << " " << player_score << " " << computer_score << "\n\n";
		return nownode;
	}
	vector< pii > next_move = find_next_move();
	int re_x = next_move[0].fi, re_y = next_move[0].se;
	if (who == player) {
		int max_val = -INT_MAX;
		for (auto it : next_move) {
			board[it.fi][it.se] = who;
			nownode = alpha_beta(depth - 1, alpha, beta, computer);
			board[it.fi][it.se] = EMPTY;
			if (max_val < nownode.val) re_x = it.fi, re_y = it.se, max_val = nownode.val;
			alpha = max(alpha, nownode.val);
			if (alpha >= beta) break;
		}
		nownode.val = max_val, nownode.x = re_x, nownode.y = re_y;
		return nownode;
	} else {
		int min_val = INT_MAX;
		for (auto it : next_move) {
			board[it.fi][it.se] = who;
			nownode = alpha_beta(depth - 1, alpha, beta, computer);
			board[it.fi][it.se] = EMPTY;
			if (min_val > nownode.val) re_x = it.fi, re_y = it.se, min_val = nownode.val;
			beta = min(beta, nownode.val);
			if (alpha >= beta) break;
		}
		nownode.val = min_val, nownode.x = re_x, nownode.y = re_y;
		return nownode;
	}
}

//if I can win in one step otherwise if computer have  dead 4

bool win_or_defense_first(int& x, int& y) {
	vector<pii> next_step = find_next_move();
	for (auto it : next_step) {
		board[it.fi][it.se] = people;
		int tmp = find_score(people);
		if (tmp >= 1000000) {
			x = it.fi, y = it.se;
			board[it.fi][it.se] = EMPTY;
			return true;
		}
		board[it.fi][it.se] = EMPTY;
	}
	for (auto it : next_step) {
		board[it.fi][it.se] = computer;
		int tmp = find_score(computer);
		if (tmp >= 1000000) {
			x = it.fi, y = it.se;
			board[it.fi][it.se] = EMPTY;
			return true;
		}
		board[it.fi][it.se] = EMPTY;
	}
	return false;
}

void write_valid_spot(std::ofstream& fout) {
	int x, y;
	if (firststep) {
		if (board[7][7] == EMPTY) x = 7, y = 7;
		else x = 9, y = 7;
		fout << x << " " << y << "\n";
		fout.flush();
		return;
	}
	/*if (win_or_defense_first(x, y)) {
		fout << x << " " << y << "\n";
		fout.flush();
		return;
	}*/
	node ans_node = alpha_beta(3, -INT_MAX, INT_MAX, people);
	cout << people << " " << computer << "\n";
	fout << ans_node.x << " " << ans_node.y << "\n";
	fout.flush();
	return;
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
