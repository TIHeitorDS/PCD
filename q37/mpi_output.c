/* File:     mpi_output.c
 *
 * Purpose:  A program in which multiple MPI processes try to print 
 *           a message.
 *
 * Compile:  mpicc -g -Wall -o mpi_output mpi_output.c
 * Usage:    mpiexec -n<number of processes> ./mpi_output
 *
 * Input:    None
 * Output:   A message from each process
 *
 * IPP:      Section 3.3.1  (pp. 97 and ff.)
 */
#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main(void) {
   int my_rank, comm_sz;
   char orders[MAX_STRING];

   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank != 0) {
      sprintf(orders, "Proc %d of %d > Does anyone have a toothpick?\n", my_rank, comm_sz);
      MPI_Send(orders, strlen(orders)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
   }  else {
      printf("Proc %d of %d > Does anyone have a toothpick?\n", my_rank, comm_sz);
      
      for (int q = 1; q < comm_sz; q++) {
            MPI_Recv(orders, MAX_STRING, MPI_CHAR, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", orders);
      }
   }

   MPI_Finalize();
   return 0;
}  /* main */
