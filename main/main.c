#include "app.h"
#include <MLX42/MLX42.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  srand(time(NULL));
  start();
  return EXIT_SUCCESS;
}
