#include "launcher.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  srand(time(NULL));
  launcher_start();  // let the user choose a project
  return 0;
}
