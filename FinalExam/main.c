#include <stdio.h>
#include <math.h>
int main()
{
    printf("Hello, World!\n");
    return 0;
}
typedef struct _matrix{
    int rows;
    int cols;
    double**val;
    double*storage;
}Matrix;
void print_marix(const Matrix* pm)
{
    printf("{");

    for(int i =0;i< pm->rows; i++)
    {
        printf("{");
        for (int j =0;j< pm->cols; j++)
            printf("%f, ", pm->val[i][j]);
        printf("}");

        if (i != pm->rows-1)
            printf(",");
    }

    printf("}");
}
int swap_rows(Matrix * m,int r1,int r2)
{
    if (r1 >= m->rows || r1 < 0 || r2 >= m->rows || r2 < 0)
        return 0;

    for(int i = 0; i < m->cols; i++)
    {
        double tmp = m->val[r1][i];
        m->val[r1][i] = m->val[r2][i];
        m->val[r2][i] = tmp;
    }

    return 1; // nie podano co trzeba zrobic po udaniu
}
int inplace_add(Matrix*left,const Matrix*right)
{
    if(left->cols != right->cols || left->rows != right->rows)
        return 0;

    for(int i =0;i< left->rows;i++)
        for(int j = 0; j < left->cols;j++)
            left->val[i][j] += right->val[i][j];

    return  1;
}
void mul_vect(double *result, const Matrix*pm, const double*vect)
{
    // zakladam ze to wektor o 1 kolumnie o dlugosci liczby wierszy macierzy
    // jezeli wektor tu jest dluszy to sie nie da tego wywnioskowac bez podanego rozmiaru
    for(int i =0;i< pm->rows;i++)
    {
        double sum = 0;
        for(int j = 0; j < pm->cols;j++)
            sum += pm->val[i][j];

        result[i] = sum * vect[i];
    }
}
double norm(const Matrix*pm)
{
    double sum = 0;

    for(int i =0; i < pm->rows;i++)
        for(int j = 0; j < pm->cols;j++)
            sum += pm->val[i][j]*pm->val[i][j];

    if (sum == 0) // zerowe wartosci pol to znaczy suma = 0
        return 0;

    return sqrt(sum);
}

typedef char Data;
typedef struct tagStackElement
{
    struct tagStackElement* next;
    Data data;
} StackElement;
typedef struct tagStack
{
    StackElement* head;
} Stack;
void push(Stack *plist, const Data data)
{
    StackElement *new;
    new->next = NULL;
    new->data = data;

    if (plist->head == NULL)
        plist->head = new;

    StackElement* i = plist->head;
    while(i->next != NULL)
        i = i->next;

    i->next = new;
}
int peek(const Stack *plist)
{
    if( plist->head == NULL)
        return -1;

    StackElement* i = plist->head;
    while(i->next != NULL)
        i = i->next;

    // tu cos nie gra z zadaniem bo data to char a zwracam int
    return i->data;
}
int pop(Stack *plist, Data *pdata)
{
    if (plist->head == NULL)
        return -1;

    if (plist->head->next == NULL)
    {
        pdata = &plist->head->data;
        plist->head = NULL;
        return  0;
    }

    StackElement* previ = plist->head;
    StackElement* i = plist->head->next;
    while(i->next != NULL)
    {
        previ = i;
        i = i->next;
    }

    pdata = &i->data;
    previ->next = NULL;

    return 1;
}
int readtillend(FILE* file)
{
    Stack stack;
    char x = (char) fgetc(file);
    int count = 0;
    while (x != EOF)
    {
        if (x =='('||x =='{'||x =='[')
            push(&stack, x);


        if (x ==')'||x =='}'||x ==']')
        {
            Data z;
            int d = pop(&stack, &z);
            if (d == -1)
                return -1;

            if (z == '(' && x !=')')
                    return  -1;
            if (z == '{' && x !='}')
                    return  -1;
            if (z == '[' && x !=']')
                    return  -1;
        }

        x = (char) fgetc(file);
    }

    return count;
}