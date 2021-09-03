#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <mpi.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], long int end)
{
    for (int i = 0; i < end; i++)
    {
        vector[i] = rand() % 100;
    }
}

int main(int argc, char **argv)
{
    srand(time(0));

    // initialise mpi
    int numtasks, rank = 0, name_len, world_size = 2;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &name_len);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    unsigned long size = 100000000;

    unsigned long batch_size = size / world_size;

    auto start = high_resolution_clock::now();

    // allocating memory for the arrays
    // v1 is the 1st outgoing array
    int *v1;

    // v2 is the 2nd outgoing array
    int *v2;

    // final array
    int *v3;

    // populating out array in head process
    if (rank == 0)
    {
        // to prevent unnecessory allocations in nodes
        v1 = (int *)malloc(size * sizeof(int *));
        v2 = (int *)malloc(size * sizeof(int *));
        v3 = (int *)malloc(size * sizeof(int *));

        // v1 init
        randomVector(v1, size);

        // v2 init
        randomVector(v2, size);
    }

    // Input of node
    int *data1 = (int *)malloc(batch_size * sizeof(int *));
    int *data2 = (int *)malloc(batch_size * sizeof(int *));
    int *result = (int *)malloc(batch_size * sizeof(int *));

    // mpi scatter
    MPI_Scatter(v1, batch_size, MPI_INT, data1, batch_size, MPI_INT, 0, MPI_COMM_WORLD);

    // mpi scatter
    MPI_Scatter(v2, batch_size, MPI_INT, data2, batch_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (long i = 0; i < batch_size; i++)
    {
        result[i] = data1[i] + data2[i];
    }

    // mpi gatheris one and send_datatype is MPI_INT
    MPI_Gather(result, batch_size, MPI_INT, v3, batch_size, MPI_INT, 0, MPI_COMM_WORLD);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    if (rank == 0)
        cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    MPI_Finalize();
    return 0;
}