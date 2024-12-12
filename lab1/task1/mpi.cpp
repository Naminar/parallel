#include <chrono>
#include <math.h>
#include <mpi.h>
#include <stdio.h>

#define ISIZE 1000
#define JSIZE 1000

int main(int argc, char *argv[]) {
  int rank, size;
  double a[ISIZE][JSIZE];
  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = 10 * i + j;
    }
  }
  // start
  auto start = std::chrono::high_resolution_clock::now();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Divide the work among processes
  int rows_per_process = ISIZE / size;
  int start_row = rank * rows_per_process + 2; // Adjust for i >= 2
  int end_row = start_row + rows_per_process;

  // Ensure last process handles remaining rows
  if (rank == size - 1) {
    end_row = ISIZE;
  }

  // Perform computation
  for (i = start_row; i < end_row; i++) {
    for (j = 0; j < JSIZE - 3; j++) {
      a[i][j] = sin(5 * a[i - 2][j + 3]);
    }
  }

  // Gather results (if needed, only rank 0 gathers all)
  if (rank == 0) {
    for (int p = 1; p < size; p++) {
      int recv_start_row = p * rows_per_process + 2;
      int recv_end_row = recv_start_row + rows_per_process;

      if (p == size - 1) {
        recv_end_row = ISIZE;
      }

      MPI_Recv(&a[recv_start_row][0], (recv_end_row - recv_start_row) * JSIZE,
               MPI_DOUBLE, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Duration: " << duration.count() << " ms\n";

    ff = fopen("result_mpi.txt", "w");
    for (i = 0; i < ISIZE; i++) {
      for (j = 0; j < JSIZE; j++) {
        fprintf(ff, "%f ", a[i][j]);
      }
      fprintf(ff, "\n");
    }
    fclose(ff);
  } else {
    // Send computed results to rank 0
    MPI_Send(&a[start_row][0], (end_row - start_row) * JSIZE, MPI_DOUBLE, 0, 0,
             MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}