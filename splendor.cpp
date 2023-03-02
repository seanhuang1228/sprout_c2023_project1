#include "splendor.hpp"
#include <random>

using namespace std;

Gem gameboard[BOARD_HEIGHT][BOARD_WIDTH];
bool moved_tags[BOARD_HEIGHT][BOARD_WIDTH];
bool elimi_tags[BOARD_HEIGHT][BOARD_WIDTH];

mt19937 mt(1);

int menu() {
  return 1;
}

int gen_rand() {
  return mt();
}

int gen_rand_type() {
  return gen_rand() % 5 + 1; // TODO: maybe use a faster way to random?
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
  int ret = 0;
  if (p.x != 0 and p.x != BOARD_HEIGHT and 
      curr_gem == gameboard[p.x - 1][p.y].type and 
      curr_gem == gameboard[p.x + 1][p.y].type)
    ret |= 1;
  if (p.y != 0 and p.y != BOARD_WIDTH and 
      curr_gem == gameboard[p.x][p.y - 1].type and 
      curr_gem == gameboard[p.x][p.y + 1].type)
    ret |= 2;
  return ret;
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

int dist_sq(Pos a, Pos b) {
  return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

bool check_swap(Pos a, Pos b) {
  if (dist_sq(a, b) != 1) return 0;

  Gem tmp = gameboard[a.x][a.y];
  gameboard[a.x][a.y] = gameboard[b.x][b.y];
  gameboard[b.x][b.y] = tmp;

  bool ret = check_eliminate(nullptr);

  tmp = gameboard[a.x][a.y];
  gameboard[a.x][a.y] = gameboard[b.x][b.y];
  gameboard[b.x][b.y] = tmp;

  return ret;
}

void tag_eliminate(Pos pos, Pos buff[], ElimiData *data) {
  if (elimi_tags[pos.x][pos.y]) return;

  elimi_tags[pos.x][pos.y] = 1;
    data->total_elimi++;
  int ret = check_line(pos);
  if (ret) {
    buff[0] = pos;
    buff++;
    data->rnd_cnt++;
    data->mid_elimi++;
  }
  if (ret & 1) {
    tag_eliminate({pos.x - 1, pos.y}, buff, data);
    tag_eliminate({pos.x + 1, pos.y}, buff, data);
  }
  if (ret & 2) {
    tag_eliminate({pos.x, pos.y - 1}, buff, data);
    tag_eliminate({pos.x, pos.y + 1}, buff, data);
  }
  return;
}

void gen_special(Pos *buff, ElimiData data) {
  int abi = ABI_NORMAL;
  if (data.total_elimi == 4) {
    abi = ABI_CROSS;
  }
  else if (data.total_elimi > 4) {
    if (data.total_elimi - data.mid_elimi == 2)
      abi = ABI_KILLSAME;
    else
      abi = ABI_BOMB;
  }

  if (!abi)  return;

  Pos gen_pos = buff[gen_rand() % data.rnd_cnt];
  gameboard[gen_pos.x][gen_pos.y].type = abi;
  elimi_tags[gen_pos.x][gen_pos.y] = 0;
  return;
}

void eliminate() {
  Pos buff[BOARD_HEIGHT * BOARD_WIDTH];
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_HEIGHT; ++j) {
      if (check_line({i, j})) {
        ElimiData elimi_data;
        tag_eliminate({i, j}, buff, &elimi_data);
        gen_special(buff, elimi_data);
      }
    }
  }
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      if (elimi_tags[i][j]) {
        elimi_tags[i][j] = 0;
        gameboard[i][j].type = GEM_NULL;
      }
    }
  }
  return;
}

void draw_board() {
  return;
}

void droping() {
  return;
}

bool check_inboard(Pos t) {
  return (t.x >= 0 and t.y >= 0 and t.x < BOARD_WIDTH and t.y < BOARD_HEIGHT);
}

bool check_dead() {
  bool is_dead = 1;
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      if (check_inboard({i, j + 1})) is_dead &= !(check_swap({i, j}, {i, j + 1}));
      if (check_inboard({i + 1, j})) is_dead &= !(check_swap({i, j}, {i + 1, j}));
    }
  }

  return is_dead;
}

bool game_end(int mode) {
  return 1;
}

void gem_swap(Pos a, Pos b) {
  Gem tmp = gameboard[a.x][a.y];
  gameboard[a.x][a.y] = gameboard[b.x][b.y];
  gameboard[b.x][b.y] = tmp;

  moved_tags[a.x][a.y] = 1;
  moved_tags[b.x][b.y] = 1;
}

int main_game(int mode) {
  int running = 0;
  int step = 0;
  game_init();
  do {
    Pos a, b;

    cin >> a.x >> a.y;
    cin >> b.x >> b.y;

    if (check_swap(a, b)) gem_swap(a, b);
    else continue;

    while (check_eliminate(nullptr)) {
      eliminate();
      draw_board();
      droping();
    }

    step++;
    if (check_dead()) gen_board();

    if (game_end(mode)) running = 0;
  } while (running);
  return 0;
}
