#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
    int first;
    int second;
} pair;

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(array[i].first == array[i].second)
            continue;
        else
            return 0;
    }
    return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(array[i].second != array[i].first)
            continue;
        else
            return 0;
    }

    return 1;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(array[i].first == array[j].second && array[j].first == array[i].second)
                continue;
            else
                return 0;
        }
    }
    return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(array[i].first == array[j].second && array[j].first == array[i].second)
            {
                if(array[i].first != array[j].second || array[j].first != array[i].second)
                    return 0;
            }
        }
    }
    return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(array[i].first > array[j].second && array[j].first > array[i].second)
                continue;
            else
                return 1;
        }
    }
    return 0;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            for(int k = 0; k < size; k++)
            {
                if(array[i].first == array[j].second && array[j].first == array[i].second)
                {
                    if(array[j].first == array[k].second && array[k].first == array[j].second)
                    {
                        if(array[i].first == array[k].second && array[k].first == array[i].second)
                            continue;
                        else
                            return 0;
                    }
                }
            }
        }
    }
    return 1;
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* arr, int size)
{
    if(is_antisymmetric(arr, size) && is_transitive(arr,size) && is_reflexive(arr, size))
        return 1;
    return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if((arr[i].first == arr[j].second && arr[j].first == arr[i].second)||
               (arr[j].first == arr[i].second && arr[i].first == arr[j].second)||
               (arr[i].first == arr[j].first && arr[j].second == arr[i].second))
                continue;
            else
                return 0;
        }
    }
    return 1;
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair* arr, int size)
{
    if(is_partial_order(arr, size) && is_connected(arr, size))
        return 1;

    return 0;
}


int find_max_elements(pair* arr, int size, int* max){}
int find_min_elements(pair* arr, int size, int* min){}
int get_domain(pair* arr, int size, int* domain){}

// Case 3:
int composition (pair* arr1, int n1, pair* arr2, int n2, pair* out_arr){}

int cmp (pair p1, pair p2) {
    if (p1.first == p2.first) return p1.second - p2.second;
    return p1.first - p2.first;
}

// Add pair to existing relation if not already there
int add_relation (pair* array, int size, pair p)
{
    // tego chyba mielismy uzyc w read relation

    return 0;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* array)
{
    int size = 0;
    scanf("%d",&size);

    for(int i = 0; i < size; i++)
    {
        pair p1;

        scanf("%d",&p1.first);
        scanf("%d",&p1.second);

        int is_ok = 1;
        int j = 0;
        for(; j < i; j++)
        {
            if(array[j].first == p1.first && array[j].second == p1.second)
            {
                is_ok = 0;
                break;
            }
        }

        if(is_ok==1)
            array[j] = p1;

    }

    return size;
}

void print_int_array(int *array, int n) {
    printf("%d\n", n);
    for (int i = 0; i < n; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];

    scanf("%d",&to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size));
            print_int_array(domain, n_domain);
            if (!ordered) break;
            int max_elements[MAX_REL_SIZE];
            int min_elements[MAX_REL_SIZE];
            int no_max_elements = find_max_elements(relation, size, max_elements);
            int no_min_elements = find_min_elements(relation, size, min_elements);
            print_int_array(max_elements, no_max_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            printf("%d\n", composition(relation, size,
                                       relation_2, size_2, comp_relation));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

