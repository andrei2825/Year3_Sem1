#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int numtasks, rank, flag;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int recv_num;

    // First process starts the circle.
    if (rank == 0) {
        // First process starts the circle.
        MPI_Request request;
        // Generate a random number.
        int x = rand()%100+1;
        // Send the number to the next process.
        MPI_Isend( &x , 1 , MPI_INT , rank+1 , 0 , MPI_COMM_WORLD , &request);
        MPI_Test(&request, &flag, &status);
        if(!flag) {
            MPI_Wait( &request , &status);
        }
        MPI_Irecv( &recv_num , 1 , MPI_INT , numtasks - 1 , 0 , MPI_COMM_WORLD , &request);
        MPI_Test(&request, &flag, &status);
        if(!flag) {
            MPI_Wait( &request , &status);
        }
        printf("Rank: %d and number: %d\n", rank, recv_num);

    } else if (rank == numtasks - 1) {
        MPI_Request request;
        // Last process close the circle.
        // Receives the number from the previous process.
        MPI_Irecv( &recv_num , 1 , MPI_INT , rank - 1 , 0 , MPI_COMM_WORLD , &request);
        MPI_Test(&request, &flag, &status);
        if(!flag) {
            MPI_Wait( &request , &status);
        }
        printf("Rank: %d and number: %d\n", rank, recv_num);
        // Increments the number.
        recv_num +=1;
        // Sends the number to the first process.
        MPI_Isend( &rand , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD , &request);
        MPI_Test(&request, &flag, &status);
        if(!flag) {
            MPI_Wait( &request , &status);
        }

    } else {
        MPI_Request request;
        // Middle process.
        // Receives the number from the previous process.
        MPI_Irecv( &recv_num , 1 , MPI_INT , rank - 1 , 0 , MPI_COMM_WORLD , &request);
        MPI_Test(&request, &flag, &status);
        if(!flag) {
            MPI_Wait( &request , &status);
        }
        printf("Rank: %d and number: %d\n", rank, recv_num);
        // Increments the number.
        recv_num +=1;
        // Sends the number to the next process.
        MPI_Isend( &recv_num , 1 , MPI_INT , rank+1 , 0 , MPI_COMM_WORLD , &request);
        MPI_Test(&request, &flag, &status);
        if(!flag) {
            MPI_Wait( &request , &status);
        }

    }

    MPI_Finalize();

}

