#include <iostream>
#include "bits/stdc++.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define INFINITE 1e9
#define INF 1e7 // live 4 or 5
#define DANGER 1e4 // live 3 and dead 4
#define SOSO 1000 // dead 3
#define PREPARE 200

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player, turn, changed_turn = 1, opp = 2; // turn will be 1 if you are black
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

struct node {
	int x, y, alpha, beta, dep, value;
	node(int a, int b, int depth) : x(a), y(b),  alpha(-INFINITE), beta(INFINITE), dep(depth), value(0) {};
};

void read_board(std::ifstream& fin) {
	int b = 0, w = 0;
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
			if (board[i][j] == 1) b++;
			if (board[i][j] == 2) w++;
        }
    }
	if (!b && !w) turn = 1;
	else if (b > w) turn = 0;
	else turn = 1;
	if (!turn) changed_turn = 2, opp = 1;
}

void some(int cnt, int& bval, bool black) {
	if (cnt == 1) bval += 5;
	if (cnt == 2) bval += 75;
	if (cnt == 3) bval += SOSO;
	if (cnt == 4) {
		if (turn != black) bval += INF;
		else bval += DANGER;
	}
	if (cnt == 5) bval += INF;
}

void check(std::array<std::array<int, SIZE>, SIZE> a, int i, int j, int& bval, int& cnt, int& block, bool black) {
	if (black) {
		if (a[i][j] == BLACK) {
			cnt++, block = 0;
		} else if (a[i][j] == WHITE) {
			if (cnt == 1) bval += 5;
			if (cnt == 2) bval += 75;
			if (cnt == 3) bval += SOSO; // dead 3
			if (cnt == 4) {
				if (turn != black) bval += INF;
				else bval += DANGER;
			}// dead 4
			if (cnt == 5) bval += INF;
			cnt = 0, block = 0;
		} else if (a[i][j] == EMPTY) {
			block++;
			if (block == 2) cnt = 0;
			else {
				if (block == 2) cnt = 0;
				if (cnt == 1) bval += 10;
				if (cnt == 2) bval += PREPARE;
				if (cnt == 3) {
					if (turn != black) bval += INF;
					else bval += DANGER;
				}// there is a live 3
				if (cnt > 3) bval += INF; // there is a live 4 or 5
			}
		}
	} else {
		if (a[i][j] == WHITE) {
			cnt++, block = 0;
		} else if (a[i][j] == BLACK) {
			if (cnt == 1) bval += 5;
			if (cnt == 2) bval += 75;
			if (cnt == 3) bval += SOSO; // dead 3
			if (cnt == 4) {
				if (turn == black) bval += INF;
				else bval += DANGER;
			}// dead 4
			if (cnt == 5) bval += INF;
			cnt = 0, block = 0;
		} else if (a[i][j] == EMPTY) {
			if (cnt == 1) bval += 10;
			if (cnt == 2) bval += PREPARE;
			if (cnt == 3) bval += DANGER;// there is a live 3
			if (cnt > 3) bval += INF; // there is a live 4 or 5
			block++;
			if (block == 2) cnt = 0;
		}
	}
}

int get_score(std::array<std::array<int, SIZE>, SIZE> a, bool isblack) {
	int bval = 0;
	for (int i = 0; i < SIZE; ++i) {
		int cnt = 0, block = 0;
		for (int j = 0; j < SIZE; ++j) {
			check(a, i, j, bval, cnt, block, isblack); 
		}
		some(cnt, bval, isblack);
	}
	for (int j = 0; j < SIZE; ++j) {
		int cnt = 0, block = 0;
		for (int i = 0; i < SIZE; ++i) {
			check(a, i, j, bval, cnt, block, isblack);
		}
		some(cnt, bval, isblack);
	}
	for (int k = 0; k <= 2 * (SIZE - 1); k++) {
        int st = std::max(0, k - SIZE + 1);
        int ed = std::min(SIZE - 1, k);
		int cnt = 0, block = 0;
        for (int i = st; i <= ed; i++) {
			int j = k - i;
			check(a, i, j, bval, cnt, block, isblack);
		}
		some(cnt, bval, isblack);
	}
	for (int k = 1 - SIZE; k < SIZE; k++) {
        int st = std::max(0, k);
        int ed = std::min(SIZE + k - 1, SIZE - 1);
		int cnt = 0, block = 0;
        for (int i = st; i <= ed; i++) {
            int j = i - k;	
			check(a, i, j, bval, cnt, block, isblack);
		}
		some(cnt, bval, isblack);
	}
	return bval;
}

