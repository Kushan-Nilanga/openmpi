#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <mpi.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], long int start, long int end, int batchsize)
{
    bool skip = false;
    for (int i = start; i < end; i++)
    {
        if (skip == false)
            vector[i] = rand() % 100;
        if (i % batchsize == 0 && i != start)
            skip = !skip;
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

    auto start = high_resolution_clock::now();

    unsigned long batch_size = size / world_size;
    int *v1 = (int *)malloc(2 * size * sizeof(int *));
    int *v2 = (int *)malloc(size * sizeof(int *));

    if (rank == 0)
    {
        randomVector(v1, 0, 2 * size, batch_size);
        randomVector(v1, batch_size, 2 * size, batch_size);
    }

    int *data = (int *)malloc(2 * batch_size * sizeof(int *));

    // mpi scatter
    MPI_Scatter(v1, 2 * size, MPI_INT, data, 2 * batch_size, MPI_INT, 0, MPI_COMM_WORLD);

    int *result = (int *)malloc(batch_size * sizeof(int *));
    for (long i = 0; i < batch_size; i++)
    {
        result[i] = data[i] + data[batch_size + i];
    }

    // mpi gather
    MPI_Gather(result, batch_size, MPI_INT, v2, size, MPI_INT, 0, MPI_COMM_WORLD);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    MPI_Finalize();
    return 0;
}