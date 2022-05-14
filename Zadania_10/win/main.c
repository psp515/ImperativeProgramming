#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
    // dane
    void *data;
    // rozmiar elementu
    size_t element_size;
    // liczba elemetów w tablicy
    size_t size;
    // maksymalna pojemność tablicy
    size_t capacity;
} Vector;

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
// OK
void init_vector(Vector *vector, size_t block_size, size_t element_size)
{
    vector->data = malloc(block_size * element_size) ;
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
// OK
void reserve(Vector *vector, size_t new_capacity)
{
    if(new_capacity > vector->capacity)
    {
        vector->capacity = new_capacity;
        vector->data = realloc(vector->data,new_capacity * vector->element_size);
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
// TODO
void resize(Vector *vector, size_t new_size)
{
    if (new_size == vector->size)
        return;

    if (new_size > vector->size)
    {

        char* z = (char*) vector->data;
        z += vector->element_size*vector->size;

        int range = new_size - vector->size;
        for (size_t i = 0; i <range ; i++)
        {
            memcpy(z, calloc(1, vector->element_size), vector->element_size);
            z += vector->element_size;
        }


    }

    vector->size = new_size;
}

// Add element to the end of the vector
// OK
void push_back(Vector *vector, void *value)
{
    if (vector->capacity == vector->size)
        reserve(vector,vector->capacity * 2);


    char* z = (char*) vector->data;

    z += vector->element_size*vector->size;
    memcpy(z, value, vector->element_size);
    vector->size = vector->size + 1;
}

// Remove all elements from the vector
// OK
void clear(Vector *vector)
{
    vector->size = 0;
}

// Remove the last element from the vector
// OK
void pop_back(Vector *vector)
{
    vector->size = vector->size - 1;
}

// Insert new element at index (0 <= index <= size) position
// TODO
void insert(Vector *vector, int index, void *value)
{
    if (vector->size + 1 > vector->capacity)
        reserve(vector, vector->capacity * 2);


    if (index == vector->size)
    {
        push_back(vector, value);
        return;
    }

    // 	// Move pointer do insert index
    char *z = (char *)vector->data;
    z += vector->element_size * index;

    char *last_element = (char *)vector->data;
    last_element += vector->element_size * vector->size;
    char *previous_element = last_element - vector->element_size;
    // 	// Move data
    int vector_size = vector->size;
    while (vector_size > index)
    {
        memcpy(last_element, previous_element, vector->element_size);
        vector_size -= 1;
        last_element = previous_element;
        previous_element -= vector->element_size;
    }

    memcpy(last_element, value, vector->element_size);
    vector->size += 1;
}

// Erase element at position index
// TODO
void erase(Vector *vector, int index)
{
    char *z = (char *)vector->data;
    z += (vector->element_size) * index;

    char *next_element = (char *)vector->data;
    next_element += (vector->element_size) * (index + 1);

    int x = index;
    for( int x = index;x<vector->size-1;x++)
    {
        memcpy(z, next_element, vector->element_size);
        z = next_element;
        next_element += vector->element_size;
    }

    vector->size -= 1;
}

// Erase all elements that compare equal to value from the container
// OK
void erase_value(Vector *vector, void *value, int(*cmp)(const void*, const void*))
{
    char* z = (char*) vector->data;
    int dynamic_size = vector->size;
    int i =0;
    while (i<dynamic_size)
    {
        if(cmp(z, value)==0)
        {
            erase(vector,i);
            dynamic_size--;
            continue;
        }

        z +=vector->element_size;
        i++;
    }
}

// Erase all elements that satisfy the predicate from the vector
// OK
void erase_if(Vector *vector, int (*predicate)(void *))
{
    char* z = (char*) vector->data;
    int dynamic_size = vector->size;
    int i =0;
    while (i<dynamic_size)
    {
        if(predicate(z))
        {
            erase(vector,i);
            dynamic_size--;
            continue;
        }

        z +=vector->element_size;
        i++;
    }
}

// Request the removal of unused capacity
// OK
void shrink_to_fit(Vector *vector)
{
    if (vector->size == vector->capacity)
        return;

    vector->data = realloc(vector->data, vector->size * vector->element_size);
    vector->capacity = vector->size;
}

// Print integer vector
// OK
void print_vector_int(Vector *vector)
{
    int * ints = (int*)vector->data;
    printf("%zu\n", vector->size);
    for(int i = 0; i < vector->size;i++)
    {
        int x = ints[i];
        printf("%d ", x);
    }
}

// Print char vector
// OK
void print_vector_char(Vector *vector)
{
    char * chars = (char*)vector->data;
    printf("%zu\n", vector->capacity);
    for(int i = 0; i < vector->size;i++)
        printf("%c ", chars[i]);
}

// Print vector of Person
// OK
void print_vector_person(Vector *vector)
{
    Person * persons = (Person*)vector->data;
    printf("%zu\n", vector->capacity);
    for(int i = 0;i < vector->size;i++)
        printf("%d %s %s\n",persons[i].age, persons[i].first_name, persons[i].last_name);
}

// integer comparator - increasing order
// OK??
int int_cmp(const void *v1, const void *v2)
{
    int p1c = *(int*)v1;
    int p2c = *(int*)v2;

    if (p1c < p2c)
        return -1;
    else if (p1c > p2c)
        return 1;
    else
        return 0;
}

// char comparator - lexicographical order (case sensitive)
// OK
int char_cmp(const void *v1, const void *v2)
{
    char p1c = *(char*)v1;
    char p2c = *(char*)v2;

    if (p1c < p2c)
        return -1;
    else if (p1c > p2c)
        return 1;
    else
        return 0;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
// OK
int person_cmp(const void *p1, const void *p2)
{
    Person p1c = *(Person*)p1;
    Person p2c = *(Person*)p2;
    if (p1c.age==p2c.age)
    {
        int i = strcmp(p1c.first_name, p2c.first_name);
        if (i==0)
            return strcmp(p1c.last_name, p2c.last_name);
        return i;
    }

    return p2c.age - p1c.age;

}

// predicate: check if number is even
// OK
int is_even(void *value)
{
    return *(int*)value % 2 == 0;
}

// predicate: check if char is a vowel
// OK
int is_vowel(void *value)
{
    char x = *(char*)value;
    return  x == 'a' || x == 'e' || x == 'i' || x == 'o' || x == 'u' || x == 'y' ||
            x == 'A' || x == 'E' || x == 'I' || x == 'O' || x == 'U' || x == 'Y';

}

// predicate: check if person is older than 25
// OK ??
int is_older_than_25(void *person)
{
    Person cp = *(Person*) person;
    return cp.age > 25;
}

// -------------------------------------------------------------

void read_int(void* value) {
    scanf("%d", (int*)value);
}

void read_char(void* value) {
    char c[2];
    scanf("%s", c);
    *(char*)value = c[0];
}

void read_person(void* value) {
    Person *person = (Person*)value;
    scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

void vector_test(Vector *vector, int n, void(*read)(void*),
                 int (*cmp)(const void*, const void*), int(*predicate)(void*)) {
    char op[2];
    int index;
    size_t size;
    void *v = malloc(vector->element_size);
    for (int i = 0; i < n; ++i) {
        scanf("%s", op);
        switch (op[0]) {
            case 'p': // push_back
                read(v);
                push_back(vector, v);
                break;
            case 'i': // insert
                scanf("%d", &index);
                read(v);
                insert(vector, index, v);
                break;
            case 'e': // erase
                scanf("%d", &index);
                read(v);
                erase(vector, index);
                erase_value(vector, v, cmp);
                break;
            case 'd': // erase (predicate)
                erase_if(vector, predicate);
                break;
            case 'r': // resize
                scanf("%zu", &size);
                resize(vector, size);
                break;
            case 'c': // clear
                clear(vector);
                break;
            case 'f': // shrink
                shrink_to_fit(vector);
                break;
            case 's': // sort
                qsort(vector->data, vector->size,
                      vector->element_size, cmp);
                break;
            default:
                printf("No such operation: %s\n", op);
                break;
        }
    }
    free(v);
}

int main(void) {
    int to_do, n;
    Vector vector_int, vector_char, vector_person;

    scanf("%d%d", &to_do, &n);

    switch (to_do) {
        case 1:
            init_vector(&vector_int, 4, sizeof(int));
            vector_test(&vector_int, n, read_int, int_cmp, is_even);
            print_vector_int(&vector_int);
            free(vector_int.data);
            break;
        case 2:
            init_vector(&vector_char, 2, sizeof(char));
            vector_test(&vector_char, n, read_char, char_cmp, is_vowel);
            print_vector_char(&vector_char);
            free(vector_char.data);
            break;
        case 3:
            init_vector(&vector_person, 2, sizeof(Person));
            vector_test(&vector_person, n, read_person, person_cmp, is_older_than_25);
            print_vector_person(&vector_person);
            free(vector_person.data);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }

    return 0;
}


/*

1
12
p 10
p 20
p 5
p 3
p 15
i 0 30
i 4 40
i 7 50
e 4 10
e 4 20
r 6
f

1
6
p 10
p 20
p 5
p 3
p 15
i 0 30

*/