/*
Ejercicio 2
Uso de 3 operaciones de reducción sobre un array/vector de 5,000 elementos con valores enteros generados de manera aleatoria.
Jose Mariano Reyes
20074
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 5000

int main(int argc, char *argv[])
{
    int rank, size;
    int i;
    int vector[N];
    int sum_result, max_result, min_result;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Generar un vector de valores enteros aleatorios
    if (rank == 0)
    {
        srand(time(NULL));
        for (i = 0; i < N; i++)
        {
            vector[i] = rand() % 1000; // Valores aleatorios entre 0 y 999
        }
    }

    // Repartir el vector entre los procesos
    int elements_per_process = N / size;
    int subvector[elements_per_process];
    MPI_Scatter(vector, elements_per_process, MPI_INT, subvector, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    // Realizar la reducción para calcular la suma, el máximo y el mínimo
    int local_sum = 0;
    int local_max = subvector[0];
    int local_min = subvector[0];

    for (i = 0; i < elements_per_process; i++)
    {
        local_sum += subvector[i];
        if (subvector[i] > local_max)
        {
            local_max = subvector[i];
        }
        if (subvector[i] < local_min)
        {
            local_min = subvector[i];
        }
    }

    MPI_Reduce(&local_sum, &sum_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &max_result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &min_result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    // Imprimir resultados en el proceso 0
    if (rank == 0)
    {
        printf("Operaciones de Reducción\n");
        printf("Resultado de la suma: %d\n", sum_result);
        printf("Resultado del máximo: %d\n", max_result);
        printf("Resultado del mínimo: %d\n", min_result);
    }

    MPI_Finalize();

    return 0;
}
