#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


#define TAB_SIZE  1000
#define BUF_SIZE  1000

// ok
void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB)
{
    for(int i = 0; i < rowsA;i++)
        for(int j = 0; j < colsB; j++)
            for(int k = 0; k < colsA;k++)
                AB[i*colsB + j] += A[i*colsA + k] * B[k*colsB + j];
}
// OK
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

// OK
void print_mat(int rows, int cols, double *t)
{
    for(int i =0 ; i < rows; i++)
    {
        for(int j = 0; j<cols; j++)
            printf("%.2lf ",t[i * rows + j]);
        printf("\n");
    }
}
// OK
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
// OK
void write_char_line(char *tab[], int n)
{
    int i = 0;
    while(tab[n-1][i] != '\0')
    {
        printf("%c", tab[n-1][i]);
        i += 1;
    }
}
// OK
void delete_lines(char *tab[], int line_count)
{
    for(int i = 0; i < line_count; i++)
        free(tab[i]);
}
//OK?
int read_dbl_lines_v1(double *ptr_tab[]){

    int count, num_len, len = 0;
    char *line=NULL;
    while(getline(&line, &len, stdin) != -1)
    {
        int number_count = 0, has_numbers = 0;
        double arr[BUFSIZ] = {0};
        double x;
        while(sscanf(line,"%lf%n", &x, &num_len)==1)
        {
            if(x == 33)
                printf("");
            line += num_len;
            has_numbers = 1;
            arr[number_count] = x;
            number_count += 1;
            printf("%.2lf ", x);
        }
        if(has_numbers == 1)
        {
            count++;
            ptr_tab[count] = malloc(sizeof (double ) * (number_count+2));

            for(int z = 0; z < number_count;z++)
                ptr_tab[count][z] = arr[z];

            ptr_tab[count][number_count] = -1;
        }
        int h =0;
    }

    return count;
}

//OK
void write_dbl_line_v1(double *ptr_tab[], int n)
{
    int i = 0;
    while(ptr_tab[n][i] != -1)
    {
        printf("%.2lf ", ptr_tab[n][i]);
        i++;
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
/*
3
2
To jest wiersz 1,
a to drugi.
Trzeciego nie ma.
 */