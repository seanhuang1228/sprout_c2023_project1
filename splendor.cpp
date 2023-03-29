#include "splendor.hpp"
#include <bits/stdc++.h>

using namespace std;

Gem gameboard[BOARD_HEIGHT][BOARD_WIDTH];
bool moved_tags[BOARD_HEIGHT][BOARD_WIDTH];
bool elimi_tags[BOARD_HEIGHT][BOARD_WIDTH];

mt19937 mt(1);

int menu() {
  return 1;
}

int gen_rand() {
  return abs((int)mt());
}

int gen_rand_type() {
  return gen_rand() % 5 + 1; // TODO: maybe use a faster way to random?
}

string get_color(int type) {
  static string color_array[GEM_CNT] = {COLOR_NULL, COLOR_RUBY, COLOR_LAPIZ, COLOR_EMERALD, COLOR_AMBER, COLOR_AMETHYST};
  return color_array[type];
}

char get_style(int ability) {
  static char ability_array[ABI_CNT] = {' ', 'o', '+', 'Q', 'z'};
  return ability_array[ability];
}

void gen_board() {
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      gameboard[i][j].type = gen_rand_type(); // TODO: remain special gem
      gameboard[i][j].ability = gameboard[i][j].ability ? gameboard[i][j].ability : ABI_NORMAL;
    }
  }

  Pos bad_pos = {0, 0};
  while (check_eliminate(&bad_pos)) {
    gameboard[bad_pos.x][bad_pos.y].type = gen_rand_type();
  }
  return;
}


int check_line(Pos p) {
  int curr_gem = gameboard[p.x][p.y].type;
  int ret = 0;
  if (p.x != 0 and p.x != BOARD_HEIGHT - 1 and 
      curr_gem == gameboard[p.x - 1][p.y].type and 
      curr_gem == gameboard[p.x + 1][p.y].type)
    ret |= 1;
  if (p.y != 0 and p.y != BOARD_WIDTH - 1 and 
      curr_gem == gameboard[p.x][p.y - 1].type and 
      curr_gem == gameboard[p.x][p.y + 1].type)
    ret |= 2;
  return ret;
}

bool check_eliminate(Pos *pos) {
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      if (check_line({i, j})) {
#ifdef DEBUG
        //cout << "line success at " << i << ", " << j << '\n';
#endif
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
  cout << "tags: " << "(" << pos.x << ", " << pos.y << ")\n";
  data->total_elimi++;
  if (moved_tags[pos.x][pos.y])
    buff[data->rnd_cnt++] = pos;
  int ret = check_line(pos);
  cout << "ret: " << ret << "\n";
  if (ret)
    data->mid_elimi++;
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

void gen_special(Pos *buff, ElimiData data, Pos* r_data, int *idx) {
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

  if (abi == ABI_NORMAL) return;

  r_data[*idx] = buff[gen_rand() % data.rnd_cnt];
  gameboard[r_data[*idx].x][r_data[*idx].y].ability = abi;
  *idx = *idx + 1;
  return;
}

void eliminate() {
  Pos buff[BOARD_HEIGHT * BOARD_WIDTH];
  Pos recover_data[BOARD_HEIGHT * BOARD_WIDTH];
  int recover_idx = 0;

  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_HEIGHT; ++j) {
      if (check_line({i, j})) {
        ElimiData elimi_data = {};
        tag_eliminate({i, j}, buff, &elimi_data);
        gen_special(buff, elimi_data, recover_data, &recover_idx);
      }
    }
  }
  for (int i = 0; i < recover_idx; ++i) {
    elimi_tags[recover_data[i].x][recover_data[i].y] = 0;
  }
  draw_board(2);
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      moved_tags[i][j] = 0;
      if (elimi_tags[i][j]) {
        elimi_tags[i][j] = 0;
        gameboard[i][j].type = GEM_NULL;
        gameboard[i][j].ability = ABI_NULL;
      }
    }
  }
  return;
}

void clean_color() {
  cout << "\x1b[0m";
}

void draw_board(int time) {
  system("clear");
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      string color = get_color(gameboard[i][j].type);
      string deco = elimi_tags[i][j] ? "5" : "0";
      string ansi = "\x1b[" + deco + ";" + color + "m";
      cout << ansi << get_style(gameboard[i][j].ability) << ' ';
    }
    cout << '\n';
  }
  clean_color();
  sleep(time);
  return;
}

void draw_board() {
  draw_board(1);
}

void droping() {
  for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      if (gameboard[i][j].type != GEM_NULL) continue;
      int curr_height = i;
      while (check_inboard({curr_height, j}) and gameboard[curr_height][j].type == GEM_NULL)
        curr_height--;
      if (check_inboard({curr_height, j})) {
        swap(gameboard[curr_height][j], gameboard[i][j]);
      }
    }
  }

  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      if (gameboard[i][j].type == GEM_NULL) gameboard[i][j].type = gen_rand_type(), gameboard[i][j].ability = ABI_NORMAL;
    }
  }
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
  return 0;
}

void gem_swap(Pos a, Pos b) {
  swap(gameboard[a.x][a.y], gameboard[b.x][b.y]);

  moved_tags[a.x][a.y] = 1;
  moved_tags[b.x][b.y] = 1;
}

void game_init() {
  gen_board();
  draw_board();
#ifdef DEBUG
  cout << "init done\n";
#endif
  return;
}

int main_game(int mode) {
  int running = 1;
  int step = 0;
  game_init();
  do {
    Pos a, b;

    // cout << "input two pos (0 base): \n";
    cin >> a.x >> a.y;
    cin >> b.x >> b.y;

    if (check_swap(a, b)) {
#ifdef DEBUG
    cout << "after swap check\n";
    draw_board();
#endif
      gem_swap(a, b);
      draw_board();
    }
    else continue;

#ifdef DEBUG
    cout << "after swap\n";
    draw_board();
#endif

    while (check_eliminate(nullptr)) {
      eliminate();
#ifdef DEBUG
      cout << "after eli\n";
#endif
      draw_board();
      droping();
#ifdef DEBUG
      cout << "after dropping\n";
#endif
      draw_board();
    }

    step++;
    if (check_dead()) gen_board();

    if (game_end(mode)) running = 0;
  } while (running);
  return 0;
}
