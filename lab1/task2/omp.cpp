#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <stdexcept>
#include <omp.h>
#include <chrono>
#include <iostream>

int main(int argc, char **argv)
{
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
    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            b[i][j] = 10*i +j;
        }
    }
  // start
  auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for collapse(2) private(i, j)
    for (i=0; i<ISIZE-1; i++){
        for (j = 6; j < JSIZE; j++){
            a[i][j] = sin(0.2*b[i+1][j-6]);
        }
    }

    for (i=ISIZE-1; i<ISIZE; i++){
        for (j = 6; j < JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }

    for (i=0; i<ISIZE-1; i++){
        for (j = 5; j >= 0; j--){
            a[i][j] = 10*i+j;
        }
    }

  // end
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms\n";

  ff = fopen("result_omp.txt", "w");
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }
  fclose(ff);
}