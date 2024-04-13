#include <iostream>
#include <string>
#include <print>
#include <optional>
#include <vector>
using namespace std;

using i8 = char;
using u8 = unsigned char;
using u16 = unsigned short;

u8 p1[7];
u8 p2[7];
string instr;

optional<u8> stoub(const string& str) {
	if (str.size() == 0 || str.size() > 3) return nullopt;
	u16 result = 0;
	for (u8 i = 0; i < str.size(); i++) {
		if (!isdigit(str[i])) return nullopt;
		result *= 10;
		result += str[i] - '0';
	}
	if (result > 255) return nullopt;
	return result;
}

bool validMove(u8 index, u8* cur) {
	if ((index == 0) || (index > 6)) {
		println("Invalid Move! (must be a number 1-6)");
		return false;
	}
	else if (cur[index - 1] == 0) {
		println("Invalid Move! (cannot choose an empty pocket)");
		return false;
	}
	return true;
}

void printBoard() {
	println(" __________________________________________________________________________________________ ");
	println("|     ______                                                                    ______     |");
	println("|    |      |     ______    ______    ______    ______    ______    ______     |      |    |");
	println("|    |      |    |      |  |      |  |      |  |      |  |      |  |      |    |      |    |");
	println("|    |      |    |  {:02}  |  |  {:02}  |  |  {:02}  |  |  {:02}  |  |  {:02}  |  |  {:02}  |    |      |    |", p2[5], p2[4], p2[3], p2[2], p2[1], p2[0]);
	println("|    |      |    |______|  |______|  |______|  |______|  |______|  |______|    |      |    |");
	println("|    |  {:02}  |                                                                  |  {:02}  |    |", p2[6], p1[6]);
	println("|    |      |     ______    ______    ______    ______    ______    ______     |      |    |");
	println("|    |      |    |      |  |      |  |      |  |      |  |      |  |      |    |      |    |");
	println("|    |      |    |  {:02}  |  |  {:02}  |  |  {:02}  |  |  {:02}  |  |  {:02}  |  |  {:02}  |    |      |    |", p1[0], p1[1], p1[2], p1[3], p1[4], p1[5]);
	println("|    |______|    |______|  |______|  |______|  |______|  |______|  |______|    |______|    |");
	println("|__________________________________________________________________________________________|");
}

bool gameOver(u8* cur, u8* alt) {
	bool curEmpty = true;
	bool altEmpty = true;
	for (u8 i = 0; i < 6; i++) {
		curEmpty &= (cur[i] == 0);
		altEmpty &= (alt[i] == 0);
	}
	return curEmpty || altEmpty;
}

bool makeMove(u8 move, u8* cur, u8* alt) {
	u8 curLim = 6;
	u8 altLim = 5;
	u8 stones = cur[move];
	cur[move] = 0;
	while (stones) {
		if (move == curLim) {
			move = 0;
			swap(cur, alt);
			swap(curLim, altLim);
		}
		else {
			move++;
		}
		cur[move]++;

		stones--;
	}
	if (curLim == 6 && move < 6 && cur[move] == 1 && alt[5 - move] != 0) {
		cur[6] += cur[move] + alt[5 - move];
		cur[move] = 0;
		alt[5 - move] = 0;
	}
	if (gameOver(cur, alt)) {
		for (u8 i = 0; i < 6; i++) {
			cur[6] += cur[i];
			alt[6] += alt[i];
			cur[i] = 0;
			alt[i] = 0;
		}
		return false;
	}
	return move == 6;
}

