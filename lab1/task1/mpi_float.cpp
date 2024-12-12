#include <cassert>
#include <cmath>
#include <cstdio>
#include <mpi.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>

int main(int argc, char **argv) {
  int world_size;
  int world_rank;
  int ISIZE = 1000;
  int JSIZE = 3000;

  int size, rank;
  int tag = 1;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int chunk = JSIZE / size;
  float *shared_buffer = new float[(chunk + 3) * ISIZE];
  float *global_shared_buffer = new float[(chunk + 3) * ISIZE * size];
  float **localBox = new float *[ISIZE];
  for (int i = 0; i < ISIZE; i++) {
    // localBox[i] = new float[chunk + 3];
    localBox[i] = &shared_buffer[i * (chunk + 3)];
  }
  auto start = std::chrono::high_resolution_clock::now();
  if (rank == 0) {
     start = std::chrono::high_resolution_clock::now();
  }

  for (int i = 0; i < ISIZE; i++)
    for (int j = 0; j < chunk; j++)
      localBox[i][j] = 10 * i + rank * chunk + j;
  if (rank != 0) {
    MPI_Send(&localBox[0][0], 3, MPI_FLOAT, rank - 1, tag, MPI_COMM_WORLD);
    MPI_Send(&localBox[1][0], 3, MPI_FLOAT, rank - 1, tag, MPI_COMM_WORLD);
  }

  if (rank != size - 1) {
    for (int i = 2; i < ISIZE; i++) {
      MPI_Recv(&localBox[i - 2][chunk], 3, MPI_FLOAT, rank + 1, tag,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // std::cout << rank << " <= rank | " << localBox[i - 2][chunk] << " " <<
      // localBox[i - 2][chunk+1] << " " << localBox[i - 2][chunk+2] <<
      // std::endl;
      for (int j = 0; j < chunk; j++)
        localBox[i][j] = sin(5 * localBox[i - 2][j + 3]);

      if (rank != 0)
        MPI_Send(&localBox[i][0], 3, MPI_FLOAT, rank - 1, tag, MPI_COMM_WORLD);
    }
  } else {
    for (int i = 2; i < ISIZE; i++) {
      for (int j = 0; j < chunk - 3; j++)
        localBox[i][j] = sin(5 * localBox[i - 2][j + 3]);
      MPI_Send(&localBox[i][0], 3, MPI_FLOAT, rank - 1, tag, MPI_COMM_WORLD);
    }
  }

  if (rank == 0) {
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Duration: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end -
                                                                      start)
                     .count()
              << " ms\n";}

  // for (int i = 0; i < ISIZE; i++){
  //   for (int j = 0; j < chunk; j++)
  //     std::cout << rank << " <= rank | (ij) | " << i << " " << j << " | "  <<
  //     localBox[i][j] << std::endl;}

  // for (int i = 0; i < ISIZE; i++) {
  //   delete[] localBox[i];
  // }
  MPI_Allgather(shared_buffer, (chunk + 3) * ISIZE, MPI_FLOAT,
                global_shared_buffer, (chunk + 3) * ISIZE, MPI_FLOAT,
                MPI_COMM_WORLD);

  if (rank == 0) {
    // auto end = std::chrono::high_resolution_clock::now();
    // std::cout << "Duration: "
    //           << std::chrono::duration_cast<std::chrono::nanoseconds>(end -
    //                                                                   start)
    //                  .count()
    //           << " ms\n";

    FILE *ff;
    ff = fopen("result_mpi_float.txt", "w");

    for (int i = 0; i < ISIZE; i++) {
      for (int j = 0; j < size; j++) {
        for (int k = 0; k < chunk; k++) {
          fprintf(ff, "%f ",
                  global_shared_buffer[j * (chunk + 3) * ISIZE +
                                       i * (chunk + 3) + k]);
        }
      }
      fprintf(ff, "\n");
    }
  }

  delete[] localBox;
  delete[] shared_buffer;
  delete[] global_shared_buffer;

  MPI_Finalize();

  return 0;
}
