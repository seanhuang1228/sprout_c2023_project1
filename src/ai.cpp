#include "splendor.hpp"
#include <queue>

#define travel_board() for(int i = 0; i < BOARD_HEIGHT; ++i) for(int j = 0; j < BOARD_WIDTH; ++j)
#define get_bd(a, b) ((a)[(b).x][(b).y])

#define DIR_CNT 4

using namespace std;

Gem my_board[BOARD_HEIGHT][BOARD_WIDTH];
Pos my_dir[DIR_CNT] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

int my_check_line(Pos p) {
  int curr_gem = my_board[p.x][p.y].type;
  int ret = 0;
  if (curr_gem == GEM_NULL) return ret;
  if (p.x != 0 and p.x != BOARD_HEIGHT - 1 and 
      curr_gem == my_board[p.x - 1][p.y].type and 
      curr_gem == my_board[p.x + 1][p.y].type)
    ret |= 1;
  if (p.y != 0 and p.y != BOARD_WIDTH - 1 and 
      curr_gem == my_board[p.x][p.y - 1].type and 
      curr_gem == my_board[p.x][p.y + 1].type)
    ret |= 2;
  return ret;
}

int cal_nei(Pos pos) {
  static int is_visited[BOARD_HEIGHT][BOARD_WIDTH];
  int cnt = 0;
  queue<Pos> waiting;

  travel_board() {
    is_visited[i][j] = 0;
  }

  waiting.push(pos);

  get_bd(is_visited, pos) = 1;
  while (!waiting.empty()) {
    Pos curr_p = waiting.front();
    waiting.pop();

    if (get_bd(my_board, curr_p).ability == ABI_CROSS)
      cnt += 19;
    else
      cnt++;
    
    int ret = my_check_line(curr_p);
    if ((ret & 1) && !is_visited[curr_p.x + 1][curr_p.y]) {
      is_visited[curr_p.x + 1][curr_p.y] = 1;
      waiting.push({curr_p.x + 1, curr_p.y});
    }
    if ((ret & 1) && !is_visited[curr_p.x - 1][curr_p.y]) {
      is_visited[curr_p.x + 1][curr_p.y] = 1;
      waiting.push({curr_p.x - 1, curr_p.y});
    }
    if ((ret & 2) && !is_visited[curr_p.x][curr_p.y + 1]) {
      is_visited[curr_p.x][curr_p.y + 1] = 1;
      waiting.push({curr_p.x, curr_p.y + 1});
    }
    if ((ret & 2) && !is_visited[curr_p.x][curr_p.y - 1]) {
      is_visited[curr_p.x][curr_p.y - 1] = 1;
      waiting.push({curr_p.x, curr_p.y - 1});
    }
  }

  return cnt;
}

int cal_g() {
  Pos arr[BOARD_HEIGHT * BOARD_WIDTH];
  int idx = 0;

  travel_board() {
    if (my_check_line({i, j})) {
      arr[idx++] = {i, j};
    }
  }

  int ret = 0;
  for (int i = 0; i < idx; ++i) {
    ret = max(ret, cal_nei(arr[i]));
  }
  return ret;
}

void ai (Pos& pos1, Pos& pos2) {
  // pos1.x = gen_rand() % BOARD_HEIGHT;
  // pos1.y = gen_rand() % BOARD_WIDTH;
  // pos2.x = gen_rand() % BOARD_HEIGHT;
  // pos2.y = gen_rand() % BOARD_WIDTH;
  
  for (int i = 0; i < BOARD_HEIGHT; ++i) {
    for (int j = 0; j < BOARD_WIDTH; ++j) {
      my_board[i][j] = get_gem({i, j});
    }
  }

  int grade = 0;
  Pos tmp1, tmp2;
  for (int i = 0; i < BOARD_HEIGHT - 1; ++i) {
    for (int j = 0; j < BOARD_WIDTH - 1; ++j) {
      if (check_swap({i, j}, {i + 1, j})) {
        swap(my_board[i][j], my_board[i + 1][j]);
        int curr_g = cal_g();
        swap(my_board[i][j], my_board[i + 1][j]);
        if (curr_g > grade) {
          tmp1 = {i, j};
          tmp2 = {i + 1, j};
          grade = curr_g;
        }
      }
      if (check_swap({i, j}, {i, j + 1})) {
        swap(my_board[i][j], my_board[i][j + 1]);
        int curr_g = cal_g();
        swap(my_board[i][j], my_board[i][j + 1]);
        if (curr_g > grade) {
          tmp1 = {i, j};
          tmp2 = {i, j + 1};
          grade = curr_g;
        }
      }
    }
  }

  travel_board() {
    Pos curr_p = {i, j};
    if (get_bd(my_board, curr_p).ability == ABI_BOMB) {
      int curr_g = 0;
      for (int k = -2; k <= 2; ++k) {
        for (int l = -2; l <= 2; ++l) {
          if (check_inboard({i + k, j + l})) {
            if (my_board[i + k][j + l].ability == ABI_CROSS)
              curr_g++;
            else
              curr_g++;
          }
        }
      }
      if (curr_g > grade) {
        grade = curr_g;
        tmp1 = {i, j};
        for (int k = 0; k < DIR_CNT; ++k) {
          if (check_inboard({i + my_dir[k].x, j + my_dir[k].y})) {
            tmp2 = {i + my_dir[k].x, j + my_dir[k].y};
          }
        }
      }
    }
    if (get_bd(my_board, curr_p).ability == ABI_KILLSAME) {
      for (int k = 0; k < DIR_CNT; ++k) {
        int curr_g = 0;
        if (check_inboard({i + my_dir[k].x, j + my_dir[k].y})) {
          int tar = my_board[i + my_dir[k].x][j + my_dir[k].y].type;
          for (int l = 0; l < BOARD_HEIGHT; ++l) {
            for (int m = 0; m < BOARD_WIDTH; ++m) {
              if (my_board[l][m].type == tar) curr_g++;
            }
          }
        }
        if (curr_g > grade) {
          grade = curr_g;
          tmp1 = {i, j};
          tmp2 = {i + my_dir[k].x, j + my_dir[k].y};
        }
      }
    }
  }

  pos1 = tmp1;
  pos2 = tmp2;

  return;
}