i8 dfs(u8 move, u8* cur, u8* alt, u8 depth = 10, i8 alpha = -128, i8 beta = 127, bool minimize = false) {

	u8 tempBoard1[7];
	u8 tempBoard2[7];
	memcpy(tempBoard1, cur, 7);
	memcpy(tempBoard2, alt, 7);

	if (minimize) {
		if (makeMove(move, tempBoard2, tempBoard1)) {
			minimize = true;
			depth++;
		}
		else {
			minimize = false;
		}
	}
	else {
		if (makeMove(move, tempBoard1, tempBoard2)) {
			minimize = false;
			depth++;
		}
		else {
			minimize = true;
		}
	}

	if (depth == 0 || gameOver(tempBoard1, tempBoard2)) {
		return tempBoard1[6] - tempBoard2[6];
	}

	i8 score = minimize ? 127 : -128;

	for (u8 i = 0; i < 6; i++) {
		if (minimize) {
			if (tempBoard2[i] != 0) {
				i8 curScore = dfs(i, tempBoard1, tempBoard2, depth - 1, alpha, beta, minimize);
				score = min(score, curScore);
				beta = min(beta, curScore);
				if (alpha > beta) break;
			}
		}
		else {
			if (tempBoard1[i] != 0) {
				i8 curScore = dfs(i, tempBoard1, tempBoard2, depth - 1, alpha, beta, minimize);
				score = max(score, curScore);
				alpha = max(alpha, curScore);
				if (alpha > beta) break;
			}
		}
	}
	return score;
}

bool takeTurn(u8* cur, u8* alt) {
	u8 move;
	do {
		printBoard();
		do {
			print("{} move (1-6): ", cur == p1 ? "Player 1" : "Player 2");
			getline(cin, instr);
			move = stoub(instr).value_or(-1);
		} while (!validMove(move, cur));
	} while (makeMove(move - 1, cur, alt));
	return !gameOver(cur, alt);
}

bool takeTurnAI(u8* cur, u8* alt) {
	u8 bestMove;
	i8 bestScore;
	do {
		printBoard();
		bestMove = 0;
		bestScore = -128;
 		for (u8 i = 0; i < 6; i++) {
			if (cur[i] != 0) {
				i8 score = dfs(i, cur, alt);
				if (score > bestScore || (score == bestScore && rand() % 2)) {
					bestScore = score;
					bestMove = i;
				}
			}
		}
		println("{} move (1-6): {}", cur == p1 ? "Player 1" : "Player 2", static_cast<int>(bestMove));
	} while (makeMove(bestMove, cur, alt));
	return !gameOver(cur, alt);
}

void game0p() {
	for (u8 i = 0; i < 6; i++) {
		p1[i] = 4;
		p2[i] = 4;
	}
	p1[6] = 0;
	p2[6] = 0;

	while (takeTurnAI(p1, p2) && takeTurnAI(p2, p1));
	printBoard();
	if (p1[6] == p2[6]) println("Tie Game!");
	else println("{} Wins!", p1[6] > p2[6] ? "Player 1" : "Player 2");
}

void game1p() {
	for (u8 i = 0; i < 6; i++) {
		p1[i] = 4;
		p2[i] = 4;
	}
	p1[6] = 0;
	p2[6] = 0;

	while (takeTurn(p1, p2) && takeTurnAI(p2, p1));
	printBoard();
	if (p1[6] == p2[6]) println("Tie Game!");
	else println("{} Wins!", p1[6] > p2[6] ? "Player 1" : "Player 2");
}

void game2p() {
	for (u8 i = 0; i < 6; i++) {
		p1[i] = 4;
		p2[i] = 4;
	}
	p1[6] = 0;
	p2[6] = 0;

	u8* cur = p1;
	u8* alt = p2;

	while (takeTurn(cur, alt)) swap(cur, alt);
	printBoard();
	if (p1[6] == p2[6]) println("Tie Game!");
	else println("{} Wins!", p1[6] > p2[6] ? "Player 1" : "Player 2");
}

int main() {
	do {
		print("How many players (0 or 1 or 2)? ");
		getline(cin, instr);
	} while (instr != "0" && instr != "1" && instr != "2");
	auto game = instr == "2" ? game2p : instr == "1" ? game1p : game0p;
	do {
		game();
		do {
			print("Would you like to play again (y/n)? ");
			getline(cin, instr);
		} while (instr != "y" && instr != "n");
	} while (instr != "n");
}