#include <iostream>
#include <omp.h>

using namespace std;

#define ROW 100
#define COL 1000
#define NUM_THREADS 32

void shearSort(int matrix[][COL])
{
    int i, j;
    bool swapped = true;

    while (swapped)
    {
        swapped = false;

        #pragma omp parallel private(i, j) shared(swapped)
        {
            #pragma omp for
            for (i = 0; i < ROW; i += 2)
            {
                for (j = 0; j < COL - 1; j++)
                {
                    if (matrix[i][j] > matrix[i][j + 1])
                    {
                        swap(matrix[i][j], matrix[i][j + 1]);
                        swapped = true;
                    }
                }
            }

            #pragma omp for
            for (i = 1; i < ROW; i += 2)
            {
                for (j = COL - 1; j > 0; j--)
                {
                    if (matrix[i][j] > matrix[i][j - 1])
                    {
                        swap(matrix[i][j], matrix[i][j - 1]);
                        swapped = true;
                    }
                }
            }
        }

        for (i = 0; i < ROW - 1; i++)
        {
            #pragma omp parallel for shared(swapped)
            for (j = 0; j < COL; j++)
            {
                if (matrix[i][j] > matrix[i + 1][j])
                {
                    swap(matrix[i][j], matrix[i + 1][j]);
                    swapped = true;
                }
            }
        }
    }
}

void printMatrix(int matrix[][COL])
{
    int i, j;

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            printf(" %d", matrix[i][j]);
        }

        printf("\n");
    }
}

int main()
{
    int i, j;
    int matrix[ROW][COL];
    double startTime, elapsedTime;

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            matrix[i][j] = rand() % 100;
        }
    }

    omp_set_num_threads(NUM_THREADS);

    printf("Unordered Matrix: \n");
    printMatrix(matrix);

    startTime = omp_get_wtime();
    shearSort(matrix);
    elapsedTime = omp_get_wtime() - startTime;

    printf("\n\nOrdered Matrix: \n");
    printMatrix(matrix);

    printf("\nElapsed Time: %f\n", elapsedTime);

    return 0;
}