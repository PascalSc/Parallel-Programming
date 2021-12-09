#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int arrayMax(int *array, int size)
{
    int max = array[0], i;
    for (i = 1; i < size; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
        }
    }
    return max;
}

int arrayMin(int *array, int size)
{
    int min = array[0], i;
    for (i = 1; i < size; i++)
    {
        if (array[i] < min)
        {
            min = array[i];
        }
    }
    return min;
}

int main(int argc, char *argv[])
{

    int n = atoi(argv[1]), r = atoi(argv[2]), a[n][n], stage = 1, d[n], e[n], nextRound = 1, minMax[2 * n],
        verbose = atoi(argv[5]);
    int i, ii, j, jj, row, column, count = 0, rightShift = 0, temp = 0, test = 0, b[n][n], downShift = 0, test2 = 0;
    int zzg = atoi(argv[4]);
    double epsilon = atof(argv[3]);
    double result = 0.0;

    omp_set_num_threads(17);
    int num = n * n;
    srand(zzg);

    //init array
    for (int o = 0; o < n; o++)
    {
        for (int p = 0; p < n; p++)
        {
            a[o][p] = o * n + p;
        }
    }

    //random column and row
    while (nextRound == 1)
    {
        for (i = 0; i < n; ++i)
        {
            d[i] = (rand() % 2) + 1;
        }
        for (j = 0; j < n; ++j)
        {
            e[j] = (rand() % 2) + 1;
        }

        //shift right
        for (int k = 0; k < r; ++k)
        {
#pragma omp parallel private(i, j, row, column, temp, test, count, rightShift)
            {
                count = 0;

#pragma omp for schedule(static, 1)
                for (i = 0; i <= num; ++i)
                {
                    if (omp_get_thread_num() != 0)
                    {
                        if (i == 0)
                        {
                            count = 0;
                        }

                        row = ((count * omp_get_num_threads() + omp_get_thread_num()) - 1) / n;
                        column = ((count * omp_get_num_threads() + omp_get_thread_num()) - 1) % n;

                        count++;

                        rightShift = d[column];
                        temp = a[column][row];

                        if ((row + rightShift) >= n)
                        {
                            test = (row + rightShift) - n;
                            b[column][test] = temp;
                        }
                        else
                        {
                            test = row + rightShift;
                            b[column][test] = temp;
                        }
                    }
                    else
                    {

                        --i;
                    }
                }
            }
        }

        //copy array b into a
        for (ii = 0; ii < n; ++ii)
        {
            for (jj = 0; jj < n; ++jj)
            {
                a[ii][jj] = b[ii][jj];
            }
        }

        for (int k = 0; k < r; ++k)
        {
//shift down
#pragma omp parallel private(i, j, row, column, temp, test, count, downShift, test2)
            {
                count = 0;
#pragma omp for schedule(static, 1)
                for (i = 0; i <= num; ++i)
                {

                    if (omp_get_thread_num() != 0)
                    {
                        if (i == 0)
                        {
                            count = 0;
                        }
                        row = ((count * omp_get_num_threads() + omp_get_thread_num()) - 1) / n;
                        column = ((count * omp_get_num_threads() + omp_get_thread_num()) - 1) % n;
                        count++;

                        //down
                        downShift = e[row];
                        temp = a[column][row];
                        if ((column + downShift) >= n)
                        {
                            test2 = (column + downShift) - n;
                            b[test2][row] = temp;
                        }
                        else
                        {
                            test2 = column + downShift;
                            b[test2][row] = temp;
                        }
                    }
                    else
                    {
                        --i;
                    }
                }
            }
        }

        //copy array b into a
        for (ii = 0; ii < n; ++ii)
        {
            for (jj = 0; jj < n; ++jj)
            {
                a[ii][jj] = b[ii][jj];
            }
        }


        //calculate min and max
        int sum = 0;
        for (int l = 0; l < n; ++l)
        {
            for (int u = 0; u < n; ++u)
            {
                if (a[l][u] < 10)
                {
                    sum += a[l][u];
                }
                else if (a[l][u] >= 10)
                {
                    sum += a[l][u];
                }
                else if (a[l][u] >= 100)
                {
                    sum += a[l][u];
                }
            }
            minMax[l] = sum / n;
            sum = 0;
        }

        //
        for (int l = 0; l < n; ++l)
        {
            for (int u = 0; u < n; ++u)
            {
                sum += a[u][l];
            }
            minMax[l + n] = sum / n;
            sum = 0;
        }

        //calculate min and max
        int min = arrayMin(minMax, 2 * n);
        int max = arrayMax(minMax, 2 * n);

        double minMinusMax = max - min;
        result = (double)(minMinusMax / max);

        //print round and check epsilon
        if (verbose == 1)
        {
            printf("Runde  %d: (max - min) / max = %f\n", stage, result);
        }

        if (result >= epsilon)
        {
            stage++;
        }
        else
        {
            nextRound = 0;
        }
    }
    if (verbose == 0 || verbose == 1)
    {
        printf("Runden gesammt: %d\n", stage);
        //print final state of 0
        for (ii = 0; ii < n; ++ii)
        {
            for (jj = 0; jj < n; ++jj)
            {
                if (a[ii][jj] == 0)
                {
                    printf("a(%d, %d) = %d\n", ii, jj, a[ii][jj]);
                }
            }
        }

        if (verbose == 1)
        {
            //print final array
            for (int l = 0; l < n; ++l)
            {
                for (int u = 0; u < n; ++u)
                {
                    if (a[l][u] < 10)
                    {
                        printf("  %d", a[l][u]);
                    }
                    else if (a[l][u] >= 10)
                    {
                        printf(" %d", a[l][u]);
                    }
                    else if (a[l][u] >= 100)
                    {
                        printf("%d", a[l][u]);
                    }
                }
                printf("\n");
            }
        }
        printf("\n");
        printf("\n");
    }
}