std::vector< std::pair<int, int> > find_next(std::array<std::array<int, SIZE>, SIZE> a) {
	std::vector< std::pair<int, int> > re_v;
	for (int i = 0; i < SIZE; ++i)
		for (int j = 0; j < SIZE; ++j)
			if (a[i][j] == EMPTY) {
				if ((i - 1 >= 0 && a[i - 1][j]) || (i + 1 < SIZE && a[i + 1][j]) ||
                (j - 1 >= 0 && a[i][j - 1]) || (j + 1 < SIZE && a[i][j + 1]) ||
                (i - 1 >= 0 && j - 1 >= 0 && a[i - 1][j - 1]) || (i + 1 < SIZE && j - 1 >= 0 && a[i + 1][j - 1]) ||
                (i - 1 >= 0 && j + 1 < SIZE && a[i - 1][j + 1]) || (i + 1 < SIZE && j + 1 < SIZE && a[i + 1][j + 1])) {
					re_v.pb(std::pair<int, int> {i, j});
                }
			}
	return re_v;
}

node minimax(std::array<std::array<int, SIZE>, SIZE> a, int depth, int player) {
	//std::cout << "check xor : " << player << "\n";
	int who = ((player) ? changed_turn : opp); //turn will be 1 if it's black's turn, so does player
	node tmp(7, 7, depth);
	if (depth == 3) {
		int black_val, white_val;
		black_val = get_score(a, true);
		white_val = get_score(a, false);
		//std::cout << "black_val : " << black_val << " white_val : " << white_val << "\n";
		if (turn) tmp.value = black_val - white_val;
		else tmp.value = white_val - black_val;;
	} else {
		(player) ? tmp.value = -INFINITE : tmp.value = INFINITE;
		std::vector< std::pair<int, int> > children = find_next(a);
		for (auto i : children) {
			a[i.fi][i.se] = who;
			node next_step = minimax(a, depth+1, player^1);
			a[i.fi][i.se] = EMPTY;
			if (!player) { // opponent beta = child alpha
				/*tmp.value = std::max(tmp.value, next_step.value);
				tmp.alpha = std::max(tmp.alpha, tmp.value);
				if (tmp.alpha >= tmp.beta) {
					tmp.x = i.fi, tmp.y = i.se;
					return tmp;
				}*/
				if (next_step.value < tmp.value) tmp.x = i.fi, tmp.y = i.se, tmp.value = next_step.value;
			} else { // player alpha = child beta
				/*tmp.value = std::min(tmp.value, next_step.value);
				tmp.beta = std::min(tmp.beta, tmp.value);
				if (tmp.alpha >= tmp.beta) {
					tmp.x = i.fi, tmp.y = i.se;
					return tmp;
				}*/
				if (next_step.value  > tmp.value) tmp.x = i.fi, tmp.y = i.se, tmp.value = next_step.value;
			}
		}
	}
	return tmp;
}

bool check_win(std::array<std::array<int, SIZE>, SIZE> a) {
	for (int i = 0; i < SIZE; ++i) {
		int cnt = 0;
		for (int j = 0; j < SIZE; ++j) {
			if (a[i][j] == changed_turn) cnt++;
			else cnt = 0;
			if (cnt == 5) return true;
		}
	}
	for (int j = 0; j < SIZE; ++j) {
		int cnt = 0;
		for (int i = 0; i < SIZE; ++i) {
			if (a[i][j] == changed_turn) cnt++;
			else cnt = 0;
			if (cnt == 5) return true;
		}
	}
	for (int k = 0; k <= 2 * (SIZE - 1); k++) {
        int st = std::max(0, k - SIZE + 1);
        int ed = std::min(SIZE - 1, k);
		int cnt = 0;
        for (int i = st; i <= ed; i++) {
			int j = k - i;
			if (a[i][j] == changed_turn) cnt++;
			else cnt = 0;
			if (cnt == 5) return true;
		}
	}
	for (int k = 1 - SIZE; k < SIZE; k++) {
        int st = std::max(0, k);
        int ed = std::min(SIZE + k - 1, SIZE - 1);
		int cnt = 0;
        for (int i = st; i <= ed; i++) {
            int j = i - k;	
			if (a[i][j] == changed_turn) cnt++;
			else cnt = 0;
			if (cnt == 5) return true;
		}
	}
	return false;
}

