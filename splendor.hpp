#include <iostream>

#define MODE_EXIT 0
#define MODE_STEP 1
#define MODE_SCORE 2

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

#define GEM_NULL 0
#define GEM_RUBY 1
#define GEM_LAPIZ 2
#define GEM_EMERALD 3
#define GEM_AMBER 4
#define GEM_AMETHYST 5

#define ABI_NORMAL 0
#define ABI_CROSS 1
#define ABI_BOMB 2
#define ABI_KILLSAME 3

struct Pos {
  int x, y;
};

struct Gem {
  int type;
  int ability;
};

int menu();
int main_game(int);

int gen_rand_type(int);
int dist_sq(Pos, Pos);

bool check_line(Pos);
bool check_eliminate(Pos*);
bool check_swap(Pos, Pos);
bool check_inboard(Pos);
bool check_dead();

void gen_board();
