#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int recv_num;

    MPI_Status recv_status;

    // First process starts the circle.
    if (rank == 0) {
        srand(42);
        int rnd = rand();
        std::cout<< "proocess " << rank << " sending " << rnd << "\r\n"; 
        MPI_Send(&rnd, 1, MPI_INT, (rank + 1) % numtasks, MPI_COMM_WORLD);
        MPI_Recv(&recv_num, 1, MPI_INT, numtasks - 1, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_status);
        std::cout<< "proocess " << rank << " received " << recv_num << "\r\n"; 

    } else {
        MPI_Recv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &recv_status);
        std::cout<< "proocess " << rank << " received " << recv_num << "\r\n"; 
        recv_num + 2;
        MPI_Send(&recv_num, 1, MPI_INT, (rank + 1) % numtasks, MPI_COMM_WORLD);
        std::cout<< "proocess " << rank << " sending " << rnd << "\r\n"; 

    }

    MPI_Finalize();

}

