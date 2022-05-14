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

size_t get_last(size_t element_size, size_t size)
{
    return size * element_size;
}

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
// Seems OK
void init_vector(Vector *vector, size_t block_size, size_t element_size)
{
    vector->data = malloc(block_size * vector->element_size) ;
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
// Seems OK
void reserve(Vector *vector, size_t new_capacity)
{
    if(new_capacity > vector->capacity)
    {
        void* x = realloc(vector->data,new_capacity*vector->element_size);
        if(x == 0)
            return;
        vector->data = x;
        vector->capacity = new_capacity;
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
// OK??
void resize(Vector *vector, size_t new_size)
{
    if (new_size == vector->size)
        return;

    if (new_size > vector->size)
    {
        vector->size = new_size;
        vector->capacity = new_size;
        void* x = realloc(vector->data,new_size * vector->element_size);

        if(x == 0)
            return;

        char* z = (char*) vector->data;
        size_t last = get_last(vector->element_size,vector->size);

        memset(&z[last], 0, (new_size-last)*vector->element_size);

        vector->data = x;
    }

    vector->size = new_size;
}

// Add element to the end of the vector
// OK ??
void push_back(Vector *vector, void *value)
{
    if (vector->capacity == vector->size)
        reserve(vector,vector->capacity * 2);

    char* z = (char*) vector->data;
    size_t last = get_last(vector->element_size,vector->size);
    memcpy(&z[last], value, vector->element_size);
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
// OK??
void insert(Vector *vector, int index, void *value)
{
    vector->size = vector->size + 1;
    size_t i = index * vector->element_size;
    char* z = (char*) vector->data;
    memcpy(&z[i+vector->element_size], &z[i], (vector->size-1-index) * vector->element_size);
    memcpy(&z[i],value,vector->element_size);
}

// Erase element at position index
// OK??
void erase(Vector *vector, int index)
{
    if (index>vector->size || index < 0)
        return;

    size_t i = index * vector->element_size;
    char* z = (char*) vector->data;
    memcpy(&z[i], &z[i+vector->element_size], (vector->size-index) * vector->element_size);
    vector->size = vector->size - 1;
}

// Erase all elements that compare equal to value from the container
// OK
void erase_value(Vector *vector, void *value, int(*cmp)(const void*, const void*))
{
    char* z = (char*) vector->data;
    for(size_t i = vector->size - 1; i > -1; i--)
    {
        if (cmp(z+ i*vector->element_size, value)){
            erase(vector, i);
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
// OK
void erase_if(Vector *vector, int (*predicate)(void *))
{
    char* z = (char*) vector->data;
    for(size_t i = vector->size - 1; i > -1; i--)
    {
        if (predicate(z+ i*vector->element_size))
        {
            erase(vector, i);
        }
    }
}

// Request the removal of unused capacity
// OK
void shrink_to_fit(Vector *vector)
{
    size_t size = vector->size;

    if (size == vector->capacity)
        return;

    vector->data = realloc(vector->data, size * vector->element_size);
    vector->capacity = size;
}

// Print integer vector
// OK??
void print_vector_int(Vector *vector)
{
    int * ints = (int*)vector->data;
    size_t n = vector->size;
    printf("%zu\n", n);
    for(int i = 0; i < n;i++)
    {
        int x = ints[i];
        printf("%d ", x);
    }
}

// Print char vector
// OK??
void print_vector_char(Vector *vector)
{
    char * chars = (char*)vector->data;
    size_t n = vector->size;
    printf("%zu\n", n);
    for(int i = 0; i < n;i++)
        printf("%c ", chars[i]);
}

// Print vector of Person
// OK??
void print_vector_person(Vector *vector)
{
    Person * persons = (Person*)vector->data;
    size_t n = vector->size;
    printf("%zu\n", n);
    for(int i = 0;i < n;i++)
        printf("%d %s %s\n",persons[i].age, persons[i].first_name, persons[i].last_name);
}

// integer comparator - increasing order
// OK??
int int_cmp(const void *v1, const void *v2)
{
    int p1c = *(int*)v1;
    int p2c = *(int*)v2;

    return p1c > p2c ? 1 : -1;
}

// char comparator - lexicographical order (case sensitive)
// OK??
int char_cmp(const void *v1, const void *v2)
{
    char p1c = *(char*)v1;
    char p2c = *(char*)v2;

    return strcmp(&p1c, &p2c) > 0 ? 1 : -1;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
// OK??
int person_cmp(const void *p1, const void *p2)
{
    Person p1c = *(Person*)p1;
    Person p2c = *(Person*)p2;
    if (p1c.age==p2c.age)
    {
        int i = strcmp(p1c.first_name, p2c.first_name);
        if (i==0)
            return strcmp(p1c.last_name, p2c.last_name) > 0 ? 1 : -1;
        return i;
    }
    else if (p1c.age < p2c.age)
        return 1;
    else
        return -1;
}

// predicate: check if number is even
// OK??
int is_even(void *value)
{
    int x = *(int*)value;
    if(x % 2 == 0)
        return 1;
    return 0;
}

// predicate: check if char is a vowel
// OK ??
int is_vowel(void *value)
{
    char x = *(char*)value;
    if(x == 'a' || x == 'e' || x == 'i' || x == 'o' || x == 'u' || x == 'y')
        return 1;
    return 0;
}

// predicate: check if person is older than 25
// OK ??
int is_older_than_25(void *person)
{
    Person cp = *(Person*) person;
    return cp.age > 25 ? 1 : 0;
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


*/