#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <omp.h>

int main(int argc, char **argv) {
  unsigned ISIZE = 1000;
  unsigned JSIZE = 1000;

  try {
    omp_set_num_threads(std::stoi(argv[1]));
  } catch (...){
    omp_set_num_threads(1);
  }

  std::vector<std::vector<double>> a{ISIZE, std::vector<double>(JSIZE, 0)};
  std::vector<std::vector<double>> b{ISIZE, std::vector<double>(JSIZE, 0)};

  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = 10 * i + j;
    }
  }
  // start
  auto start = std::chrono::high_resolution_clock::now();

  #pragma omp parallel for schedule(guided) collapse(2) private(i, j)
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = sin(0.01 * a[i][j]);
    }
  }

  #pragma omp barrier

  #pragma omp parallel for schedule(guided) collapse(2) private(i, j)
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      b[i][j] = a[i][j] * 2;
    }
  }
    // end
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms\n";

  ff = fopen("baseline.txt", "w");
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      fprintf(ff, "%f ", b[i][j]);
    }
    fprintf(ff, "\n");
  }
  fclose(ff);
}