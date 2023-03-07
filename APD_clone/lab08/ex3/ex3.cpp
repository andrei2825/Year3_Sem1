#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#define MULTI 5 // chunk dimension
#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int num_elements = MULTI * numtasks; // total elements
    // int *v_send = NULL; // full vector
    // int *v_recv = (int *)malloc(MULTI * sizeof(int)); // partial vector

    std::vector<int> v_send;
    std::vector<int> v_recv(MULTI);

    if(rank == ROOT) {
        v_send = std::vector<int>(num_elements, 0);
    }

    MPI_Scatter(v_send.data(), MULTI, MPI_INT, v_recv.data(), MULTI, MPI_INT, ROOT, MPI_COMM_WORLD);

    for(int &v : v_recv) {
        v += rank;
    }

    printf("Process [%d]: have elements %d %d %d %d %d.\n", rank, v_recv[0],
            v_recv[1], v_recv[2], v_recv[3], v_recv[4]);

    MPI_Gather(v_recv.data(), MULTI, MPI_INT, v_send.data(), MULTI, MPI_INT, ROOT, MPI_COMM_WORLD);

    if(rank == ROOT) {
        std::cout<<"Result: ";
        for(const int &v : v_recv) {
        std::cout<<v << ' ';
    }
    std::cout<<" \r\n";

    }
    // Each process increments the values of the partial vector received.
    // Gathers the values from all the processes.
    // The ROOT process prints the elements received.

    MPI_Finalize();

}

