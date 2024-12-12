#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

int main(int argc, char **argv) {
  unsigned ISIZE = 1000;
  unsigned JSIZE = 1000;

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

  for (i = 0; i < ISIZE - 1; i++) {
    for (j = 6; j < JSIZE; j++) {
      a[i][j] = sin(0.2 * a[i + 1][j - 6]);
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