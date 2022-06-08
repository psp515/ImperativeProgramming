#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union
{
    int int_data;
    char char_data;
    void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element
{
    struct ht_element *next;
    data_union data;
} ht_element;

typedef struct hash_table
{
    int size;
    int no_elements;
    ht_element **ht; // dlaczego tu jest ** a nie po prostu *

    DataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    HashFp hash_function;
    DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if(ptr) return ptr;
    exit(MEMORY_ALLOCATION_ERROR);
}

void insert_element(hash_table *, data_union *);

// ---------------------- functions to implement

// initialize table fields
// OK
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
             CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data)
{
    if (size == 0)
    {
        p_table->size = size;
        p_table->no_elements = 0;
    }
    else
    {
        p_table->no_elements = 0;
        p_table->size = size;

        p_table->ht = safe_malloc(size * sizeof(ht_element*));

        for(int i = 0; i < size; i++)
        {
            p_table->ht[i] = safe_malloc(sizeof(ht_element));
            p_table->ht[i]->next = NULL;
            p_table->ht[i]->data.int_data=-1;
        }
    }

    p_table->hash_function = hash_function;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->modify_data = modify_data;
    p_table->dump_data = dump_data;
}

// print elements of the list with hash n
// OK
void dump_list(const hash_table* p_table, int n)
{
    ht_element* ht = p_table->ht[n]->next;
    
    while (ht != NULL)
    {
        p_table->dump_data(ht->data);
        ht = ht->next;
    }

}

// Free element pointed by data_union using free_data() function
// Tu sie moze wysypać
void free_element(DataFp free_data, ht_element *to_delete)
{
    if (free_data != NULL)
        free_data(to_delete->data);

    free(to_delete);
}
// free all elements from the table (and the table itself)
// Ok
void free_table(hash_table* p_table)
{

    for(int i = 0; i < p_table->size; i++)
    {

        ht_element *x = p_table->ht[i]->next;

        while (x != NULL)
        {
            ht_element *tmp = x->next;
            free_element(p_table->free_data, x);
            x = tmp;
        }

        free(p_table->ht[i]);
    }

    free(p_table->ht);
}

// calculate hash function for integer k
int hash_base(int k, int size)
{
    static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
    double tmp = k * c;
    return (int)floor(size * (tmp - floor(tmp)));
}

//TODO
void rehash(hash_table *p_table)
{
    ht_element ** new_ht = safe_malloc(p_table->size * 2 * sizeof(ht_element*));
    ht_element ** old_ht = p_table->ht;

    for(int i = 0; i <  p_table->size*2; i++)
    {
        new_ht[i] = safe_malloc(sizeof(ht_element));
        new_ht[i]->next = NULL;
    }

    for( int i = 0; i < p_table->size; i++)
    {
        ht_element * x = old_ht[i]->next;
        while (x != NULL)
        {
            ht_element * next = x->next;

            // dodajemy x do nawej tablicy
            int index = p_table->hash_function(x->data, p_table->size * 2);
            x->next = new_ht[index]->next;
            new_ht[index]->next = x;

            x = next;
        }
    }

    p_table->ht = new_ht;
    p_table->size = 2 * p_table->size;
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first)
{

}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data)
{
    int n = p_table->hash_function(*data, p_table->size);

    ht_element *prev = p_table->ht[n];
    ht_element *curr = p_table->ht[n]->next;

    while (curr!= NULL)
    {
        if(p_table->compare_data(curr->data, *data)==0)
            break;

        prev = curr;
        curr = curr->next;
    }


    return curr;
}

// insert element
void insert_element(hash_table *p_table, data_union *data)
{
    // (stosunek liczby elementow tablicy do jej dlugosci)]

    if( p_table->no_elements / p_table->size > MAX_RATE)
        rehash(p_table);

    int n = p_table->hash_function(*data, p_table->size);
    ht_element* i = p_table->ht[n];
    while (i->next != NULL)
    {
        if (p_table->compare_data(i->next->data , *data) == 0)
        {
            if(p_table->modify_data != NULL)
                p_table->modify_data(&i->next->data);

            return;
        }
        i = i->next;
    }


    ht_element * new_el = safe_malloc(sizeof(ht_element));
    new_el->data = *data;
    new_el->next = p_table->ht[n]->next;
    p_table->ht[n]->next = new_el;

    p_table->no_elements += 1;
}

