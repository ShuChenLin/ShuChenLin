#include "bits/stdc++.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
typedef long long ll;
#define pii pair<int, int>
#define fi first
#define se second
#define pb push_back
#define mp make_pair
using namespace std;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

const int SIZE = 15;
int player, people, computer, state_val, firststep = 0;
ll times = 0;
std::array<std::array<int, SIZE>, SIZE> board;

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
		else cnt--;
	}
	if (cnt == 1 && live == 2) return 2;
	if (cnt == 1 && live == 1) return 1;
	if (cnt == 2 && live == 2) return 25;
	if (cnt == 2 && live == 1) return 10;
	if (cnt == 3 && live == 3) return 2000;
	if (cnt == 3 && live == 2) return 500;
	if (cnt == 3 && live == 1) return 100;
	if (cnt == 4 && live == 2) return 2500;
	if (cnt == 4 && live == 1) return 1000;
	return 0;
}

void find_intersection(vector< pii > tmp, int who) {
	int cnt_two = 0;
	for (int i = 0; i < 8; i+=2) {
		pii a = tmp[i], b = tmp[i+1];
		if (a.fi + b.fi >= 4) dont_judge(4, a.se + b.se, who);
		else if (a.fi + b.fi == 3) dont_judge(3, a.se + b.se, who);
		if (a.fi >= 2) cnt_two++; if (b.fi >= 2) cnt_two++;
	}
	if (cnt_two >= 2) dont_judge(3, 3, who);
	return;
}

bool count_point(int i, int j, int& cnt, int& live, int who) {
	if (board[i][j] == who) cnt++;
	else if (board[i][j] == EMPTY) {
		state_val += dont_judge(cnt, 2, who); 
		live = 1;
		return false;
	}
	else {
		state_val += dont_judge(cnt, 1, who);
		return false;
	}
	return true;
}

int find_score(int who) {
	state_val = 0;
	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			if (board[i][j] == EMPTY && valid(i, j)) {
				int cnt = 0, live = 0;
				vector< pii > tmp;
				for (int k = i+1; k < SIZE; ++k) {
					if (count_point(k, j, cnt, live, who)) continue;
					else break;
				} tmp.pb(mp(cnt, live)) , cnt = 0, live = 0;
				for (int k = i-1; k >= 0; --k) {
					if (count_point(k, j, cnt, live, who)) continue;
					else break;
				} tmp.pb(mp(cnt, live)), cnt = 0, live = 0;
				for (int k = j+1; k < SIZE; ++k) {
					if (count_point(i, k, cnt, live, who)) continue;
					else break;
				} tmp.pb(mp(cnt, live)), cnt = 0, live = 0;
				for (int k = j-1; k >= 0; --k) {
					if (count_point(i, k, cnt, live, who)) continue;
					else break;
				} tmp.pb(mp(cnt, live)), cnt = 0, live = 0;
				for (int k = 1; k < SIZE; ++k) {
					if (i + k >= SIZE || j + k >= SIZE) break;
					else {
						if (count_point(i+k, j+k, cnt, live, who)) continue;
						else break;
					}
				} tmp.pb(mp(cnt, live)), cnt = 0, live = 0;
				for (int k = 1; k < SIZE; ++k) {
					if (i - k < 0 || j - k < 0) break;
					else {
						if (count_point(i-k, j-k, cnt, live, who)) continue;
						else break;
					}
				} tmp.pb(mp(cnt, live)), cnt = 0, live = 0;
				for (int k = 1; k < SIZE; ++k) {
					if (i + k >= SIZE || j - k < 0) break;
					else {
						if (count_point(i+k, j-k, cnt, live, who)) continue;
						else break;
					}
				} tmp.pb(mp(cnt, live)), cnt = 0, live = 0;
				for (int k = 1; k < SIZE; ++k) {
					if (i - k < 0 || j + k >= SIZE) break;
					else {
						if (count_point(i-k, j+k, cnt, live, who)) continue;
						else break;
					}
				} tmp.pb(mp(cnt, live)), cnt = 0, live = 0;
				find_intersection(tmp, who);
			}
		}
	}
	return state_val;
}

node alpha_beta(int depth, int alpha, int beta, int who) {
	node nownode(-2, -2, who);
	if (!depth) {
		int player_score = find_score(people), computer_score = find_score(computer);
		nownode.val = player_score - computer_score;
		//cout << player_score << " " << computer_score << "\n\n";
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
                max_val = nownode.val, re_x = it.fi, re_y = it.se;
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

void write_valid_spot(std::ofstream& fout) {
	int x, y;
	if (firststep) {
		if (board[7][7] == EMPTY) x = 7, y = 7;
		else x = 9, y = 7;
		fout << x << " " << y << "\n";
		fout.flush();
		return;
	}
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
