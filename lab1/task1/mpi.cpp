#include <cassert>
#include <cmath>
#include <cstdio>
#include <mpi.h>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  int world_size;
  int world_rank;
  int ISIZE = 10;
  int JSIZE = 12;

  int size, rank;
  int tag = 1;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int chunk = JSIZE / size;
  int **localBox = new int *[ISIZE];
  for (int i = 0; i < ISIZE; i++) {
    localBox[i] = new int[chunk + 3];
  }

  // for (int i = 0; i < ISIZE; i++) {
  //     delete[] localBox[i];
  // }
  // delete[] localBox;

  // if (rank = 0) {
  for (int i = 0; i < ISIZE; i++)
    for (int j = 0; j < chunk; j++)
      localBox[i][j] = 10 * i + rank * chunk + j;
  // }
  if (rank != 0) {
    MPI_Send(&localBox[0][0], 3, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
    MPI_Send(&localBox[1][0], 3, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
  }

  if (rank != size - 1) {
    for (int i = 2; i < ISIZE; i++) {
      MPI_Recv(&localBox[i - 2][chunk], 3, MPI_INT, rank + 1, tag,
               MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // std::cout << rank << " <= rank | " << localBox[i - 2][chunk] << " " <<
      // localBox[i - 2][chunk+1] << " " << localBox[i - 2][chunk+2] <<
      // std::endl;
      for (int j = 0; j < chunk; j++)
        localBox[i][j] =
            localBox[i - 2][j + 3]; // sin(5 * localBox[i - 2][j + 3]);

      if (rank != 0)
        MPI_Send(&localBox[i][0], 3, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
    }
  } else {
    for (int i = 2; i < ISIZE; i++) {
      for (int j = 0; j < chunk - 3; j++)
        localBox[i][j] =
            localBox[i - 2][j + 3]; // sin(5 * localBox[i - 2][j + 3]);
      MPI_Send(&localBox[i][0], 3, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
    }
  }

  for (int i = 0; i < ISIZE; i++) {
    for (int j = 0; j < chunk; j++)
      std::cout << rank << " <= rank | (ij) | " << i << " " << j << " | "
                << localBox[i][j] << std::endl;
  }

  for (int i = 0; i < ISIZE; i++) {
    delete[] localBox[i];
  }
  delete[] localBox;

  MPI_Finalize();

  return 0;
}
