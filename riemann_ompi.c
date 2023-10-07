#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

double first_function(double x) {
    return x*x;
}

double second_function(double x) {
    return 2 * x * x * x;
}

double third_function(double x) {
    return sin(x);
}

int main(int argc, char* argv[]) {
   int comm_sz, my_rank;
   int *local_x, *local_y, *local_z;
   MPI_Comm comm;
   double tstart, tend;
   int scalar, local_scalar;
   double a, b;
   int n = 10000000;
   double global_sum = 0;
   int myrank;

   if (argc == 3) {
      a = strtod(argv[1], NULL);
      b = strtod(argv[2], NULL);
   } else {
      a = 0;
      b = 100;
   }

   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   tstart = MPI_Wtime();

   if (myrank == 0) {
      double h = (b - a) / n;
      double division_size = n / comm_sz;
      for (int i = 0; i < comm_sz; i++) {
         MPI_Send(&a, 1, MPI_DOUBLE, i, 0, comm);
         MPI_Send(&h, 1, MPI_DOUBLE, i, 0, comm);
         MPI_Send(&division_size, 1, MPI_DOUBLE, i, 0, comm);
         a += division_size * h;
      }
   }

   double local_a, local_h, local_division_size;

   MPI_Recv(&local_a, 1, MPI_DOUBLE, 0, 0, comm, MPI_STATUS_IGNORE);
   MPI_Recv(&local_h, 1, MPI_DOUBLE, 0, 0, comm, MPI_STATUS_IGNORE);
   MPI_Recv(&local_division_size, 1, MPI_DOUBLE, 0, 0, comm, MPI_STATUS_IGNORE);

   double local_sum = 0;
   for (int i = local_a; i < local_a + local_division_size; i++) {
      double x = local_a + i * local_h;
      local_sum += third_function(x);
   }

   local_sum *= local_h;

   MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

   tend = MPI_Wtime();

   //Print_vector(local_z, local_n, n, "The sum is", my_rank, comm);
   if(my_rank==0) {
      printf("El resultado de la integral es: %f\n", global_sum);
      printf("Tiempo de ejecución: %f s\n", tend - tstart);
   }
   MPI_Finalize();

   return 0;
}