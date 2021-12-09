#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T 4

int main(int argc, char *argv[], char **envp)
{
    double start, end;

    int n = atoi(argv[1]), z = 2;
    int array[n];
    int arrayCount[T];

    struct padded_int
    {
        int value;
        char padding[60];
    } private_count[T];

    for (int i = 0; i < T; i++)
    {
        private_count[i].value = 0;
    }
    omp_set_num_threads(T);

    //init array
    for (int i = 1; i < n; i++)
    {
        array[i] = i;
    }

    int ar = 0;
    start = omp_get_wtime();
    while (z <= n)
    {

#pragma omp parallel for schedule(static, 1)
        for (int h = 1; h < n; h++)
        {
            if (array[h] != 0)
            {
                arrayCount[omp_get_thread_num()] += 1;
                printf("Thr: %d, array[]: %d\n", omp_get_thread_num(), arrayCount[omp_get_thread_num()]);
                //count++;
            }

            if (array[h] != 0 && arrayCount[omp_get_thread_num()] == z)
            {
                array[h] = 0;
            }

            if (arrayCount[omp_get_thread_num()] == z)
            {
                arrayCount[omp_get_thread_num()] = 0;
            }
        }
#pragma omp parallel
        {
#pragma omp atomic
            arrayCount[omp_get_thread_num()] += private_count[omp_get_thread_num()].value;
        }

        //find next x. number
        int found = 0, m = 3;

        while (found != 1)
        {
            if (array[m] != 0 && array[m] != 1)
            {
                if (z < array[m])
                {
#pragma omp barrier
                    z = array[m];
                    found = 1;
                }
            }
            m++;
        }
        ar++;
    }

    end = omp_get_wtime();
    printf("Benoetigte Zeil: %f Sekunden\n", end - start);

    //print all left numbers
    int counter = 0;
    int highest = 0;
    for (int k = 1; k < n; k++)
    {
        if (array[k] != 0)
        {
            counter++;
            highest = array[k];
        }
    }

    printf("anzahl = %d  max = %d\n ", counter, highest);
}
