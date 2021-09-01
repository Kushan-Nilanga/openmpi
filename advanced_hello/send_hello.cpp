#include "mpi.h"
#include <string.h>
#include <iostream>

using namespace std;

void node(char *name, int rank)
{
    char data[11];
    void *status;
    MPI_Recv(&data, 11, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Recieved data %s on %s with rank %d\n", data, name, rank);
}

void head(int dest)
{
    char data[] = {"Hello World"};
    MPI_Send(&data, strlen(data), MPI_UNSIGNED_CHAR, dest, 0, MPI_COMM_WORLD);
}

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

    switch (rank)
    {
    case 0:
        for (int i = 1; i < world_size; i++)
        {
            head(i);
        }
        break;

    default:
        node(name, rank);
        break;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 1;
}