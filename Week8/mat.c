#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


#define TAB_SIZE  1000
#define BUF_SIZE  1000


void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB)
{
    for(int i = 0; i < rowsA;i++)
        for(int j = 0; j < colsB; j++)
            for(int k = 0; k < colsA;k++)
                AB[i*colsB + j] += A[i*colsA + k] * B[k*colsB + j];
}

void read_mat(int rows, int cols, double *t)
{
    for(int i = 0;i<rows*cols; i++)
    {
        t[i] = 0;
        double x = 0;
        scanf("%lf", &x);
        t[i] = x;
    }
}

void print_mat(int rows, int cols, double *t)
{
    for(int i =0 ; i < rows; i++)
    {
        for(int j = 0; j<cols; j++)
            printf("%.2lf ",t[i * rows + j]);

        printf("\n");
    }
}

int read_char_lines(char *tab[])
{
    char x = (char )getchar();
    int i = 0;
    while (1)
    {
        x = (char)getchar();
        if(x==-1 || x == '\0' || x == EOF)
            break;

        int bufi = 0;
        char Buffer[BUFSIZ] = { 0 };

        while (x != '\n' && x != '\0' && x != EOF)
        {
            Buffer[bufi] = x;
            ++bufi;
            x = (char )getchar();
        }

        tab[i] = malloc(sizeof(char) * (bufi+1));

        for(int j =0; j < bufi; j++)
            tab[i][j] = Buffer[j];

        tab[i][bufi] = '\0';

        i += 1;
    }

    tab[i] = 0;

    return i;
}

void write_char_line(char *tab[], int n)
{
    int i = 0;
    while(tab[n-1][i] != '\0')
    {
        printf("%c", tab[n-1][i]);
        i += 1;
    }
}

void delete_lines(char *tab[], int line_count)
{
    for(int i = 0; i < line_count; i++)
        free(tab[i]);
}

int read_dbl_lines_v1(double *ptr_tab[])
{
    int count = 1;
    size_t len = 0;
    char *line=NULL;
    double *buff = ptr_tab[0];
    int total_count = 0;

    while(getline(&line, &len, stdin) != -1)
    {
        int has_numbers = 0,num_len = 0;

        double *start = &buff[total_count];
        while(sscanf(line,"%lf%n", &buff[total_count] , &num_len) == 1)
        {
            has_numbers = 1;
            line += num_len;
            total_count++;
        }

        if(has_numbers)
        {
            ptr_tab[count] = start;
            count++;
        }

    }


    return count;
}

void write_dbl_line_v1(double *ptr_tab[], int n)
{
    double *first = ptr_tab[n];
    while (first != ptr_tab[n + 1])
    {
        printf("%.2f ", *first);
        first += 1;
    }
}

int main(void) {

    int to_do;

    scanf ("%d", &to_do);

    double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
    int n, lines_counter, rowsA,colsA,rowsB,colsB;
    char *char_lines_table[TAB_SIZE];
    double series_table[TAB_SIZE];
    double *ptr_table[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d",&rowsA,&colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d",&rowsB,&colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA,colsA,colsB,A,B,C);
            print_mat(rowsA,colsB,C);
            break;
        case 2:
            scanf("%d",&n);
            ptr_table[0] = series_table;
            lines_counter = read_dbl_lines_v1(ptr_table);
            write_dbl_line_v1(ptr_table,n);
            break;
        case 3:
            scanf("%d", &n);
            lines_counter = read_char_lines(char_lines_table);
            write_char_line(char_lines_table,n);
            delete_lines(char_lines_table,lines_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}