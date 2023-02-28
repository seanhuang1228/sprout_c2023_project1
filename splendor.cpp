#include "splendor.hpp"
#include <random>

using namespace std;

Gem gameboard[BOARD_HEIGHT][BOARD_WIDTH];
mt19937 mt(1);

int menu() {
  return 1;
}

int gen_rand_type() {
  return mt() % 5 + 1; // TODO: maybe use a faster way to random?
}

void gen_board() {
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      gameboard[i][j].type = gen_rand_type(); // TODO: remain special gem
    }
  }

  Pos bad_pos = {0, 0};
  while (check_eliminate(&bad_pos)) {
    gameboard[bad_pos.x][bad_pos.y].type = gen_rand_type();
  }
  return;
}

void game_init() {
  return;
}

bool check_line(Pos p) {
  int curr_gem = gameboard[p.x][p.y].type;
  if (p.x != 0 and p.x != BOARD_HEIGHT and 
      curr_gem == gameboard[p.x - 1][p.y].type and 
      curr_gem == gameboard[p.x + 1][p.y].type)
    return 1;
  if (p.y != 0 and p.y != BOARD_WIDTH and 
      curr_gem == gameboard[p.x][p.y - 1].type and 
      curr_gem == gameboard[p.x][p.y + 1].type)
    return 1;
  return 0;
}

bool check_eliminate(Pos *pos) {
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      if (check_line({i, j})) {
        if (pos) *pos = {i, j};
        return 1;
      }
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

bool in_board(Pos t) {
  return (t.x >= 0 and t.y >= 0 and t.x < BOARD_WIDTH and t.y < BOARD_HEIGHT);
}

void reset_board() {
  bool is_dead = 1;
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      if (in_board({i, j + 1})) is_dead &= !(check_swap({i, j}, {i, j + 1}));
      if (in_board({i + 1, j})) is_dead &= !(check_swap({i, j}, {i + 1, j}));
    }
  }

  if (is_dead) gen_board(); 

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

    while (check_eliminate(nullptr)) {
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
