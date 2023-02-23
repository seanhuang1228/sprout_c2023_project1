#include "splendor.hpp"

#define MODE_EXIT 0
#define MODE_STEP 1
#define MODE_SCORE 2

using namespace std;

signed main () {
  int mode = MODE_EXIT;
  while (mode = menu()) {
    main_game(mode);
  }
  return 0;
}

