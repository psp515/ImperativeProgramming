#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
    int first;
    int second;
} pair;


void print_rel_array(pair *array, int n)
{
    printf("%d\n", n);

    for (int i = 0; i < n; ++i)
        printf("%d %d \n", array[i].first, array[i].second);

    printf("\n");
}

int is_in_relation_withthemself(int val, pair* array, int size)
{
    for(int j = 0; j < size; j++)
        if (array[j].first == val && array[j].second == val)
            return 1;

    return 0;
}

// for xRy checks if yRx occurs
int is_having_symmetry(pair p, pair* array, int size)
{
    for(int i = 0; i < size; i++)
        if(p.first == array[i].second && p.second == array[i].first)
            return 1;

    return 0;
}

int compare( const void* a, const void* b)
{
    int int_a = * ( (int*) a );
    int int_b = * ( (int*) b );

    if ( int_a == int_b )
        return 0;
    else if ( int_a < int_b )
        return -1;
    else
        return 1;
}

// for xRy checks if yRx occurs
int contains_relation(pair p, pair* array, int size)
{
    for(int i = 0; i < size; i++)
        if(p.first == array[i].first && p.second == array[i].second)
            return 1;

    return 0;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* array , int size)
{
    for(int i = 0; i < size; i++)
        if (is_in_relation_withthemself(array[i].first, array, size) == 0 ||
            is_in_relation_withthemself(array[i].second, array, size) == 0)
            return 0;

    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* array, int size)
{
    for(int i = 0; i < size; i++)
        if (!is_in_relation_withthemself(array[i].first, array, size) == 0 && !is_in_relation_withthemself(array[i].second, array,size) == 0)
            return 0;

    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* array, int size)
{
    for(int i = 0; i < size; i++)
        if(is_having_symmetry(array[i], array, size) == 0)
            return 0;

    return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* array, int size)
{
    for(int i = 0; i < size; i++)
        for( int j = 0; j < size; j++)
            if(array[i].first == array[j].second && array[i].second == array[j].first)
                if(i!=j)
                    return 0;

    return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
// To moze byc zle
int is_asymmetric(pair* array, int size)
{
    for(int i = 0; i < size; i++)
        if(is_having_symmetry(array[i], array, size) == 1)
            return 0;

    return 1;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
// do sprawdzenia jak cos
int is_transitive(pair* array, int size)
{
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            for(int k = 0; k < size; k++)
                if(array[i].second == array[j].first)
                {
                    pair p = {array[i].first,array[j].second};
                    if(contains_relation(p, array,size)==0)
                        return 0;
                }

    return 1;
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
// OK
int is_partial_order(pair* arr, int size)
{
    if(is_antisymmetric(arr, size) && is_transitive(arr,size) && is_reflexive(arr, size))
        return 1;

    return 0;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* array, int size)
{
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
        {
            pair p1 = {array[i].first,array[j].first};
            pair p2 = {array[j].first,array[i].first};

            pair p3 = {array[i].second,array[j].second};
            pair p4 = {array[j].second,array[i].second};

            pair p5 = {array[i].first,array[j].second};
            pair p6 = {array[j].first,array[i].second};

            pair p7 = {array[i].second,array[j].first};
            pair p8 = {array[j].second,array[i].first};

            if(contains_relation(p1, array, size) == 0 && contains_relation(p2, array, size) == 0)
                return 0;
            if(contains_relation(p2, array, size) == 0 && contains_relation(p3, array, size) == 0)
                return 0;
            if(contains_relation(p4, array, size) == 0 && contains_relation(p5, array, size) == 0)
                return 0;
            if(contains_relation(p6, array, size) == 0 && contains_relation(p7, array, size) == 0)
                return 0;
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

int find_max_elements(pair* array, int size, int* max)
{
    int count = 0;

    for(int i = 0; i < size; i++)
    {
        int is_ok = 1;
        int potential_max = array[i].second;
        for(int j = 0; j < size; j++)
        {
            if(array[j].first == potential_max && array[j].second != potential_max)
            {
                is_ok = 0;
                break;
            }
        }
        if(is_ok == 1)
        {
            int is_copy = 0;
            for(int j = 0; j < count; j++)
                if(max[j] == potential_max)
                {
                    is_copy = 1;
                    break;
                }

            if(is_copy==0)
            {
                max[count] = potential_max;
                count++;
            }
        }
    }

    qsort( max, count, sizeof(int), compare );

    return count;
}

int find_min_elements(pair* array, int size, int* min)
{
    int count = 0;

    for(int i = 0; i < size; i++)
    {
        int is_ok = 1;
        int potential_min = array[i].first;
        for(int j = 0; j < size; j++)
        {
            if(array[j].second == potential_min && array[j].first != potential_min)
            {
                is_ok = 0;
                break;
            }
        }

        if(is_ok == 1)
        {
            int is_copy = 0;
            for(int j = 0; j < count; j++)
                if(min[j] == potential_min)
                {
                    is_copy = 1;
                    break;
                }

            if(is_copy==0)
            {
                min[count] = potential_min;
                count++;
            }
        }
    }

    qsort( min, count, sizeof(int), compare );

    return count;
}

int get_domain(pair* array, int size, int* domain)
{
    int j = 0;
    domain[j] = array[0].first;
    j++;

    if(array[0].first != array[0].second)
    {
        domain[j] = array[0].second;
        j++;
    }

    for(int i = 1; i < size; i++)
    {
        int is_ok = 1;
        for(int k = 0; k < j;k++)
        {
            if(domain[k] == array[i].first)
            {
                is_ok = 0;
                break;
            }
        }

        if (is_ok == 1)
        {
            int k = j-1;
            while(k >= 0 && domain[k] > array[i].first)
            {
                domain[k+1] = domain[k];
                k--;
            }

            domain[k+1] = array[i].first;
            j++;
        }

        is_ok = 1;

        for(int k = 0; k < j;k++)
        {
            if(domain[k] == array[i].second)
            {
                is_ok = 0;
                break;
            }
        }

        if (is_ok == 1)
        {
            int k = j-1;
            while(k >= 0 && domain[k] > array[i].second)
            {
                domain[k+1] = domain[k];
                k--;
            }

            domain[k+1] = array[i].second;
            j++;
        }
    }

    return j;
}


// Case 3:
int composition (pair* arr1, int n1, pair* arr2, int n2, pair* out_arr)
{
    int count = 0;

    for(int i =0; i < n1; i++)
    {
        for(int j = 0; j < n2; j++)
        {
            if(arr1[i].second == arr2[j].first)
            {
                pair p = {arr1[i].first,arr2[j].second };
                if(count==0)
                {
                    out_arr[count] = p;
                    count++;
                }
                else
                {
                    int is_ok = 1;
                    for(int k = 0; k < count; k++)
                    {
                        if(out_arr[k].first == p.first && out_arr[k].second == p.second)
                        {
                            is_ok =0;
                            break;
                        }
                    }

                    if(is_ok == 1)
                    {
                        out_arr[count] = p;
                        count++;
                    }
                }
            }
        }
    }

    return count;
}

int cmp (pair p1, pair p2)
{
    if (p1.first == p2.first)
        return p1.second - p2.second;
    return p1.first - p2.first;
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

void print_int_array(int *array, int n)
{
    printf("%d\n", n);
    for (int i = 0; i < n; ++i)
        printf("%d ", array[i]);

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

            // is order zmienia cos w relacji

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