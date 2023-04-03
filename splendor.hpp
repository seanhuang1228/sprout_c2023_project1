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
#define GEM_CNT 6

#define COLOR_NULL "37"
#define COLOR_RUBY "91"
#define COLOR_LAPIZ "34"
#define COLOR_EMERALD "32"
#define COLOR_AMBER "33"
#define COLOR_AMETHYST "35"

#define ABI_NULL 0
#define ABI_NORMAL 1
#define ABI_CROSS 2
#define ABI_BOMB 3
#define ABI_KILLSAME 4
#define ABI_CNT 5

#define DRAW_PAUSE_TIME 1

#define STEP_LIMIT 10
#define SCORE_TARGET 36000

struct Pos {
  int x, y;
};

struct Gem {
  int type;
  int ability;
};

struct ElimiData {
  int total_elimi, mid_elimi, rnd_cnt;
};

struct RecoverData {
  Pos pos;
  Gem gem;
};

int menu();
int main_game(int);

int gen_rand_type();
int gen_rand();
int dist_sq(Pos, Pos);

int check_line(Pos);
bool check_eliminate(Pos*);
bool check_swap(Pos, Pos);
bool check_inboard(Pos);
bool check_dead();

void gen_board();
void eliminate(int);
void gen_special(Pos*, ElimiData, Pos*, int*);

void draw_board(int, double);