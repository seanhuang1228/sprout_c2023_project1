#include "splendor.hpp"

using namespace std;

Gem gameboard[BOARD_HEIGHT][BOARD_WIDTH];

int menu() {
  return 1;
}

void game_init() {
  return;
}

bool check_line(Pos p, bool test_x) {
  int curr_gem = gameboard[p.x][p.y].type;
  if (test_x and p.x != 0 and p.x != BOARD_HEIGHT and 
      curr_gem == gameboard[p.x - 1][p.y].type and 
      curr_gem == gameboard[p.x + 1][p.y].type)
    return 1;
  else if (p.y != 0 and p.y != BOARD_WIDTH and 
      curr_gem == gameboard[p.x][p.y - 1].type and 
      curr_gem == gameboard[p.x][p.y + 1].type)
    return 1;
  else return 0;
}

bool check_eliminate() {
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      if (check_line((Pos){i, j}, true)) return 1;
    }
  }
  return 0;
}

bool check_swap(Pos a, Pos b) {
  return 1;
}

void eliminate() {
  return;
}

void draw_board() {
  return;
}

void droping() {
  return;
}

void reset_board() {
  return;
}

bool game_end(int mode) {
  return 1;
}

int main_game(int mode) {
  int running = 0;
  int step = 0;
  game_init();
  do {
    Pos a, b;

    cin >> a.x >> a.y;
    cin >> b.x >> b.y;

    if (!check_swap(a, b)) continue;

    while (check_eliminate()) {
      eliminate();
      draw_board();
      droping();
    }

    step++;
    reset_board();

    if (game_end(mode)) running = 0;
  } while (running);
  return 0;
}