// remove element
void remove_element(hash_table *p_table, data_union data)
{
    // tu sie cos posypało znowu
    int n = p_table->hash_function(data, p_table->size);

    ht_element* ht = p_table->ht[n]->next;
    ht_element* prev = p_table->ht[n];
    
    while ( ht != NULL) // tu moze sie wykrzaczyc
    {
        if(p_table->compare_data(ht->data, data) == 0)
            break;
        prev = ht;
        ht = ht->next;
    }

    if (ht == NULL)
        return;

    prev->next = ht->next;

    if(p_table->free_data != NULL)
        p_table->free_data(ht->data);
        
    free(ht);

    p_table->no_elements -= 1;
}

// type-specific definitions

// int element

int hash_int(data_union data, int size)
{
    return hash_base(data.int_data, size);
}

void dump_int(data_union data)
{
    printf("%d ", data.int_data);
}

// returns x > 0 if a > b
int cmp_int(data_union a, data_union b)
{
    return a.int_data - b.int_data;
}

// read int value and insert to the union
data_union create_data_int()
{
    int x;
    scanf("%d", &x);

    data_union data;
    data.int_data = x;

    return data;
}

// char element

int hash_char(data_union data, int size)
{
    return hash_base((int)data.char_data, size);
}

void dump_char(data_union data)
{
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b)
{
    char aw = a.char_data;
    char bw = b.char_data;

    if (aw < bw)
        return -1;
    else if (aw > bw)
        return 1;
    else
        return 0;
}

// read char value and insert to the union
data_union create_data_char()
{
    char x;
    scanf(" %c", &x);

    data_union data;
    data.char_data = x;

    return data;
}

// Word element

typedef struct DataWord
{
    char *word;
    int counter;
} DataWord;

void dump_word(data_union data)
{
    DataWord x = *(DataWord*) data.ptr_data;
    printf("%s %d", x.word, x.counter);
}

void free_word(data_union data)
{
    DataWord* x = (DataWord*) data.ptr_data;

    free(x->word);
    free(x);
}

int cmp_word(data_union a, data_union b)
{
    return strcmp( ((DataWord*)a.ptr_data)->word ,((DataWord*)b.ptr_data)->word);
}

int hash_word(data_union data, int size) {
    int s = 0;
    DataWord *dw = (DataWord*)data.ptr_data;
    char *p = dw->word;
    while (*p) {
        s += *p++;
    }
    return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data)
{
    DataWord *x = (DataWord*) data->ptr_data;
    x->counter += 1;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value)
{
    // value zaalokowane
    char* element = strdup(value);

    for(int i = 0; i < strlen(element); i++)
        element[i] = tolower(element[i]);

    DataWord *x = safe_malloc(sizeof(DataWord));

    x->word = element;
    x->counter = 1;

    data_union data;
    data.ptr_data = x;

    return data;
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream)
{
    char buff[BUFFER_SIZE];
    char *ptr = fgets(buff, BUFFER_SIZE, stream);
    char *sep1 = ".,?!:;-\n\t\r ";

    while (ptr != NULL)
    {
        char *word;
        for(word = strtok(buff, sep1); word; word = strtok(NULL, sep1))
        {
            //printf("%s\n",word);
            fflush(stdout);
            data_union data = create_data_word(word);
            insert_element(p_table, &data);
        }

        ptr = fgets(buff, BUFFER_SIZE, stream);
    }
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
    char op;
    data_union data;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        data = create_data();
        switch (op) {
            case 'r':
                remove_element(p_table, data);
                break;
            case 'i':
                insert_element(p_table, &data);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n, index;
    hash_table table;
    char buffer[BUFFER_SIZE];
    data_union data;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer table
            scanf("%d %d",&n, &index);
            init_ht(&table, 4, dump_int,NULL,
                    cmp_int, hash_int, NULL);
            table_test(&table, n, create_data_int);
            printf ("%d\n", table.size);
            dump_list(&table, index);
            free_table(&table);
            break;
        case 2: // test char table
            scanf("%d %d",&n, &index);
            init_ht(&table, 4, dump_char,NULL,
                    cmp_char, hash_char, NULL);
            table_test(&table, n, create_data_char);
            printf ("%d\n", table.size);
            dump_list(&table, index);
            free_table(&table);
            break;
        case 3: // read words from text, insert into table, and print
            scanf("%s", buffer);
            init_ht(&table, 8, dump_word, free_word,
                    cmp_word, hash_word, modify_word);
            stream_to_ht(&table, stdin);
            printf ("%d\n", table.size);
            data = create_data_word(buffer);
            ht_element *e = get_element(&table, &data);
            if (e)
                table.dump_data(e->data);
            //if (table.free_data)
                //table.free_data(data);
            free_table(&table);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

