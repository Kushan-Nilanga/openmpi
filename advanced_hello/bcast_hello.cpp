#include "mpi.h"
#include <string.h>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{

    int numtasks, rank, name_len;
    char name[MPI_MAX_PROCESSOR_NAME];
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of tasks/process
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    // Get the rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Find the processor name
    MPI_Get_processor_name(name, &name_len);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    char data[20];

    if(rank==0){
        strcpy(data, "Hello world");
    }

    // Bcase is like someone talking in a group, Him and the rest of the group can hear all of the things he say at the same thing
    MPI_Bcast(&data, 12, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    

    printf("Recieved data %s on %s with rank %d\n", data, name, rank);
    

    // Finalize the MPI environment
    MPI_Finalize();

    return 1;
}