void bala(int i, int j, int& final_x, int& final_y, int& cnt, int& block, int& zero) {
	if (board[i][j] == opp) cnt++;
	else if (board[i][j] == EMPTY) {
		final_x = i, final_y = j;
		block++, zero = 1;
		if (block >= 2) cnt = 0;
	} else cnt = 0, zero = 0;
}

bool need_defense(int& final_x, int& final_y) {
	std::vector< std::pair<int, int> > fistep = find_next(board);
	for (auto i : fistep) {
		for (int i = 0; i < SIZE; ++i) {
			int cnt = 0, block = 0, zero = 0;
			for (int j = 0; j < SIZE; ++j) {
				bala(i, j, final_x, final_y, cnt, block, zero);
				if (cnt == 4 && zero) return true;
				if ((cnt == 4 || (zero && cnt == 3)) && j < SIZE-1 && board[i][j+1] == EMPTY) {
					if (cnt == 4) final_x = i, final_y = j+1;
					return true;
				}
			}
		}
		for (int j = 0; j < SIZE; ++j) {
			int cnt = 0, block = 0, zero = 0;
			for (int i = 0; i < SIZE; ++i) {
				bala(i, j, final_x, final_y, cnt, block, zero);
				if (cnt == 4 && zero) return true;
				if ((cnt == 4 || (zero && cnt == 3)) && i < SIZE-1 && board[i+1][j] == EMPTY) {
					if (cnt == 4) final_x = i+1, final_y = j;
					return true;
				}
			}
		}
		for (int k = 0; k <= 2 * (SIZE - 1); k++) {
			int st = std::max(0, k - SIZE + 1);
			int ed = std::min(SIZE - 1, k);
			int cnt = 0, block = 0, zero = 0;
			for (int i = st; i <= ed; i++) {
				int j = k - i;
				bala(i, j, final_x, final_y, cnt, block, zero);
				if (cnt == 4 && zero) return true;
				if ((cnt == 4 || (zero && cnt == 3))) {
					if (i+1 < SIZE && j > 0 && board[i+1][j-1] == EMPTY) {
						if (cnt == 4) final_x = i+1, final_y = j-1;
						return true;
					}
				}
			}
		}
		for (int k = 1 - SIZE; k < SIZE; k++) {
			int st = std::max(0, k);
			int ed = std::min(SIZE + k - 1, SIZE - 1);
			int cnt = 0, block = 0, zero = 0;
			for (int i = st; i <= ed; i++) {
				int j = i - k;	
				bala(i, j, final_x, final_y, cnt, block, zero);
				if (cnt == 4 && zero) return true;
				if ((cnt == 4 || (zero && cnt == 3))) {
					if (i+1 < SIZE && j+1 < SIZE && board[i+1][j+1] == EMPTY) {
						if (cnt == 4) final_x = i+1, final_y = j+1;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool win_or_lose(int& final_x, int& final_y) {
	std::vector< std::pair<int, int> > fistep = find_next(board);
	for (auto i : fistep) {
		board[i.fi][i.se] = changed_turn;
		if (check_win(board)) {final_x = i.fi, final_y = i.se; return true;}
		board[i.fi][i.se] = EMPTY;
	}
	return false;
}

void write_valid_spot(std::ofstream& fout) {
	int something = 0, final_x, final_y, opponent_x = -1, opponent_y = -1;
	if (win_or_lose(final_x, final_y)) {
		//std::cout << "hello\n";
		fout << final_x << " " << final_y << "\n";
		fout.flush();
		return;
	} else if (need_defense(final_x, final_y)) {
		std::cout << "hello\n";
		fout << final_x << " " << final_y << "\n";
		fout.flush();
		return;
	}
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == changed_turn) something = 1; 
			if (board[i][j] == opp) opponent_x = i, opponent_y = j;
		}
	}
	if (!something) {
		if (opponent_x != -1 && opponent_y != -1) final_x = opponent_x + 2, final_y = opponent_y;
		else final_x = 7, final_y = 7;
		//std::cout << final_x << " " << final_y << "debugging\n";
		fout << final_x << " " << final_y << "\n";
		fout.flush();
	} else {
		node final_node = minimax(board, 0, 1);
		//std::cout << final_node.x << " " << final_node.y << "\n";
		fout << final_node.x << " " << final_node.y << "\n";
		fout.flush();
	}
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
