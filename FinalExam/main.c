#include <stdio.h>
#include <math.h>
#include <string.h>
#include <cstdlib>

# There is no task description so it makes this a bit useless.

# Task 1 Score: 25/30

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

    return 1; 
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

    if (sum == 0) 
        return 0;

    return sqrt(sum);
}



# Task 2 Score: 24/35

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

//Task 3 Score: 18/35

typedef struct {
    int size; 
    int maxsize; 
    char **strings; 
} StringTable;

void st_append(StringTable *p_dst, const StringTable *p_src);
void st_qsort(StringTable *st, int (*cmp)(const void*, const void*));


int st_insert(StringTable *st, const char *txt, int index)
{
    if(index >= st->size)
        return 1;

    char* copy = "";
    strcpy(copy, txt);




    return 0;
}

int st_mul_by_to(StringTable *p_dst, const StringTable *p_src, int cnt)
{
    if(cnt < 1)
        return 1;

    for (int x = 0 ; x < cnt; x++)
    {
        for (int i =0 ; i < p_src->size; i++)
        {
            // zakladam ze append alokuje nowy element bo nie pisze jak działa
            st_append(p_dst, p_src->strings[i]);
        }
    }

    return 0;
}

void st_zip(StringTable *p_dst, const StringTable *p_src_1, const StringTable *p_src_2)
{
    int x = p_src_1->size < p_src_2->size ? p_src_1->size:p_src_2->size;

    for(int i = 0 ; i < x;i++)
    {
        char* new = strcat(p_src_1->strings[i], p_src_2->strings[i]);
        st_append(p_dst, &new);
    }

    if(x == p_src_1->size &&p_src_1->size != p_src_2->size)
    {
        for(int i =x;i < p_src_2->size;i++)
            st_append(p_dst, p_src_2->strings[i]);
    }

    if(x == p_src_2->size && p_src_1->size != p_src_2->size)
    {
        for(int i =x;i < p_src_2->size;i++)
            st_append(p_dst, p_src_2->strings[i]);
    }
}
#define MAXSIZE 1024

void st_qsort(StringTable *st, int (*cmp)(const void*, const void*))
{
    qsort((void*)st->strings,(size_t)st->size,sizeof (char*), cmp);
}


int compare( const void *arg1, const void *arg2 )
{
    char* x = (char*) arg1;
    char* y = (char*)arg2;

    int lenx = (int) strlen(x);
    int leny = (int) strlen(y);

    if(x == y)
        return strcmp(x, y);

    return leny - lenx; 
}

void sort_it(StringTable *st)
{
    st_qsort(st, compare);
}