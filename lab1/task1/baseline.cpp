#include <cassert>
#include <cmath>
#include <cstdio>
#include <mpi.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

// #define ISIZE 1000
// #define JSIZE 1000

int main(int argc, char **argv) {
  // double a[ISIZE][JSIZE];
  unsigned ISIZE = 1000;
  unsigned JSIZE = 3000;

  int size, rank;
  int tag = 1;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  int chunk = JSIZE * ISIZE / size; 
  float* localBox = new float[chunk];
  // float* sharedBox = new float[JSIZE * ISIZE];
  float *shared_buffer = new float[JSIZE* ISIZE];

  for (int i = 0; i < ISIZE / size; i++) {
    for (int j = 0; j < JSIZE; j++) {
      localBox[i * JSIZE + j] = 10 * (rank*(ISIZE / size)) + j;
    }
  }

  auto start = std::chrono::high_resolution_clock::now();
  if (rank == 0) {
     start = std::chrono::high_resolution_clock::now();
  }

  for (int i = 0; i < ISIZE / size; i++) {
    for (int j = 0; j < JSIZE; j++) {
      localBox[i * JSIZE + j] = sin(5 * localBox[i * JSIZE + j]);
    }
  }


  if (rank == 0) {
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Duration: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end -
                                                                      start)
                     .count()
              << " ms\n";
  }
  MPI_Allgather(localBox, chunk, MPI_FLOAT,
                shared_buffer, chunk, MPI_FLOAT,
                MPI_COMM_WORLD);

    // FILE *ff;
    // ff = fopen("result_baseline.txt", "w");
    // for (int i = 0; i < ISIZE; i++) {
    //   for (int j = 0; j < JSIZE; j++) {
    //     fprintf(ff, "%f ", a[i][j]);
    //   }
    //   fprintf(ff, "\n");
    // }
    // fclose(ff);
  // }
  delete[] localBox;
  delete[] shared_buffer;
  MPI_Finalize();

}