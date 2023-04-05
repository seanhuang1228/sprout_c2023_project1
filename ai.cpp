#include "splendor.hpp"

void ai (Gem game_board[][10], Pos* pos1, Pos* pos2) {
  pos1->x = gen_rand() % BOARD_HEIGHT;
  pos1->y = gen_rand() % BOARD_WIDTH;
  pos2->x = gen_rand() % BOARD_HEIGHT;
  pos2->y = gen_rand() % BOARD_WIDTH;

  return;
}
