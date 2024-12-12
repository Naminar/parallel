
#include <chrono>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>

// #define ISIZE 1000
// #define JSIZE 1000

int main(int argc, char **argv) {
  // double a[ISIZE][JSIZE];
  unsigned ISIZE = 1000;
  unsigned JSIZE = 3000;

  std::vector<std::vector<double>> a{ISIZE, std::vector<double>(JSIZE, 0)};
  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = 10 * i + j;
    }
  }
  // start
  auto start = std::chrono::high_resolution_clock::now();

  for (i = 2; i < ISIZE; i++) {
    for (j = 0; j < JSIZE - 3; j++) {
      a[i][j] = sin(5 * a[i - 2][j + 3]);
    }
  }
  // end
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms\n";

  ff = fopen("result_serial.txt", "w");
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }
  fclose(ff);
}