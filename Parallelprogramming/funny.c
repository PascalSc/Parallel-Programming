#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareInt(const void *pon, const void *pon2)
{
    const int *point1 = pon;
    const int *point2 = pon2;
    return *point1 - *point2;
}

int main(int argc, char *argv[])
{

    int someInt = 0, true = 0, counter = 0, num = 0, paramA = 0, paramB = 0, thr = 0;


    paramA = atoi(argv[1]);
    paramB = atoi(argv[2]);
    thr = atoi(argv[3]);

    //intervall
    someInt = paramB - paramA;
    if (someInt < 0)
    {
        someInt = 0;
    }
    num = someInt;

    //check case <100 or >= 100
    if (someInt < 100)
    {
        true = 1;
    }
    double start = omp_get_wtime();

    int array[num];
    printf("Zahlen: ");
    omp_set_num_threads(thr);
    int x, i, number, result, test;

#pragma omp parallel for shared(array, counter) private(x, i, number, result, test) schedule(static)
    for (i = 1; i <= num; i++)
    {
        if (i == 1)
        {
#pragma omp critical
            i += omp_get_thread_num();
        }
        x = i;

        while (x != 1 && x != 4)
        {

            result = 0, test = 0;
            number = x;
            while (number > 0)
            {
                test = number % 10;
                result += test * test;
                number /= 10;
            }
            x = result;
        }
        if (x == 1)
        {
            if (true == 1)
            {
#pragma omp atomic write
                array[i - 1] = i;
#pragma omp atomic update
                counter++;
            }
            else
            {
#pragma omp atomic update
                counter++;
            }
        }
        else
        {
#pragma omp atomic write
            array[i - 1] = 0;
        }
    }

    qsort(array, num, sizeof(int), compareInt);
    double end = omp_get_wtime();
    double time = end - start;
    if (true == 1)
    {
        for (int i = 0; i < num; i++)
        {
            if (array[i] != 0)
            {
                printf(" %d", array[i]);
            }
        }
    }
    printf("\nBenoetigte Zeit: %15.15f Sekunden", time);
    printf("\nAnzahl: %d\n", counter);

    return 0;
}
