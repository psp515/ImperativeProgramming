#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n)
{
    for(int i = 0; i < n; ++i)
        scanf("%lf", x++);

}

void print_vector(double x[], int n)
{
    for(int i = 0; i < n; ++i)
        printf("%.4f ", x[i]);

    printf("\n");
}

void read_mat(double A[][SIZE], int m, int n)
{
    for(int i = 0; i < m; ++i)
        for(int j = 0; j < n; ++j)
            scanf("%lf", &A[i][j]);
}

void print_mat(double A[][SIZE], int m, int n)
{
    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < n; ++j)
            printf("%.4f ", A[i][j]);

        printf("\n");
    }
}

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n)
{
    for(int i =0;i < m;i++)
    {
        for(int j = 0; j < n; j++)
        {
            AB[i][j] = 0;
            for(int k = 0; k < p;k++)
                AB[i][j] += A[i][k]*B[k][j];

        }
    }
}

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n)
{
    double score = 1;
    for(int i = 0; i < n; i++)
    {
        if(A[i][i] == 0)
            return NAN;

        score *= A[i][i];

        for(int j = i + 1; j < n; j++)
        {
            double ratio = A[j][i] / A[i][i];
            for(int k = 0; k < n; k++)
                A[j][k] -= A[i][k] * ratio;

        }
    }
    return score;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps)
{
    int index[n];

    for(int i = 0; i < n; i++)
        index[i] = i;

    if(b!=NULL)
        for (int i = 0; i < n; ++i)
            x[i] = b[i];


    double det = 1;
    int swaps=0;
    for(int i = 0; i < n - 1; i++)
    {

        int max_index = i;
        for(int j = i+1; j < n; j++)
            if (fabs(A[index[j]][i]) > fabs(A[index[max_index]][i]))
                max_index = j;

        if(i != max_index)
            swaps++;

        int tmp = index[i];
        index[i] = index[max_index];
        index[max_index] = tmp;

        if(b!=NULL)
        {
            double tmp_b = x[i];
            x[i] = x[max_index];
            x[max_index] = tmp_b;
        }

        for(int j = i + 1; j < n; j++){
            double ratio = A[index[j]][i] / A[index[i]][i];

            if(b!=NULL)
                x[j] -= (ratio * x[i]);

            for(int k = i; k < n; k++)
                A[index[j]][k] -= (A[index[i]][k] * ratio);

        }

    }

    for(int i = 0; i < n; i++)
        det *= A[index[i]][i];

    for(int i = 0; i < swaps;i++)
        det*=-1;

    if(det != 0 && x != NULL && b!= NULL)
    {
        for(int i = n - 1; i >=0; i--)
        {
            for(int j = n - 1; j > i; j--)
                x[i] -= (A[index[i]][j] * x[j]);

            x[i] = x[i] / A[index[i]][i];
        }
    }

    return det;
}

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps)
{
    double copy[SIZE][SIZE];
    for(int i = 0 ; i < n; i++)
        for(int j = 0; j < n; j++)
            copy[i][j] = A[i][j];



    for(int i = 0; i < n; i++)
        B[i][i] = 1;

    for(int i=0;i<n;i++)
    {
        double item = A[i][i];
        for(int j = 0; j < n;j++)
        {
            A[i][j] /= item;
            B[i][j] /= item;
        }

        for(int j = 0; j < n; j++)
        {
            if(j == i)
                continue;

            double increaser = A[j][i];

            for(int k=0; k<n; k++)
            {
                A[j][k]-= increaser * A[i][k];
                B[j][k]-= increaser * B[i][k];
            }
        }
    }


    return gauss(copy,NULL,NULL,n,eps);
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;

    int to_do;
    int m, n, p;

    scanf ("%d", &to_do);

    switch (to_do) {
        case 1:
            scanf("%d %d %d", &m, &p, &n);
            read_mat(A, m, p);
            read_mat(B, p, n);
            mat_product(A, B, C, m, p, n);
            print_mat(C, m, n);
            break;
        case 2:
            scanf("%d", &n);
            read_mat(A, n, n);
            printf("%.4f\n", gauss_simplified(A, n));
            break;
        case 3:
            scanf("%d", &n);
            read_mat(A,n, n);
            read_vector(b, n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n", det);
            if(det) print_vector(x, n);
            break;
        case 4:
            scanf("%d", &n);
            read_mat(A,n,n);
            printf("%.4f\n",matrix_inv(A, B, n, eps));
            print_mat(B, n, n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

